//
//  interop.hpp
//  itlwm
//
//  Created by usrsse2 on 30.07.2020.
//  Copyright © 2020 钟先耀. All rights reserved.
//

#ifndef interop_hpp
#define interop_hpp

#include <IOKit/IOService.h>

struct NetworkInformation {
	u_int8_t		essid[32];
	u_int8_t		bssid[6];
	u_int8_t		rssi;
	u_int16_t		capabilities;
	u_int16_t		beacon_interval;
	u_int32_t		timestamp;
	u_int8_t		*rsn_ie;
	int 			channel;
};


class ScanResult : public OSObject {
	OSDeclareDefaultStructors(ScanResult)
	
public:
	virtual bool init() override;
	virtual void free() override;

	static ScanResult* scanResult();
	
	size_t count;
	NetworkInformation *networks;
};


#define APPLE80211_MAX_CHANNELS        64

struct channel_desc {
	uint8_t channel_num;
	uint32_t channel_flags;
};

class Black80211Device : public IOService {
	OSDeclareDefaultStructors(Black80211Device)
	
public:
	virtual IOReturn getMACAddress(IOEthernetAddress* address) = 0;
	virtual void setController(IOEthernetController* io80211controller) = 0;
	virtual void setInterface(IOEthernetInterface* interface) = 0;
	virtual void enable() = 0;
	virtual void disable() = 0;
	virtual ScanResult* getScanResult() = 0;
	virtual void disassociate() = 0;
	virtual IOReturn bgscan(uint8_t* channels, uint32_t length, const char* ssid, uint32_t ssid_len) = 0;
	virtual void getESSID(uint8_t essid[32], uint32_t* len) = 0;
	virtual void getBSSID(u_int8_t bssid[6]) = 0;
	virtual int getChannel() = 0;
	virtual int getRate() = 0;
	virtual int getMCS() = 0;
	virtual int getRSSI() = 0;
	virtual int getNoise() = 0;
	virtual int getState() = 0;
	virtual bool isScanning() = 0;
	virtual void getRSNIE(uint16_t &ie_len, uint8_t ie_buf[257]) = 0;
	virtual void getSupportedChannels(uint32_t &channels_count, struct channel_desc channel_desc[APPLE80211_MAX_CHANNELS]) = 0;
	virtual UInt32 outputPacket(mbuf_t m, void *param) = 0;
	virtual IOCommandGate *getCommandGate() const = 0;
	virtual const OSString * newVendorString() const = 0;
    virtual const OSString * newModelString() const = 0;
	virtual void getFirmwareVersion(char version[256], uint16_t &version_len) = 0;
	virtual uint32_t getPHYMode() = 0;
	virtual uint32_t getSupportedPHYModes() = 0;
	virtual uint32_t getOpMode() = 0;
	virtual void getCountryCode(char countryCode[3]) = 0;
	virtual void getAP_IE_LIST(uint32_t &ie_list_len, uint8_t *ie_buf) = 0;
	virtual void setPTK(const u_int8_t *key, size_t key_len) = 0;
	virtual void setGTK(const u_int8_t *key, size_t key_len, u_int8_t kid, u_int8_t *rsc) = 0;
	virtual void associate(uint8_t *ssid, uint32_t ssid_len, uint32_t authtype_lower, uint32_t authtype_upper, uint8_t *key, uint32_t key_len, int key_index) = 0;
};

#endif /* interop_hpp */
