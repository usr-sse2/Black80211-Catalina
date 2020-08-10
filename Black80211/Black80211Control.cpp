/* add your code here*/

typedef unsigned int ifnet_ctl_cmd_t;

#include "IONetworkInterface.h"
#include "IONetworkController.h"

#include "Black80211Control.hpp"

#include "debug.h"

OSDefineMetaClassAndStructors(Black80211Control, IO80211Controller);
#define super IO80211Controller

bool Black80211Control::init(OSDictionary* parameters) {
    IOLog("Black80211: Init");
    
    if (!super::init(parameters)) {
        IOLog("Black80211: Failed to call IO80211Controller::init!");
        return false;
    }
    
    dev = new FakeDevice();
    return true;
}

void Black80211Control::free() {
    IOLog("Black80211: Free");
    
    ReleaseAll();
    super::free();
}

IOService* Black80211Control::probe(IOService *provider, SInt32 *score) {
    IOLog("Black80211: probing");
    
    super::probe(provider, score);
    
    return this;
}

bool Black80211Control::createWorkLoop() {
    if(!fWorkloop) {
        fWorkloop = IO80211WorkLoop::workLoop();
    }
    
    return (fWorkloop != NULL);
}

IOWorkLoop* Black80211Control::getWorkLoop() const {
    return fWorkloop;
}

bool Black80211Control::start(IOService* provider) {
    IOLog("Black80211: Start");
    if (!super::start(provider)) {
        IOLog("Black80211: Failed to call IO80211Controller::start!");
        ReleaseAll();
        return false;
    }
    
    //fWorkloop = (IO80211WorkLoop *)getWorkLoop();
    if (!fWorkloop) {
        IOLog("Black80211: Failed to get workloop!");
        ReleaseAll();
        return false;
    }
    
    fCommandGate = IOCommandGate::commandGate(this);
    if (!fCommandGate) {
        IOLog("Black80211: Failed to create command gate!");
        ReleaseAll();
        return false;
    }
    
    if (fWorkloop->addEventSource(fCommandGate) != kIOReturnSuccess) {
        IOLog("Black80211: Failed to register command gate event source!");
        ReleaseAll();
        return false;
    }
    
    fCommandGate->enable();
    
    mediumDict = OSDictionary::withCapacity(MEDIUM_TYPE_INVALID + 1);
    addMediumType(kIOMediumIEEE80211None,  0,  MEDIUM_TYPE_NONE);
    addMediumType(kIOMediumIEEE80211Auto,  0,  MEDIUM_TYPE_AUTO);
    addMediumType(kIOMediumIEEE80211DS1,   1000000, MEDIUM_TYPE_1MBIT);
    addMediumType(kIOMediumIEEE80211DS2,   2000000, MEDIUM_TYPE_2MBIT);
    addMediumType(kIOMediumIEEE80211DS5,   5500000, MEDIUM_TYPE_5MBIT);
    addMediumType(kIOMediumIEEE80211DS11, 11000000, MEDIUM_TYPE_11MBIT);
    addMediumType(kIOMediumIEEE80211,     54000000, MEDIUM_TYPE_54MBIT, "OFDM54");
    //addMediumType(kIOMediumIEEE80211OptionAdhoc, 0, MEDIUM_TYPE_ADHOC,"ADHOC");
    
    if (!publishMediumDictionary(mediumDict)) {
        IOLog("Black80211: Failed to publish medium dictionary!");
        ReleaseAll();
        return false;
    }
    
    if (!setCurrentMedium(mediumTable[MEDIUM_TYPE_AUTO])) {
        IOLog("Black80211: Failed to set current medium!");
        ReleaseAll();
        return false;
    }
    if (!setSelectedMedium(mediumTable[MEDIUM_TYPE_AUTO])) {
        IOLog("Black80211: Failed to set selected medium!");
        ReleaseAll();
        return false;
    }
    
    /*
    if (!setLinkStatus(kIONetworkLinkValid, mediumTable[MEDIUM_TYPE_AUTO])) {
        IOLog("Black80211: Failed to set link status!");
        ReleaseAll();
        return false;
    }
     */
    if (!attachInterface((IONetworkInterface**) &fInterface, true)) {
        kprintf("Black80211: Failed to attach interface!");
        ReleaseAll();
        return false;
    }
        
    
    fInterface->registerService();
    registerService();

    
    return true;
}

IOReturn Black80211Control::enable(IONetworkInterface* iface) {
    kprintf("Black80211: enable");
    IOMediumType mediumType = kIOMediumIEEE80211Auto;
    IONetworkMedium *medium = IONetworkMedium::getMediumWithType(mediumDict, mediumType);
    setLinkStatus(kIONetworkLinkActive | kIONetworkLinkValid, medium);
    
    if(fInterface) {
        fInterface->postMessage(1);
    }
    
    return kIOReturnSuccess;
}

