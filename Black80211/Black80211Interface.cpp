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
	uint8_t ether_type[2];
	if (mbuf_len(packet) >= 14 && mbuf_copydata(packet, 12, 2, &ether_type) == 0 && (ether_type[0] == 0x88 && ether_type[1] == 0x8e)) { // EAPOL packet
		const char* dump = hexdump((uint8_t*)mbuf_data(packet), mbuf_len(packet));
		IOLog("Black80211: input EAPOL packet, len: %zu, data: %s\n", mbuf_len(packet), dump ? dump : "Failed to allocate memory");
		if (dump)
			IOFree((void*)dump, 3 * mbuf_len(packet) + 1);
		return IO80211Interface::inputPacket(packet, length, options, param);
	}
	return IONetworkInterface::inputPacket(packet, length, options, param);
}
