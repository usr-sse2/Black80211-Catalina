/* add your code here*/

typedef unsigned int ifnet_ctl_cmd_t;

#include "IONetworkInterface.h"
#include "IONetworkController.h"

#include "Black80211Control.hpp"
#include "Black80211Interface.hpp"

#include "debug.h"

OSDefineMetaClassAndStructors(Black80211Control, IO80211Controller);
#define super IO80211Controller

bool Black80211Control::init(OSDictionary* parameters) {
	IOCTL_NAMES[353] = "NSS";
	
    IOLog("Black80211: Init\n");
    
    if (!super::init(parameters)) {
        IOLog("Black80211: Failed to call IO80211Controller::init!\n");
        return false;
    }
	scan_result = nullptr;

	requestedScanning = false;
	powerState = APPLE80211_POWER_ON;
	networkIndex = 0;

	fInterface = nullptr;
	fTimerEventSource = nullptr;
	fWorkloop = nullptr;
	fCommandGate = nullptr;
	fProvider = nullptr;

	authtype_upper = 0;
	authtype_lower = 0;
	memset(&cipher_key, 0, sizeof(apple80211_key));

    return true;
}

void Black80211Control::free() {
    IOLog("Black80211: Free\n");
    
    ReleaseAll();
    super::free();
}

bool Black80211Control::useAppleRSNSupplicant(IO80211Interface *interface) {
	return true;
}

bool Black80211Control::useAppleRSNSupplicant(IO80211VirtualInterface *interface) {
	return true;
}

IOService* Black80211Control::probe(IOService *provider, SInt32 *score) {
    IOLog("Black80211: probing\n");

    if (!super::probe(provider, score))
		return NULL;

	fProvider = (Black80211Device*)provider->metaCast("Black80211Device");

	if (!fProvider) {
		IOLog("Black80211: failed to find itlwm\n");
		return NULL;
	}
	fProvider->retain();
	
    return this;
}

IONetworkInterface *Black80211Control::createInterface() {
	auto *interface = new Black80211Interface;
	if (interface == NULL)
		return NULL;
	if (!interface->init(this)) {
		interface->release();
		return NULL;
	}
	interface->setProperty(kIOBuiltin, true);
	return interface;
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
	OSDictionary *matchingDict = provider->serviceMatching("AppleSMC");
	if (!matchingDict)
		return false;
	
	IOService *smc = provider->waitForMatchingService(matchingDict);
	OSSafeReleaseNULL(matchingDict);

	if (!smc)
		return false; // too early
	
	OSSafeReleaseNULL(smc);	
	
    IOLog("Black80211: Start\n");
    
    createWorkLoop();
    if (!fWorkloop) {
        IOLog("Black80211: Failed to get workloop!\n");
        ReleaseAll();
        return false;
    }

    
    fProvider->setController(this);
	
    if (!super::start(provider)) {
        IOLog("Black80211: Failed to call IO80211Controller::start!\n");
        ReleaseAll();
        return false;
    }
	    
	fCommandGate = fProvider->getCommandGate();
    if (!fCommandGate) {
        IOLog("Black80211: Failed to create command gate!\n");
        ReleaseAll();
        return false;
    }
	fCommandGate->retain();

	fTimerEventSource = IOTimerEventSource::timerEventSource(this);
	if (!fTimerEventSource) {
		IOLog("Black80211: Failed to create timer event source!\n");
		ReleaseAll();
		return false;
	}

	fWorkloop->addEventSource(fTimerEventSource);

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
        IOLog("Black80211: Failed to publish medium dictionary!\n");
        ReleaseAll();
        return false;
    }
    
    if (!setCurrentMedium(mediumTable[MEDIUM_TYPE_AUTO])) {
        IOLog("Black80211: Failed to set current medium!\n");
        ReleaseAll();
        return false;
    }
    if (!setSelectedMedium(mediumTable[MEDIUM_TYPE_AUTO])) {
        IOLog("Black80211: Failed to set selected medium!\n");
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
    if (!attachInterface((IONetworkInterface**) &fInterface, false)) {
        IOLog("Black80211: Failed to attach interface!\n");
        ReleaseAll();
        return false;
    }

	//((uint8_t*)fInterface)[0x160] &= ~2; // disable use of Apple RSN supplicant
	//((uint64_t*)fInterface)[0x280] = 0xffffffffffffffffull; // ffffull debug!

	attach(provider);
	fProvider->setInterface(fInterface);
    
    fInterface->registerService();
    registerService();
    
    return true;
}

