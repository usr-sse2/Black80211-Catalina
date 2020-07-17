This project joins https://github.com/AppleIntelWifi/Black80211-Catalina and https://github.com/OpenIntelWireless/itlwm to provide a wireless driver for Intel adapters that can be controlled using native ![AirPort](NetworkMenuIcon.png) menu item.

![menu](NetworkMenu.png) 

It works in such way:

- There are two network interfaces: Ethernet from itlwm and Wi-Fi from Black80211.
- Ethernet interface is used for actual data transfer, it has the real MAC and IP addresses.
![interfaces](NetworkInterfaces.png)
- Wi-Fi interface is used only for control, it should be manually set to a link-local IP address and made the lowest priority (order) in Network Preferences.
![ip](IPSettings.png)

The following actions already work:
- Turning Wi-Fi off and on
- Scanning networks (currently is poor due to frequent error 16, timeouts or airportd silently ignoring reported scan results)
- Connecting to Open, WEP Open System and WPA2 Personal networks using entered password

The following authentication types are not supported:
- WEP Shared Key
- WPA3
- OWE
- All kinds of WPA Enterprise

The following has not been tested:
- WPA Personal
- Loading on boot (I load it manually using kextutil).

itlwm should be used from https://github.com/usr-sse2/itlwm

Only macOS Catalina is supported.

How to load:
put Black80211.kext and itlwm.kext in the same folder
```bash
sudo chown -R root:wheel *.kext
sudo chmod -R 755 *.kext
kextutil Black80211.kext -r .
```

![profiler](SystemProfiler.png)
