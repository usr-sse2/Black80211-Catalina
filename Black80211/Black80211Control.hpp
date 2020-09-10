/* add your code here */
#ifndef net80211_Voodoo80211Device_h
#define net80211_Voodoo80211Device_h

#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/network/IOEthernetInterface.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/network/IOGatedOutputQueue.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOLocks.h>

#include <IOKit/network/IOPacketQueue.h>
#include <IOKit/network/IONetworkMedium.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IODeviceMemory.h>
#include <IOKit/assert.h>
#include <IOKit/IODataQueue.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/network/IONetworkController.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOInterruptEventSource.h>


#include <IOKit/network/IOGatedOutputQueue.h>

#include "interop.h"

#include "apple80211.h"

typedef enum {
    MEDIUM_TYPE_NONE = 0,
    MEDIUM_TYPE_AUTO,
    MEDIUM_TYPE_1MBIT,
    MEDIUM_TYPE_2MBIT,
    MEDIUM_TYPE_5MBIT,
    MEDIUM_TYPE_11MBIT,
    MEDIUM_TYPE_54MBIT,
    MEDIUM_TYPE_INVALID
} mediumType_t;

class Black80211Control : public IO80211Controller {
    
    OSDeclareDefaultStructors(Black80211Control)
    
public:
    bool init(OSDictionary* parameters) override;
    void free() override;
    bool start(IOService* provider) override;
    void stop(IOService* provider) override;
    IOService* probe(IOService* provider, SInt32* score) override;
    
    SInt32 apple80211Request(unsigned int request_type, int request_number, IO80211Interface* interface, void* data) override;
    SInt32 apple80211RequestGated(unsigned int request_type, int request_number, IO80211Interface* interface, void* data);
    UInt32 outputPacket (mbuf_t m, void* param) override;
    IOReturn getMaxPacketSize(UInt32* maxSize) const;
    const OSString* newVendorString() const;
    const OSString* newModelString() const;
    const OSString* newRevisionString() const;
    IOReturn enable(IONetworkInterface *netif) override;
    IOReturn disable(IONetworkInterface *netif) override;
    bool configureInterface(IONetworkInterface *netif) override;
    IO80211Interface* getNetworkInterface() override;
    IOReturn getHardwareAddressForInterface(IO80211Interface* netif, IOEthernetAddress* addr) override;
    IOReturn getHardwareAddress(IOEthernetAddress* addr) override;
    IOReturn setPromiscuousMode(IOEnetPromiscuousMode mode) override;
    IOReturn setMulticastMode(IOEnetMulticastMode mode) override;
    IOReturn setMulticastList(IOEthernetAddress* addr, UInt32 len) override;
    SInt32 monitorModeSetEnabled(IO80211Interface* interface, bool enabled, UInt32 dlt) override;
	
	int outputRaw80211Packet(IO80211Interface *, mbuf_t) override;
	int outputActionFrame(IO80211Interface *, mbuf_t) override;
    
    bool createWorkLoop() override;
    IOWorkLoop* getWorkLoop() const override;

	IOReturn message(UInt32 type, IOService * provider, void * argument = NULL) override;

	virtual bool setLinkStatus(
							   UInt32                  status,
							   const IONetworkMedium * activeMedium = 0,
							   UInt64                  speed        = 0,
							   OSData *                data         = 0) override;

	virtual IONetworkInterface * createInterface() override;