bool Black80211Control::setLinkStatus(
UInt32                  status,
const IONetworkMedium * activeMedium,
UInt64                  speed,
OSData *                data) {
	if (!fInterface)
		return false;
	IOLog("Changing link status: %d\n", status);
	bool ret = super::setLinkStatus(status, activeMedium, speed, data);
	if (status & kIONetworkLinkValid && status & kIONetworkLinkActive) {
		fInterface->setLinkState(kIO80211NetworkLinkUp, 0);
	}
	else if (status & kIONetworkLinkValid) {
		fInterface->setLinkState(kIO80211NetworkLinkDown, 0);
	}
	else {
		fInterface->setLinkState(kIO80211NetworkLinkUndefined, 0);
	}
	return ret;
}

IOReturn Black80211Control::enable(IONetworkInterface* iface) {
    IOLog("Black80211: enable");
    IOMediumType mediumType = kIOMediumIEEE80211Auto;
    IONetworkMedium *medium = IONetworkMedium::getMediumWithType(mediumDict, mediumType);
    setLinkStatus(kIONetworkLinkValid, medium);
	
	fProvider->enable();
    
    if(fInterface) {
        fInterface->postMessage(APPLE80211_M_POWER_CHANGED);
    }
    
    return kIOReturnSuccess;
}

IOReturn Black80211Control::disable(IONetworkInterface* iface) {
    IOLog("Black80211: disable");
	fProvider->disable();
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
        IOLog("Black80211::stop: Command gate alive. Disabling it.\n");
        fCommandGate->disable();
        IOLog("Black80211::stop: Done disabling command gate\n");
        if (fWorkloop) {
            IOLog("Black80211::stop: Workloop alive. Removing command gate\n");
            fWorkloop->removeEventSource(fCommandGate);
        }
    }
    
    if (fInterface) {
        IOLog("Black80211::stop: Detaching interface\n");
        detachInterface(fInterface, true);
		OSSafeReleaseNULL(fInterface);
        fInterface = NULL;
    }

	detach(provider);
    
    super::stop(provider);
}

IOReturn Black80211Control::getHardwareAddress(IOEthernetAddress* addr) {
	return fProvider->getMACAddress(addr);
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
        IOLog("Black80211: Invalid IOCTL request type: %u\n", request_type);
        IOLog("Expected either %lu or %lu\n", SIOCGA80211, SIOCSA80211);
        return kIOReturnError;
    }
	return fCommandGate->runActionBlock(^IOReturn{
		return apple80211RequestGated(request_type, request_number, interface, data);
	});
}

SInt32 Black80211Control::apple80211RequestGated(unsigned int request_type,
                                            int request_number,
                                            IO80211Interface* interface,
                                            void* data) {
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
    
    IOLog("Black80211: IOCTL %s(%d) %s\n",
          isGet ? "get" : "set",
          request_number,
          IOCTL_NAMES[request_number]);
    
    switch (request_number) {
        case APPLE80211_IOC_SSID: // 1
            IOCTL(request_type, SSID, apple80211_ssid_data);
            break;
        case APPLE80211_IOC_AUTH_TYPE: // 2
            IOCTL(request_type, AUTH_TYPE, apple80211_authtype_data);
            break;
		case APPLE80211_IOC_CIPHER_KEY: // 3
			IOCTL(request_type, CIPHER_KEY, apple80211_key);
			break;
        case APPLE80211_IOC_CHANNEL: // 4
            IOCTL_GET(request_type, CHANNEL, apple80211_channel_data);
            break;
		case APPLE80211_IOC_PROTMODE: // 6
			IOCTL_GET(request_type, PROTMODE, apple80211_protmode_data);
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
		case APPLE80211_IOC_SCAN_REQ_MULTIPLE:
			IOCTL_SET(request_type, SCAN_REQ_MULTIPLE, apple80211_scan_multiple_data);
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
		case APPLE80211_IOC_DISASSOCIATE: // 22
			if (request_type == SIOCSA80211)
				ret = setDISASSOCIATE(interface);
			break;
        case APPLE80211_IOC_SUPPORTED_CHANNELS: // 27
            IOCTL_GET(request_type, SUPPORTED_CHANNELS, apple80211_sup_channel_data);
            break;
		case APPLE80211_IOC_DEAUTH: // 29
			IOCTL_GET(request_type, DEAUTH, apple80211_deauth_data);
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
		case APPLE80211_IOC_RSN_IE: // 46
			IOCTL(request_type, RSN_IE, apple80211_rsn_ie_data);
			break;
		case APPLE80211_IOC_AP_IE_LIST: // 48
			IOCTL_GET(request_type, AP_IE_LIST, apple80211_ap_ie_data);
			break;
		case APPLE80211_IOC_ASSOCIATION_STATUS: // 50
			IOCTL_GET(request_type, ASSOCIATION_STATUS, apple80211_assoc_status_data);
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
        case APPLE80211_IOC_ROAM_THRESH:// 80
            IOCTL_GET(request_type, ROAM_THRESH, apple80211_roam_threshold_data);
            break;
		case APPLE80211_IOC_SCANCACHE_CLEAR: // 90
			if (request_type == SIOCSA80211)
				ret = setSCANCACHE_CLEAR(interface);
			break;
        case APPLE80211_IOC_TX_CHAIN_POWER: // 108
            break;
        case APPLE80211_IOC_THERMAL_THROTTLING: // 111
            break;
		case APPLE80211_IOC_LINK_CHANGED_EVENT_DATA: // 156
			IOCTL_GET(request_type, LINK_CHANGED_EVENT_DATA, apple80211_link_changed_event_data);
			break;
		case APPLE80211_IOC_HW_SUPPORTED_CHANNELS: // 254
			IOCTL_GET(request_type, HW_SUPPORTED_CHANNELS, apple80211_sup_channel_data);
			break;
		case APPLE80211_IOC_NSS: // 353
			IOCTL_GET(request_type, NSS, apple80211_nss_data);
			break;
        default:
            IOLog("Black80211: unhandled ioctl %s %d\n", request_number > 353 ? "" : IOCTL_NAMES[request_number], request_number);
            break;
    }
#undef IOCTL
    
    return ret;
}

