//
//  Black80211Control_ioctl.cpp
//  Black80211
//
//  Created by Roman Peshkov on 30/06/2018.
//  Copyright © 2018 Roman Peshkov. All rights reserved.
//

#include "Black80211Control.hpp"
#include "ieee80211_ioctl.h"

const char *fake_hw_version = "Hardware 1.0";
const char *fake_drv_version = "Driver 1.0";
const char *fake_country_code = "RU";

#define CHANNELS

static apple80211_channel supported_channels[] = {
	{
		.version = APPLE80211_VERSION,
		.channel = 1,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
#ifdef CHANNELS
	{
		.version = APPLE80211_VERSION,
		.channel = 2,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 3,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 4,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 5,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 6,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 7,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 8,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 9,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 10,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 11,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 12,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 13,
		.flags = APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 36,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 40,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 44,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 48,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 52,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 56,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 60,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 64,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 100,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 104,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 108,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 112,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 116,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 120,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 124,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 128,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 132,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 136,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 140,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 149,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 153,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 157,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 161,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	},
	{
		.version = APPLE80211_VERSION,
		.channel = 165,
		.flags = APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE
	}
#endif
};

extern void itlwm_enable(IONetworkController *itlwm);
extern void itlwm_disable(IONetworkController *itlwm);
extern interop_scan_result* itlwm_get_scan_result(IONetworkController *itlwm);
extern void itlwm_associate(IONetworkController *self, const u_int8_t* ssid, const u_int8_t *key, u_int32_t key_len);
extern void itlwm_disassociate(IONetworkController *self);
extern void itlwm_bgscan(IONetworkController *self, void* target, void (* callback)(void*), uint8_t* channels, uint32_t length);
extern void itlwm_get_essid(IONetworkController *self, struct ieee80211_nwid& nwid);
extern void itlwm_get_bssid(IONetworkController *self, u_int8_t * bssid);
extern int itlwm_get_channel(IONetworkController *self);
extern int itlwm_get_rate(IONetworkController *self);
extern int itlwm_get_rssi(IONetworkController *self);
extern int itlwm_get_noise(IONetworkController *self);
extern int itlwm_get_state(IONetworkController *self);
extern bool itlwm_is_scanning(IONetworkController *self);


//
// MARK: 1 - SSID
//

IOReturn Black80211Control::getSSID(IO80211Interface *interface,
                                    struct apple80211_ssid_data *sd) {
    
    bzero(sd, sizeof(*sd));
    sd->version = APPLE80211_VERSION;
	if (itlwm_get_state(fItlWm) != APPLE80211_S_RUN)
		return 6;

	struct ieee80211_nwid nwid;
	itlwm_get_essid(fItlWm, nwid);
	sd->ssid_len = nwid.i_len;
	strncpy((char*)sd->ssid_bytes, (const char*)nwid.i_nwid, APPLE80211_MAX_SSID_LEN);

    return kIOReturnSuccess;
}

IOReturn Black80211Control::setSSID(IO80211Interface *interface,
                                    struct apple80211_ssid_data *sd) {
    
    fInterface->postMessage(APPLE80211_M_SSID_CHANGED);
    return kIOReturnSuccess;
}

//
// MARK: 2 - AUTH_TYPE
//

IOReturn Black80211Control::getAUTH_TYPE(IO80211Interface *interface,
                                         struct apple80211_authtype_data *ad) {
    ad->version = APPLE80211_VERSION;
	ad->authtype_lower = authtype_lower;
	ad->authtype_upper = authtype_upper;
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setAUTH_TYPE(IO80211Interface *interface,
                                         struct apple80211_authtype_data *ad) {
	authtype_lower = ad->authtype_lower;
	authtype_upper = ad->authtype_upper;
    return kIOReturnSuccess;
}


static u_int32_t channelFlags(int channel) {
	if (channel <= 14)
		return APPLE80211_C_FLAG_2GHZ | APPLE80211_C_FLAG_20MHZ | APPLE80211_C_FLAG_ACTIVE;
	else if (channel >= 52 && channel <= 140)
		return APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE | APPLE80211_C_FLAG_DFS;
	else
		return APPLE80211_C_FLAG_5GHZ | APPLE80211_C_FLAG_40MHZ | APPLE80211_C_FLAG_ACTIVE;
}

//
// MARK: 3 - CIPHER_KEY
//

IOReturn Black80211Control::getCIPHER_KEY(IO80211Interface *interface,
                                         struct apple80211_key *key) {
	*key = cipher_key;
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setCIPHER_KEY(IO80211Interface *interface,
                                         struct apple80211_key *key) {
	cipher_key = *key;
	//fInterface->postMessage(APPLE80211_M_CIPHER_KEY_CHANGED);
    return kIOReturnSuccess;
}


//
// MARK: 4 - CHANNEL
//

IOReturn Black80211Control::getCHANNEL(IO80211Interface *interface,
                                       struct apple80211_channel_data *cd) {
    memset(cd, 0, sizeof(apple80211_channel_data));
	if (itlwm_get_state(fItlWm) != APPLE80211_S_RUN)
		return 6;

	cd->version = APPLE80211_VERSION;
    cd->channel.version = APPLE80211_VERSION;
#ifdef CHANNELS
	cd->channel.channel = itlwm_get_channel(fItlWm);
#else
	cd->channel.channel = 1;
#endif
	cd->channel.flags = channelFlags(cd->channel.channel);
    return kIOReturnSuccess;
}

//
// MARK: 6 - PROTMODE
//

IOReturn Black80211Control::getPROTMODE(IO80211Interface *interface, struct apple80211_protmode_data *pd) {
	pd->version = APPLE80211_VERSION;
	pd->protmode = APPLE80211_PROTMODE_AUTO;
	pd->threshold = 0;
	return kIOReturnSuccess;
}


//
// MARK: 7 - TXPOWER
//

IOReturn Black80211Control::getTXPOWER(IO80211Interface *interface,
                                       struct apple80211_txpower_data *txd) {

    txd->version = APPLE80211_VERSION;
    txd->txpower = 100;
    txd->txpower_unit = APPLE80211_UNIT_PERCENT;
    return kIOReturnSuccess;
}

//
// MARK: 8 - RATE
//

IOReturn Black80211Control::getRATE(IO80211Interface *interface, struct apple80211_rate_data *rd) {
	if (itlwm_get_state(fItlWm) != APPLE80211_S_RUN)
		return 6;

    rd->version = APPLE80211_VERSION;
    rd->num_radios = 1;
    rd->rate[0] = itlwm_get_rate(fItlWm);
    return kIOReturnSuccess;
}

//
// MARK: 9 - BSSID
//

IOReturn Black80211Control::getBSSID(IO80211Interface *interface,
                                     struct apple80211_bssid_data *bd) {
    
    bd->version = APPLE80211_VERSION;
	if (itlwm_get_state(fItlWm) != APPLE80211_S_RUN)
		return 6;

	itlwm_get_bssid(fItlWm, bd->bssid.octet);
    return kIOReturnSuccess;
}

static void scanCallback(void *target) {
	Black80211Control *controller = (Black80211Control*)target;
	controller->requestedScanning = false;
	IO80211Interface *iface = controller->fInterface;
	controller->networkIndex = 0;
	controller->scan_result = itlwm_get_scan_result(controller->fItlWm);
	IOLog("Black80211: Scanning complete, found %lu networks\n", controller->scan_result->count);
	iface->postMessage(APPLE80211_M_SCAN_DONE);
}

static IOReturn scanAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) {
    //IO80211Interface *iface = (IO80211Interface *)arg0;
    Black80211Control *controller = (Black80211Control*)arg1;
	//IOLog("Reporting previous scan result\n");
	//controller->networkIndex = 0;
	//controller->scan_result = itlwm_get_scan_result(controller->fItlWm);
	//iface->postMessage(APPLE80211_M_SCAN_DONE);

	//if (!controller->requestedScanning || !itlwm_is_scanning(controller->fItlWm)) {
		IOLog("Begin scanning\n");
		controller->requestedScanning = true;
		itlwm_bgscan(controller->fItlWm, controller, scanCallback, (uint8_t*)arg2, *(uint32_t*)arg3);

		//for (int i = 0; i < 30 && controller->requestedScanning && itlwm_is_scanning(controller->fItlWm); i++) {
		//	IOLog("Still not done\n");
		//	IOSleep(1000);
		//}
		//IOLog("Success\n");
	//}

    return kIOReturnSuccess;
}

//
// MARK: 10 - SCAN_REQ
//
IOReturn Black80211Control::setSCAN_REQ(IO80211Interface *interface,
                                        struct apple80211_scan_data *sd) {
	if (requestedScanning && itlwm_is_scanning(fItlWm)) {
		IOLog("Error: already scanning\n");
		return iokit_family_err(sub_iokit_wlan, 0x446);
		/*IOLog("Reporting previous scan result\n");
		networkIndex = 0;
		scan_result = itlwm_get_scan_result(fItlWm);
		interface->postMessage(APPLE80211_M_SCAN_DONE);
		return kIOReturnSuccess;*/
	}

	requestedScanning = true;

	requestIsMulti = false;
	memcpy(&request, sd, sizeof(struct apple80211_scan_data));

	networkIndex = 0;

	IOLog("Black80211. Scan requested. Type: %u\n"
		  "BSS Type: %u\n"
		  "PHY Mode: %u\n"
		  "Dwell time: %u\n"
		  "Rest time: %u\n"
		  "Num channels: %u\n",
		  sd->scan_type,
		  sd->bss_type,
		  sd->phy_mode,
		  sd->dwell_time,
		  sd->rest_time,
		  sd->num_channels);

	IOLog("SSID: %s\n", sd->ssid);

	uint8_t *b = sd->bssid.octet;
	IOLog("BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n", b[0], b[1], b[2], b[3], b[4], b[5]);

	IOLog("Channels:\n");
	uint8_t channels[255];
	for (int i = 0; i < sd->num_channels; i++) {
		IOLog("%d\n", sd->channels[i].channel);
		channels[i] = sd->channels[i].channel;
	}

	if (sd->scan_type == APPLE80211_SCAN_TYPE_FAST) {
	  IOLog("Reporting previous scan result\n");
	  networkIndex = 0;
	  scan_result = itlwm_get_scan_result(fItlWm);
	  interface->postMessage(APPLE80211_M_SCAN_DONE);
	  return kIOReturnSuccess;
	}

	if (interface)
		fCommandGate->runAction(scanAction, interface, this, channels, &sd->num_channels);

	return kIOReturnSuccess;
}

IOReturn Black80211Control::setSCAN_REQ_MULTIPLE(
IO80211Interface *interface, struct apple80211_scan_multiple_data *sd) {
	return kIOReturnUnsupported;
	if (requestedScanning && itlwm_is_scanning(fItlWm)) {
		IOLog("Error: already scanning\n");
	/*	return iokit_family_err(sub_iokit_wlan, 0x446);
		IOLog("Reporting previous scan result\n");
		networkIndex = 0;
		scan_result = itlwm_get_scan_result(fItlWm);
		interface->postMessage(APPLE80211_M_SCAN_DONE);
		return kIOReturnSuccess;*/
	}

	requestIsMulti = true;
	memcpy(&multiRequest, sd, sizeof(struct apple80211_scan_multiple_data));

	networkIndex = 0;

	IOLog("Black80211. Multiple scan requested. Type: %u\n"
		"PHY Mode: %u\n"
		"Dwell time: %u\n"
		"Rest time: %u\n",
		  sd->scan_type,
		  sd->phy_mode,
		  sd->dwell_time,
		  sd->rest_time);
	IOLog("SSIDs:\n");
	for (int i = 0; i < sd->ssid_count; i++) // FIXME: may be not null-terminated
		IOLog("%s\n", sd->ssids[i].ssid_bytes);

	IOLog("BSSIDs:\n");
	for (int i = 0; i < sd->bssid_count; i++) {
		uint8_t *b = sd->bssids[i].octet;
		IOLog("%02x:%02x:%02x:%02x:%02x:%02x\n", b[0], b[1], b[2], b[3], b[4], b[5]);
	}

	IOLog("Channels:\n");
	uint8_t channels[255];
	for (int i = 0; i < sd->num_channels; i++) {
		IOLog("%d\n", sd->channels[i].channel);
		channels[i] = sd->channels[i].channel;
	}


	if (sd->scan_type == APPLE80211_SCAN_TYPE_FAST/* || (sd->num_channels > 0 && sd->channels[0].channel != 1)*/) {
	  IOLog("Reporting previous scan result\n");
	  networkIndex = 0;
	  scan_result = itlwm_get_scan_result(fItlWm);
	  interface->postMessage(APPLE80211_M_SCAN_DONE);
	  return kIOReturnSuccess;
	}

	if (interface)
		fCommandGate->runAction(scanAction, interface, this, channels, &sd->num_channels);

	return kIOReturnSuccess;
}


//
// MARK: 11 - SCAN_RESULT
//

/*
undefined8 __thiscall
getSCAN_RESULT(AirPort_BrcmNIC *this,OSObject *param_1,apple80211_scan_result **param_2)

{
  undefined8 uVar1;

  if (*(int *)(this + 0x19f8) == 0) {
    uVar1 = 0x10;
  }
  else {
    if (*(int *)(this + 0x19f8) < 0) {
      uVar1 = 0xc;
    }
    else {
      if (param_2 == (apple80211_scan_result **)0x0) {
        uVar1 = 0x16;
      }
      else {
        if (*(long *)(this + 0x1a10) == 0) {
          uVar1 = 5;
        }
        else {
          *param_2 = (apple80211_scan_result *)(*(long *)(this + 0x1a10) + 8);
          *(undefined8 *)(this + 0x1a10) = **(undefined8 **)(this + 0x1a10);
          uVar1 = 0;
        }
      }
    }
  }
  return uVar1;
}
 */


IOReturn Black80211Control::getSCAN_RESULT(IO80211Interface *interface,
                                           struct apple80211_scan_result **sr) {
	requestedScanning = false;

	if (sr == nullptr) {
		IOLog("Error: out var is null\n");
		return 0x16;
	}

	if (scan_result == nullptr) {
		IOLog("Error: scan result is not ready\n");
		return 0x10;
	}

	if (scan_result->count == 0) {
		IOLog("Error: no networks found\n");
		return 0x10;
	}

	//if (prevResult != nullptr) {
	//	if (prevResult->asr_ie_data != nullptr)
	//		IOFree(prevResult->asr_ie_data, prevResult->asr_ie_len);
	//	IOFree(prevResult, sizeof(struct apple80211_scan_result));
	//	prevResult = nullptr;
	//}

	for ( ; networkIndex < scan_result->count; networkIndex++) {
		interop_scan_result_network *network = &scan_result->networks[networkIndex];

		/*
		if (network->ni_rssi < 25) // disable weak networks
			continue;

		if (network->ni_essid[0] == 0) // disable hidden networks
			continue;
		 */

		bool requested = true;
#ifdef CHANNELS
		// itlwm reports current network even when its channel is not requested
		if (requestIsMulti && multiRequest.num_channels > 0) {
			requested = false;
			for (int i = 0; i < multiRequest.num_channels; i++)
				if (multiRequest.channels[i].channel == network->ni_channel) {
					requested = true;
					break;
				}
		}
		else if (!requestIsMulti && request.num_channels > 0) {
			requested = false;
			for (int i = 0; i < request.num_channels; i++)
				if (request.channels[i].channel == network->ni_channel) {
					requested = true;
					break;
				}
		}

		if (!requested)
			continue;
#endif

		if (requestIsMulti && multiRequest.ssid_count > 0) {
			requested = false;
			for (int i = 0; i < multiRequest.ssid_count; i++) {
				size_t ssid_len = strnlen((const char*)network->ni_essid, 32);
				if (ssid_len != multiRequest.ssids[i].ssid_len)
					continue;

				if (!strncmp((const char*)multiRequest.ssids[i].ssid_bytes, (const char*)network->ni_essid, ssid_len)) {
					requested = true;
					break;
				}
			}
		}
		else if (!requestIsMulti && request.ssid_len > 0) {
			requested = strnlen((const char*)network->ni_essid, 32) == request.ssid_len && !strncmp((const char*)network->ni_essid, (const char*)request.ssid, 32);
		}

		if (!requested)
			continue;

		struct apple80211_scan_result* result =
			(struct apple80211_scan_result*)IOMalloc(sizeof(struct apple80211_scan_result));

		bzero(result, sizeof(*result));

		result->version = APPLE80211_VERSION;

		result->asr_channel.version = APPLE80211_VERSION;
#ifdef CHANNELS
		result->asr_channel.channel = network->ni_channel;
#else
		result->asr_channel.channel = 1;
#endif
		result->asr_channel.flags = channelFlags(result->asr_channel.channel);
		result->asr_noise = itlwm_get_noise(fItlWm);
		result->asr_rssi = network->ni_rssi;
		result->asr_beacon_int = network->ni_intval;

		result->asr_cap = network->ni_capinfo;

		result->asr_age = 0;

		memcpy(result->asr_bssid, network->ni_bssid, APPLE80211_ADDR_LEN);

		result->asr_nrates = 1;
		result->asr_rates[0] = 54;

		IOLog("SSID: %s, channel %d\n", network->ni_essid, network->ni_channel);
		strncpy((char*)result->asr_ssid, (const char*)network->ni_essid, APPLE80211_MAX_SSID_LEN);
		result->asr_ssid_len = strnlen((const char*)network->ni_essid, APPLE80211_MAX_SSID_LEN);

		if (network->ni_rsnie == nullptr) {
			result->asr_ie_len = 0;
			result->asr_ie_data = nullptr;
		}
		else {
			result->asr_ie_len = 2 + network->ni_rsnie[1];
			result->asr_ie_data = IOMalloc(result->asr_ie_len);
			memcpy(result->asr_ie_data, network->ni_rsnie, result->asr_ie_len);
		}

		*sr = result;
		prevResult = result;

		networkIndex++;
		return kIOReturnSuccess;
	}

	IOLog("Reported all results\n");
	return 5;
}

//
// MARK: 12 - CARD_CAPABILITIES
//
IOReturn Black80211Control::getCARD_CAPABILITIES(
    IO80211Interface *interface, struct apple80211_capability_data *cd) {
	bzero(cd, sizeof(struct apple80211_capability_data));
	cd->version = APPLE80211_VERSION;

	int capabilities[] = {
		APPLE80211_CAP_WEP,
		APPLE80211_CAP_TKIP,
		APPLE80211_CAP_AES_CCM,
		//APPLE80211_CAP_IBSS,
		//APPLE80211_CAP_PMGT,
		//APPLE80211_CAP_HOSTAP,
		APPLE80211_CAP_SHSLOT,
		APPLE80211_CAP_SHPREAMBLE,
		//APPLE80211_CAP_MONITOR,
		APPLE80211_CAP_TKIPMIC,
		APPLE80211_CAP_WPA1,
		APPLE80211_CAP_WPA2
	};

	uint64_t capabilities_value = 0;
	for (int i = 0; i < sizeof(capabilities); i++)
		capabilities_value |= (1 << capabilities[i]);

	cd->capabilities[0] = capabilities_value & 0xff;
	cd->capabilities[1] = (capabilities_value >> 8) & 0xff;

	// Some bits of these two bytes enable multiple scanning requests. It's not what we currently want
	//cd->capabilities[2] |= 0xC0;
	//cd->capabilities[6] |= 0x84;
	//memset(cd->capabilities, 0xff, 8);

  /// ic->ic_caps =
  /// IEEE80211_C_WEP |        /* WEP */
  /// IEEE80211_C_RSN |        /* WPA/RSN */
  /// IEEE80211_C_SCANALL |    /* device scans all channels at once */
  /// IEEE80211_C_SCANALLBAND |    /* device scans all bands at once */
  /// IEEE80211_C_SHSLOT |    /* short slot time supported */
  /// IEEE80211_C_SHPREAMBLE;    /* short preamble supported */

  //cd->capabilities[0] = 0xeb;
  //cd->capabilities[1] = 0x7e;

  // cd->capabilities[5] |= 4;

	// cd->capabilities[2] |= 0xC0;
	// cd->capabilities[6] |= 0x84;

  /* Airport-related flags
  cd->capabilities[3] |= 8;
  cd->capabilities[6] |= 1;
  cd->capabilities[5] |= 0x80;
  */

  /*
   Needs further documentation,
   as setting all of these flags enables AirDrop + IO80211VirtualInterface

  cd->capabilities[2] = 3;
  cd->capabilities[2] |= 0x13;
  cd->capabilities[2] |= 0x20;
  cd->capabilities[2] |= 0x28;
  cd->capabilities[2] |= 4;
  cd->capabilities[5] |= 8;
  cd->capabilities[3] |= 2;
  cd->capabilities[4] |= 1;
  cd->capabilities[6] |= 8;

  cd->capabilities[3] |= 0x23;
  cd->capabilities[2] |= 0x80;
  cd->capabilities[5] |= 4;
  cd->capabilities[2] |= 0xC0;
  cd->capabilities[6] |= 0x84;
  cd->capabilities[3] |= 8;
  cd->capabilities[6] |= 1;
  cd->capabilities[5] |= 0x80;

  */

  /*
  cd->capabilities[0] = (caps & 0xff);
  cd->capabilities[1] = (caps & 0xff00) >> 8;
  cd->capabilities[2] = (caps & 0xff0000) >> 16;
  */

  return kIOReturnSuccess;
}

//
// MARK: 13 - STATE
//

IOReturn Black80211Control::getSTATE(IO80211Interface *interface,
                                     struct apple80211_state_data *sd) {
    sd->version = APPLE80211_VERSION;
    sd->state = itlwm_get_state(fItlWm);
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setSTATE(IO80211Interface *interface,
                                     struct apple80211_state_data *sd) {
    //IOLog("Black80211: Setting state: %u\n", sd->state);
    //state = sd->state;
    return kIOReturnSuccess;
}

//
// MARK: 14 - PHY_MODE
//

IOReturn Black80211Control::getPHY_MODE(IO80211Interface *interface,
                                        struct apple80211_phymode_data *pd) {
    pd->version = APPLE80211_VERSION;
    pd->phy_mode = APPLE80211_MODE_11A
                 | APPLE80211_MODE_11B
                 | APPLE80211_MODE_11G
				 | APPLE80211_MODE_11N;
    pd->active_phy_mode = APPLE80211_MODE_11N;
    return kIOReturnSuccess;
}

//
// MARK: 15 - OP_MODE
//

IOReturn Black80211Control::getOP_MODE(IO80211Interface *interface,
                                       struct apple80211_opmode_data *od) {
    od->version = APPLE80211_VERSION;
    od->op_mode = APPLE80211_M_STA;
    return kIOReturnSuccess;
}

//
// MARK: 16 - RSSI
//

IOReturn Black80211Control::getRSSI(IO80211Interface *interface,
                                    struct apple80211_rssi_data *rd) {
    // iwm_get_signal_strength
	// iwm_calc_rssi
	// iwm_rxmq_get_signal_strength
    bzero(rd, sizeof(*rd));
    rd->version = APPLE80211_VERSION;
    rd->num_radios = 1;
    rd->rssi[0] = itlwm_get_rssi(fItlWm);
    rd->aggregate_rssi = rd->rssi[0];
    rd->rssi_unit = APPLE80211_UNIT_DBM;
    return kIOReturnSuccess;
}

//
// MARK: 17 - NOISE
//

IOReturn Black80211Control::getNOISE(IO80211Interface *interface,
                                     struct apple80211_noise_data *nd) {
    // iwm_get_noise
    bzero(nd, sizeof(*nd));
    nd->version = APPLE80211_VERSION;
    nd->num_radios = 1;
    nd->noise[0] = itlwm_get_noise(fItlWm);
    nd->aggregate_noise = nd->noise[0];
    nd->noise_unit = APPLE80211_UNIT_DBM;
    return kIOReturnSuccess;
}

//
// MARK: 18 - INT_MIT
//
IOReturn Black80211Control::getINT_MIT(IO80211Interface* interface,
                                       struct apple80211_intmit_data* imd) {
    imd->version = APPLE80211_VERSION;
    imd->int_mit = APPLE80211_INT_MIT_AUTO;
    return kIOReturnSuccess;
}


//
// MARK: 19 - POWER
//

IOReturn Black80211Control::getPOWER(IO80211Interface *interface,
                                     struct apple80211_power_data *pd) {
    pd->version = APPLE80211_VERSION;
    pd->num_radios = 1;
    pd->power_state[0] = powerState;
    
    return kIOReturnSuccess;
}


IOReturn Black80211Control::setPOWER(IO80211Interface *interface,
                                     struct apple80211_power_data *pd) {
	requestedScanning = false;
    if (pd->num_radios > 0) {
		if (pd->power_state[0]) {
			itlwm_enable(fItlWm);
		}
		else {
			itlwm_disable(fItlWm);
		}

        powerState = pd->power_state[0];
    }
    //fInterface->postMessage(APPLE80211_M_POWER_CHANGED, NULL, 0);
    
    return kIOReturnSuccess;
}

//
// MARK: 20 - ASSOCIATE
//

IOReturn Black80211Control::setASSOCIATE(IO80211Interface *interface,
                                         struct apple80211_assoc_data *ad) {
    IOLog("Black80211::setAssociate %s\n", ad->ad_ssid);

	setDISASSOCIATE(interface);

	apple80211_authtype_data authtype_data;
	authtype_data.version = APPLE80211_VERSION;
	authtype_data.authtype_lower = ad->ad_auth_lower;
	authtype_data.authtype_upper = ad->ad_auth_upper;
	setAUTH_TYPE(interface, &authtype_data);

	apple80211_rsn_ie_data rsn_ie_data;
	rsn_ie_data.version = APPLE80211_VERSION;
	rsn_ie_data.len = ad->ad_rsn_ie_len;
	memcpy(rsn_ie_data.ie, ad->ad_rsn_ie, rsn_ie_data.len);
	setRSN_IE(interface, &rsn_ie_data);

	setCIPHER_KEY(interface, &ad->ad_key);

	itlwm_associate(fItlWm, ad->ad_ssid, ad->ad_key.key, ad->ad_key.key_len);
	IOSleep(5000);
    fInterface->setLinkState(IO80211LinkState::kIO80211NetworkLinkUp, 0);



	// setAP_MODE(interface, ap_mode);

	apple80211_ssid_data ssid_data;
	ssid_data.version = APPLE80211_VERSION;
	ssid_data.ssid_len = ad->ad_ssid_len;
	memcpy(ssid_data.ssid_bytes, ad->ad_ssid, APPLE80211_MAX_SSID_LEN);
	setSSID(interface, &ssid_data);

	// don't set BSSID – it is set by the driver

	return kIOReturnSuccess;
}

//
// MARK: 22 - DISASSOCIATE
//

IOReturn Black80211Control::setDISASSOCIATE(IO80211Interface *interface) {
    IOLog("Black80211::disassociate\n");
	requestedScanning = false;
	itlwm_disassociate(fItlWm);
	fInterface->setLinkState(IO80211LinkState::kIO80211NetworkLinkDown, 0);
	return kIOReturnSuccess;
}


//
// MARK: 27 - SUPPORTED_CHANNELS
//

IOReturn Black80211Control::getSUPPORTED_CHANNELS(IO80211Interface *interface,
                                                  struct apple80211_sup_channel_data *ad) {
    ad->version = APPLE80211_VERSION;
    ad->num_channels = sizeof(supported_channels) / sizeof(struct apple80211_channel);
	memcpy(ad->supported_channels, supported_channels, sizeof(supported_channels));
    return kIOReturnSuccess;
}

//
// MARK: 28 - LOCALE
//

IOReturn Black80211Control::getLOCALE(IO80211Interface *interface,
                                      struct apple80211_locale_data *ld) {
    ld->version = APPLE80211_VERSION;
    ld->locale  = APPLE80211_LOCALE_FCC;
    
    return kIOReturnSuccess;
}

//
// MARK: 29 - DEAUTH
//

IOReturn Black80211Control::getDEAUTH(IO80211Interface *interface, struct apple80211_deauth_data *dd) {
	dd->version = APPLE80211_VERSION;
	int state = itlwm_get_state(fItlWm);
	//if (state == APPLE80211_S_RUN)
	//	dd->deauth_reason = APPLE80211_RESULT_SUCCESS;
	//else
	//	dd->deauth_reason = APPLE80211_RESULT_UNAVAILABLE;
	dd->deauth_reason = 0;
	itlwm_get_bssid(fItlWm, dd->deauth_ea.octet);
	IOLog("Deauth reason: %d, state: %d\n", dd->deauth_reason, state);
	return kIOReturnSuccess;
}

//
// MARK: 37 - TX_ANTENNA
//
IOReturn Black80211Control::getTX_ANTENNA(IO80211Interface *interface,
                                          apple80211_antenna_data *ad) {
    ad->version = APPLE80211_VERSION;
    ad->num_radios = 1;
    ad->antenna_index[0] = 1;
    return kIOReturnSuccess;
}

//
// MARK: 39 - ANTENNA_DIVERSITY
//

IOReturn Black80211Control::getANTENNA_DIVERSITY(IO80211Interface *interface,
                                                 apple80211_antenna_data *ad) {
    ad->version = APPLE80211_VERSION;
    ad->num_radios = 1;
    ad->antenna_index[0] = 1;
    return kIOReturnSuccess;
}

//
// MARK: 43 - DRIVER_VERSION
//

IOReturn Black80211Control::getDRIVER_VERSION(IO80211Interface *interface,
                                              struct apple80211_version_data *hv) {
    hv->version = APPLE80211_VERSION;
    strncpy(hv->string, fake_drv_version, sizeof(hv->string));
    hv->string_len = strlen(fake_drv_version);
    return kIOReturnSuccess;
}

//
// MARK: 44 - HARDWARE_VERSION
//

IOReturn Black80211Control::getHARDWARE_VERSION(IO80211Interface *interface,
                                                struct apple80211_version_data *hv) {
    hv->version = APPLE80211_VERSION;
    strncpy(hv->string, fake_hw_version, sizeof(hv->string));
    hv->string_len = strlen(fake_hw_version);
    return kIOReturnSuccess;
}

//
// MARK: 46 - RSN_IE
//
IOReturn Black80211Control::getRSN_IE(IO80211Interface *interface,
                                                struct apple80211_rsn_ie_data *rsn_ie_data) {
    rsn_ie_data->version = APPLE80211_VERSION;
	rsn_ie_data->len = current_rsn_ie_length;
	memcpy(rsn_ie_data->ie, current_rsn_ie, current_rsn_ie_length);
    return kIOReturnSuccess;
}

IOReturn Black80211Control::setRSN_IE(IO80211Interface *interface,
                                                struct apple80211_rsn_ie_data *rsn_ie_data) {
    rsn_ie_data->version = APPLE80211_VERSION;
	current_rsn_ie_length = rsn_ie_data->len;
	memcpy(current_rsn_ie, rsn_ie_data->ie, current_rsn_ie_length);
    return kIOReturnSuccess;
}

//
// MARK: 50 - ASSOCIATION_STATUS
//
IOReturn Black80211Control::getASSOCIATION_STATUS(IO80211Interface *interface,
												  struct apple80211_assoc_status_data *sd) {
	sd->version = APPLE80211_VERSION;
	int state = itlwm_get_state(fItlWm);
	//if (state == APPLE80211_S_RUN)
	//	sd->status = APPLE80211_RESULT_SUCCESS;
	//else
	//	sd->status = APPLE80211_RESULT_UNAVAILABLE;
	sd->status = 0;
	IOLog("getASSOCIATION_STATUS: state %d, assoc status %d\n", state, sd->status);
	return kIOReturnSuccess;
}

//
// MARK: 51 - COUNTRY_CODE
//

IOReturn Black80211Control::getCOUNTRY_CODE(IO80211Interface *interface,
                                            struct apple80211_country_code_data *cd) {
    cd->version = APPLE80211_VERSION;
    strncpy((char*)cd->cc, fake_country_code, sizeof(cd->cc));
    return kIOReturnSuccess;
}

//
// MARK: 57 - MCS
//
IOReturn Black80211Control::getMCS(IO80211Interface* interface, struct apple80211_mcs_data* md) {
    md->version = APPLE80211_VERSION;
    md->index = APPLE80211_MCS_INDEX_AUTO;
    return kIOReturnSuccess;
}

IOReturn Black80211Control::getROAM_THRESH(IO80211Interface* interface, struct apple80211_roam_threshold_data* md) {
    md->threshold = 1000;
    md->count = 0;
    return kIOReturnSuccess;
}

IOReturn Black80211Control::getRADIO_INFO(IO80211Interface* interface, struct apple80211_radio_info_data* md)
{
    md->version = 1;
    md->count = 1;
    return kIOReturnSuccess;
}

//
// MARK: 90 - SCANCACHE_CLEAR
//

IOReturn Black80211Control::setSCANCACHE_CLEAR(IO80211Interface *interface) {
	networkIndex = 0;
	scan_result = nullptr;
	return kIOReturnSuccess;
}

//
// MARK: 156 - LINK_CHANGED_EVENT_DATA
//

IOReturn Black80211Control::getLINK_CHANGED_EVENT_DATA(IO80211Interface *interface, struct apple80211_link_changed_event_data *ed) {
	if (ed == nullptr)
		return 16;

	bzero(ed, sizeof(apple80211_link_changed_event_data));
	ed->isLinkDown = itlwm_get_state(fItlWm) != APPLE80211_S_RUN;
	ed->rssi = itlwm_get_rssi(fItlWm);
	if (ed->isLinkDown) {
		ed->voluntary = true;
		ed->reason = APPLE80211_LINK_DOWN_REASON_DEAUTH;
	}
	IOLog("Link down: %d, reason: %d\n", ed->isLinkDown, ed->reason);
	return kIOReturnSuccess;
}