	virtual bool useAppleRSNSupplicant(IO80211Interface* interface) override;
	virtual bool useAppleRSNSupplicant(IO80211VirtualInterface* interface) override;

protected:
    IO80211Interface* getInterface();
    
private:
    // 1 - SSID
    IOReturn getSSID(IO80211Interface* interface, struct apple80211_ssid_data* sd);
    IOReturn setSSID(IO80211Interface* interface, struct apple80211_ssid_data* sd);
    // 2 - AUTH_TYPE
    IOReturn getAUTH_TYPE(IO80211Interface* interface, struct apple80211_authtype_data* ad);
    IOReturn setAUTH_TYPE(IO80211Interface* interface, struct apple80211_authtype_data* ad);
	// 3 - CIPHER_KEY
    IOReturn getCIPHER_KEY(IO80211Interface* interface, struct apple80211_key* key);
    IOReturn setCIPHER_KEY(IO80211Interface* interface, struct apple80211_key* key);
    // 4 - CHANNEL
    IOReturn getCHANNEL(IO80211Interface* interface, struct apple80211_channel_data* cd);
	// 6 - PROTMODE
	IOReturn getPROTMODE(IO80211Interface* interface, struct apple80211_protmode_data* pd);
    // 7 - TXPOWER
    IOReturn getTXPOWER(IO80211Interface* interface, struct apple80211_txpower_data* txd);
    // 8 - RATE
    IOReturn getRATE(IO80211Interface* interface, struct apple80211_rate_data* rd);
    // 9 - BSSID
    IOReturn getBSSID(IO80211Interface* interface, struct apple80211_bssid_data* bd);
    // 10 - SCAN_REQ
    IOReturn setSCAN_REQ(IO80211Interface* interface, struct apple80211_scan_data* sd);
	IOReturn setSCAN_REQ_MULTIPLE(IO80211Interface* interface, struct apple80211_scan_multiple_data* sd);
    // 11 - SCAN_RESULT
    IOReturn getSCAN_RESULT(IO80211Interface* interface, apple80211_scan_result* *sr);
    // 12 - CARD_CAPABILITIES
    IOReturn getCARD_CAPABILITIES(IO80211Interface* interface, struct apple80211_capability_data* cd);
    // 13 - STATE
    IOReturn getSTATE(IO80211Interface* interface, struct apple80211_state_data* sd);
    IOReturn setSTATE(IO80211Interface* interface, struct apple80211_state_data* sd);
    // 14 - PHY_MODE
    IOReturn getPHY_MODE(IO80211Interface* interface, struct apple80211_phymode_data* pd);
    // 15 - OP_MODE
    IOReturn getOP_MODE(IO80211Interface* interface, struct apple80211_opmode_data* od);
    // 16 - RSSI
    IOReturn getRSSI(IO80211Interface* interface, struct apple80211_rssi_data* rd);
    // 17 - NOISE
    IOReturn getNOISE(IO80211Interface* interface,struct apple80211_noise_data* nd);
    // 18 - INT_MIT
    IOReturn getINT_MIT(IO80211Interface* interface, struct apple80211_intmit_data* imd);
    // 19 - POWER
    IOReturn getPOWER(IO80211Interface* interface, struct apple80211_power_data* pd);
    IOReturn setPOWER(IO80211Interface* interface, struct apple80211_power_data* pd);
    // 20 - ASSOCIATE
    IOReturn setASSOCIATE(IO80211Interface* interface, struct apple80211_assoc_data* ad);
    // 22 - DISASSOCIATE
    IOReturn setDISASSOCIATE(IO80211Interface* interface);
    // 27 - SUPPORTED_CHANNELS
    IOReturn getSUPPORTED_CHANNELS(IO80211Interface* interface, struct apple80211_sup_channel_data* ad);
    // 28 - LOCALE
    IOReturn getLOCALE(IO80211Interface* interface, struct apple80211_locale_data* ld);
	// 29 - DEAUTH
	IOReturn getDEAUTH(IO80211Interface* interface, struct apple80211_deauth_data* dd);
    // 37 - TX_ANTENNA
    IOReturn getTX_ANTENNA(IO80211Interface* interface, apple80211_antenna_data* ad);
    // 39 - ANTENNA_DIVERSITY
    IOReturn getANTENNA_DIVERSITY(IO80211Interface* interface, apple80211_antenna_data* ad);
    // 43 - DRIVER_VERSION
    IOReturn getDRIVER_VERSION(IO80211Interface* interface, struct apple80211_version_data* hv);
    // 44 - HARDWARE_VERSION
    IOReturn getHARDWARE_VERSION(IO80211Interface* interface, struct apple80211_version_data* hv);
	// 46 - RSN_IE
	IOReturn getRSN_IE(IO80211Interface *interface, struct apple80211_rsn_ie_data *rsn_ie_data);
	IOReturn setRSN_IE(IO80211Interface *interface, struct apple80211_rsn_ie_data *rsn_ie_data);
	// 48 - AP_IE_LIST
	IOReturn getAP_IE_LIST(IO80211Interface *interface, struct apple80211_ap_ie_data *ap_ie_data);	
	// 50 - ASSOCIATION_STATUS
	IOReturn getASSOCIATION_STATUS(IO80211Interface *interface, struct apple80211_assoc_status_data *sd);
    // 51 - COUNTRY_CODE
    IOReturn getCOUNTRY_CODE(IO80211Interface* interface, struct apple80211_country_code_data* cd);
    // 57 - MCS
    IOReturn getMCS(IO80211Interface* interface, struct apple80211_mcs_data* md);
    IOReturn getROAM_THRESH(IO80211Interface* interface, struct apple80211_roam_threshold_data* md);
    IOReturn getRADIO_INFO(IO80211Interface* interface, struct apple80211_radio_info_data* md);
	// 90 - SCANCACHE_CLEAR
	IOReturn setSCANCACHE_CLEAR(IO80211Interface* interface);
	// 156 - LINK_CHANGED_EVENT_DATA
	IOReturn getLINK_CHANGED_EVENT_DATA(IO80211Interface* interface, struct apple80211_link_changed_event_data* ed);
	// 254 - HW_SUPPORTED_CHANNELS
	IOReturn getHW_SUPPORTED_CHANNELS(IO80211Interface *interface, struct apple80211_sup_channel_data *cd);
	// 353 - NSS
	IOReturn getNSS(IO80211Interface *interface, struct apple80211_nss_data *nd);
    
    
    inline void ReleaseAll() {
        OSSafeReleaseNULL(fCommandGate);
        OSSafeReleaseNULL(fWorkloop);
        OSSafeReleaseNULL(mediumDict);
        OSSafeReleaseNULL(fWorkloop);
		OSSafeReleaseNULL(fTimerEventSource);
    }
    
    bool addMediumType(UInt32 type, UInt32 speed, UInt32 code, char* name = 0);
	static void postScanningDoneMessage(OSObject* self, ...);
public:
	ScanResult* scan_result;
    
    IOWorkLoop* fWorkloop;
    IO80211Interface* fInterface;
    IOCommandGate* fCommandGate;
	Black80211Device* fProvider;
	IOTimerEventSource* fTimerEventSource;
    
	size_t networkIndex = 0;

    OSDictionary* mediumDict;
    IONetworkMedium* mediumTable[MEDIUM_TYPE_INVALID];

	bool requestedScanning;
	bool requestIsMulti;
	struct apple80211_scan_data request;
	struct apple80211_scan_multiple_data multiRequest;

	uint32_t authtype_lower;
	uint32_t authtype_upper;

	uint32_t powerState;

	apple80211_key cipher_key;
};

#endif
