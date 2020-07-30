//
//  Black80211Interface.hpp
//  Black80211_Catalina
//
//  Created by usrsse2 on 29.07.2020.
//  Copyright © 2020 Roman Peshkov. All rights reserved.
//

#ifndef Black80211Interface_hpp
#define Black80211Interface_hpp

#include "IO80211Controller.h"
#include "IO80211Interface.h"

class Black80211Interface : public IO80211Interface {
    OSDeclareDefaultStructors( Black80211Interface );

public:
	virtual UInt32 inputPacket(mbuf_t packet, UInt32 length = 0, IOOptionBits options = 0, void *param = 0) override;
};

#endif /* Black80211Interface_hpp */