IOReturn Black80211Control::disable(IONetworkInterface* iface) {
    kprintf("Black80211: disable");
    return kIOReturnSuccess;
}

bool Black80211Control::addMediumType(UInt32 type, UInt32 speed, UInt32 code, char* name) {
    bool ret = false;
    
    IONetworkMedium* medium = IONetworkMedium::medium(type, speed, 0, code, name);
    if (medium) {
        ret = IONetworkMedium::addMedium(mediumDict, medium);
        if (ret)
            mediumTable[code] = medium;
        medium->release();
    }
    return ret;
}


void Black80211Control::stop(IOService* provider) {
    if (fCommandGate) {
        IOLog("Black80211::stop: Command gate alive. Disabling it.");
        fCommandGate->disable();
        IOLog("Black80211::stop: Done disabling command gate");
        if (fWorkloop) {
            IOLog("Black80211::stop: Workloop alive. Removing command gate");
            fWorkloop->removeEventSource(fCommandGate);
        }
    }
    
    if (fInterface) {
        IOLog("Black80211::stop: Detaching interface");
        detachInterface(fInterface, true);
        fInterface = NULL;
    }
    
    super::stop(provider);
}

IOReturn Black80211Control::getHardwareAddress(IOEthernetAddress* addr) {
    addr->bytes[0] = 0xAA;
    addr->bytes[1] = 0x99;
    addr->bytes[2] = 0x88;
    addr->bytes[3] = 0x77;
    addr->bytes[4] = 0x66;
    addr->bytes[5] = 0x55;
    return kIOReturnSuccess;
}

IOReturn Black80211Control::getHardwareAddressForInterface(IO80211Interface* netif,
                                                           IOEthernetAddress* addr) {
    return getHardwareAddress(addr);
}

