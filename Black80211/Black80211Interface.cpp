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

UInt32 Black80211Interface::inputPacket(mbuf_t packet, UInt32 length, IOOptionBits options, void *param) {
	return IONetworkInterface::inputPacket(packet, length, options, param);
}
