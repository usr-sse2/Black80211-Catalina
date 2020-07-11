This project joins https://github.com/AppleIntelWifi/Black80211-Catalina and https://github.com/OpenIntelWireless/itlwm to provide a wireless driver for Intel adapters that can be controlled using native AirPort menu item. It works in such way:

- There are two network interfaces: Ethernet from itlwm and Wi-Fi from Black80211.
- Ethernet interface is used for actual data transfer, it has the real MAC and IP addresses.
- Wi-Fi interface is used only for control, it should be manually set to a link-local IP address and made the lowest priority (order) in Network Preferences.

The following actions already work:
- Turning Wi-Fi off and on
- Scanning networks (currently is poor due to frequent error 16 or timeouts)
- Connecting to open and WPA2 Personal networks using entered password

Other encryption types are not tested. Loading on boot is not tested (I load it manually using kextutil).

itlwm should be used from https://github.com/usr-sse2/itlwm

How to load:
put Black80211.kext and itlwm.kext in the same folder
```bash
sudo chown -R root:wheel *.kext
sudo chmod -R 755 *.kext
kextutil Black80211.kext -r .
```