bool Black80211Control::configureInterface(IONetworkInterface *netif) {
    IOLog("Black80211: Configure interface\n");
    if (!super::configureInterface(netif)) {
        return false;
    }

    return true;
}

IO80211Interface* Black80211Control::getNetworkInterface() {
    return fInterface;
}

const char* hexdump(uint8_t *buf, size_t len);

UInt32 Black80211Control::outputPacket(mbuf_t packet, void* param) {
	uint8_t ether_type[2];
	if (mbuf_len(packet) >= 14 && mbuf_copydata(packet, 12, 2, &ether_type) == 0 && (ether_type[0] == 0x88 && ether_type[1] == 0x8e)) { // EAPOL packet
		const char* dump = hexdump((uint8_t*)mbuf_data(packet), mbuf_len(packet));
		IOLog("Black80211: output EAPOL packet, len: %zu, data: %s\n", mbuf_len(packet), dump ? dump : "Failed to allocate memory");
		if (dump)
			IOFree((void*)dump, 3 * mbuf_len(packet) + 1);
	}
	return fProvider->outputPacket(packet, param);
}

int Black80211Control::outputActionFrame(IO80211Interface * interface, mbuf_t m) {
	IOLog("%s, length %zu\n", __FUNCTION__, mbuf_pkthdr_len(m));
	return fProvider->outputPacket(m, nullptr);
}

int Black80211Control::outputRaw80211Packet(IO80211Interface * interface, mbuf_t m) {
	IOLog("%s, length %zu\n", __FUNCTION__, mbuf_pkthdr_len(m));
	return fProvider->outputPacket(m, nullptr);	
}

IOReturn Black80211Control::getMaxPacketSize( UInt32* maxSize ) const {
    *maxSize = 1500;
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setPromiscuousMode(IOEnetPromiscuousMode mode) {
	IOLog("%s\n", __FUNCTION__);
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setMulticastMode(IOEnetMulticastMode mode) {
	IOLog("%s\n", __FUNCTION__);
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setMulticastList(IOEthernetAddress* addr, UInt32 len) {
	IOLog("%s\n", __FUNCTION__);
    return kIOReturnSuccess;
}

SInt32 Black80211Control::monitorModeSetEnabled(IO80211Interface* interface,
                                                bool enabled,
                                                UInt32 dlt) {
	IOLog("%s\n", __FUNCTION__);
    return kIOReturnSuccess;
}

const OSString* Black80211Control::newVendorString() const {
	return fProvider->newVendorString();
}

const OSString* Black80211Control::newModelString() const {
	return fProvider->newModelString();
}

const OSString* Black80211Control::newRevisionString() const {
    return OSString::withCString("1.0");
}