SInt32 Black80211Control::apple80211Request(unsigned int request_type,
                                            int request_number,
                                            IO80211Interface* interface,
                                            void* data) {
    if (request_type != SIOCGA80211 && request_type != SIOCSA80211) {
        IOLog("Black80211: Invalid IOCTL request type: %u", request_type);
        IOLog("Expected either %lu or %lu", SIOCGA80211, SIOCSA80211);
        return kIOReturnError;
    }

    IOReturn ret = 0;
    
    bool isGet = (request_type == SIOCGA80211);
    
#define IOCTL(REQ_TYPE, REQ, DATA_TYPE) \
if (REQ_TYPE == SIOCGA80211) { \
ret = get##REQ(interface, (struct DATA_TYPE* )data); \
} else { \
ret = set##REQ(interface, (struct DATA_TYPE* )data); \
}
    
#define IOCTL_GET(REQ_TYPE, REQ, DATA_TYPE) \
if (REQ_TYPE == SIOCGA80211) { \
    ret = get##REQ(interface, (struct DATA_TYPE* )data); \
}
#define IOCTL_SET(REQ_TYPE, REQ, DATA_TYPE) \
if (REQ_TYPE == SIOCSA80211) { \
    ret = set##REQ(interface, (struct DATA_TYPE* )data); \
}
    
    kprintf("Black80211: IOCTL %s(%d) %s",
          isGet ? "get" : "set",
          request_number,
          IOCTL_NAMES[request_number]);
    
    switch (request_number) {
        case APPLE80211_IOC_SSID: // 1
            IOCTL(request_type, SSID, apple80211_ssid_data);
            break;
        case APPLE80211_IOC_AUTH_TYPE: // 2
            IOCTL_GET(request_type, AUTH_TYPE, apple80211_authtype_data);
            break;
        case APPLE80211_IOC_CHANNEL: // 4
            IOCTL_GET(request_type, CHANNEL, apple80211_channel_data);
            break;
        case APPLE80211_IOC_TXPOWER: // 7
            IOCTL_GET(request_type, TXPOWER, apple80211_txpower_data);
            break;
        case APPLE80211_IOC_RATE: // 8
            IOCTL_GET(request_type, RATE, apple80211_rate_data);
            break;
        case APPLE80211_IOC_BSSID: // 9
            IOCTL_GET(request_type, BSSID, apple80211_bssid_data);
            break;
        case APPLE80211_IOC_SCAN_REQ: // 10
            IOCTL_SET(request_type, SCAN_REQ, apple80211_scan_data);
            break;
        case APPLE80211_IOC_SCAN_RESULT: // 11
            IOCTL_GET(request_type, SCAN_RESULT, apple80211_scan_result*);
            break;
        case APPLE80211_IOC_CARD_CAPABILITIES: // 12
            IOCTL_GET(request_type, CARD_CAPABILITIES, apple80211_capability_data);
            break;
        case APPLE80211_IOC_STATE: // 13
            IOCTL_GET(request_type, STATE, apple80211_state_data);
            break;
        case APPLE80211_IOC_PHY_MODE: // 14
            IOCTL_GET(request_type, PHY_MODE, apple80211_phymode_data);
            break;
        case APPLE80211_IOC_OP_MODE: // 15
            IOCTL_GET(request_type, OP_MODE, apple80211_opmode_data);
            break;
        case APPLE80211_IOC_RSSI: // 16
            IOCTL_GET(request_type, RSSI, apple80211_rssi_data);
            break;
        case APPLE80211_IOC_NOISE: // 17
            IOCTL_GET(request_type, NOISE, apple80211_noise_data);
            break;
        case APPLE80211_IOC_INT_MIT: // 18
            IOCTL_GET(request_type, INT_MIT, apple80211_intmit_data);
            break;
        case APPLE80211_IOC_POWER: // 19
            IOCTL(request_type, POWER, apple80211_power_data);
            break;
        case APPLE80211_IOC_ASSOCIATE: // 20
            IOCTL_SET(request_type, ASSOCIATE, apple80211_assoc_data);
            break;
        case APPLE80211_IOC_SUPPORTED_CHANNELS: // 27
            IOCTL_GET(request_type, SUPPORTED_CHANNELS, apple80211_sup_channel_data);
            break;
        case APPLE80211_IOC_LOCALE: // 28
            IOCTL_GET(request_type, LOCALE, apple80211_locale_data);
            break;
        case APPLE80211_IOC_TX_ANTENNA: // 37
            IOCTL_GET(request_type, TX_ANTENNA, apple80211_antenna_data);
            break;
        case APPLE80211_IOC_ANTENNA_DIVERSITY: // 39
            IOCTL_GET(request_type, ANTENNA_DIVERSITY, apple80211_antenna_data);
            break;
        case APPLE80211_IOC_DRIVER_VERSION: // 43
            IOCTL_GET(request_type, DRIVER_VERSION, apple80211_version_data);
            break;
        case APPLE80211_IOC_HARDWARE_VERSION: // 44
            IOCTL_GET(request_type, HARDWARE_VERSION, apple80211_version_data);
            break;
        case APPLE80211_IOC_COUNTRY_CODE: // 51
            IOCTL_GET(request_type, COUNTRY_CODE, apple80211_country_code_data);
            break;
        case APPLE80211_IOC_RADIO_INFO:
            IOCTL_GET(request_type, RADIO_INFO, apple80211_radio_info_data);
            break;
        case APPLE80211_IOC_MCS: // 57
            IOCTL_GET(request_type, MCS, apple80211_mcs_data);
            break;
        case APPLE80211_IOC_WOW_PARAMETERS: // 69
            break;
        case APPLE80211_IOC_ROAM_THRESH:
            IOCTL_GET(request_type, ROAM_THRESH, apple80211_roam_threshold_data);
            break;
        case APPLE80211_IOC_TX_CHAIN_POWER: // 108
            break;
        case APPLE80211_IOC_THERMAL_THROTTLING: // 111
            break;
        default:
            kprintf("Black80211: unhandled ioctl %s %d", IOCTL_NAMES[request_number], request_number);
            break;
    }
#undef IOCTL
    
    return ret;
}

bool Black80211Control::configureInterface(IONetworkInterface *netif) {
    kprintf("Black80211: Configure interface");
    if (!super::configureInterface(netif)) {
        return false;
    }

    return true;
}

IO80211Interface* Black80211Control::getNetworkInterface() {
    return fInterface;
}

UInt32 Black80211Control::outputPacket(mbuf_t m, void* param) {
    freePacket(m);
    return kIOReturnSuccess;
}

IOReturn Black80211Control::getMaxPacketSize( UInt32* maxSize ) const {
    *maxSize = 1500;
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setPromiscuousMode(IOEnetPromiscuousMode mode) {
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setMulticastMode(IOEnetMulticastMode mode) {
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setMulticastList(IOEthernetAddress* addr, UInt32 len) {
    return kIOReturnSuccess;
}

SInt32 Black80211Control::monitorModeSetEnabled(IO80211Interface* interface,
                                                bool enabled,
                                                UInt32 dlt) {
    return kIOReturnSuccess;
}

const OSString* Black80211Control::newVendorString() const {
    return OSString::withCString("black_wizard");
}

const OSString* Black80211Control::newModelString() const {
    return OSString::withCString("BlackControl80211");
}

const OSString* Black80211Control::newRevisionString() const {
    return OSString::withCString("1.0");
}
