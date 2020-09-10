//
//  Black80211Interface.cpp
//  Black80211_Catalina
//
//  Created by usrsse2 on 29.07.2020.
//  Copyright © 2020 Roman Peshkov. All rights reserved.
//

#include "Black80211Interface.hpp"

#define super IO80211Interface

OSDefineMetaClassAndStructors(Black80211Interface, IO80211Interface);

const char* hexdump(uint8_t *buf, size_t len);

UInt32 Black80211Interface::inputPacket(mbuf_t packet, UInt32 length, IOOptionBits options, void *param) {
	uint16_t ether_type;
	size_t len = mbuf_len(packet);
	if (len >= 14 && mbuf_copydata(packet, 12, 2, &ether_type) == 0 && ether_type == _OSSwapInt16(ETHERTYPE_PAE)) { // EAPOL packet
		const char* dump = hexdump((uint8_t*)mbuf_data(packet), len);
		IOLog("Black80211: input EAPOL packet, len: %zu, data: %s\n", len, dump ? dump : "Failed to allocate memory");
		if (dump)
			IOFree((void*)dump, 3 * len + 1);
		return IO80211Interface::inputPacket(packet, (UInt32)len, 0, param);
	}
	return IONetworkInterface::inputPacket(packet, length, options, param);
}
