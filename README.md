This project joins https://github.com/AppleIntelWifi/Black80211-Catalina and https://github.com/OpenIntelWireless/itlwm to provide a wireless driver for Intel adapters that can be controlled using native ![AirPort](NetworkMenuIcon.png) menu item.

![menu](NetworkMenu.png) 

The driver is recognized by the system as a Wi-Fi adapter, not an Ethernet adapter:

![profiler](SystemProfiler.png)

![interfaces](NetworkInterfaces.png)

![ip](IPSettings.png)

The following actions already work:
- Turning Wi-Fi off and on
- Scanning networks (this also allows Location Services to detect location)

![maps](Maps.png)

- Connecting to Open, WEP Open System and WPA2 Personal networks using entered password

The following authentication types are not supported:
- WEP Shared Key
- WPA3
- OWE
- All kinds of WPA Enterprise

The following has not been tested:
- WPA Personal (not WPA2)

itlwm should be used from https://github.com/usr-sse2/itlwm

Only macOS Catalina is supported.

### How to load manually:
put Black80211.kext and itlwm.kext in the same folder
```bash
sudo chown -R root:wheel *.kext
sudo chmod -R 755 *.kext
sudo kextutil itlwm.kext
sudo kextutil Black80211.kext -r .
```

### How to load on boot:
- *(Recommended)* the kexts can be injected with this version of OpenCore: https://github.com/acidanthera/bugtracker/issues/1071. It supports loading `immutablekernel` which always contains IO80211Family.kext;
- *(Not recommended)* the kexts can be installed to `/Library/Extensions` with System Integrity Protection disabled;
- only in the installer or Recovery the kexts can be injected with regular OpenCore or Clover, because macOS excludes IO80211Family from the `prelinkedkernel` when there are no wireless devices with drivers in system locations.
