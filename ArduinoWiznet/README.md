# ArduinoWiznet wrapper

This folder contains a small Arduino-friendly wrapper around the WIZnet ioLibrary_Driver included in the repository.

What it provides:

What it provides now:
- `Ethernet_Arduino.h` / `Ethernet_Arduino.cpp` — a minimal `EthernetClass` that implements `config()`, `setSPISpeed()`, `begin()` and `connected()` and hooks into the ioLibrary driver.
- `EthernetUDP_Arduino.h` / `EthernetUDP_Arduino.cpp` — a minimal `EthernetUDP` implementation exposing `begin()`, `parsePacket()`, `read()`, `beginPacket()`, `write()`, `endPacket()`, `remoteIP()`, `remotePort()` that map to the ioLibrary UDP functions.

Usage notes:
- The wrapper does not depend on ArduinoSPI directly; you must provide byte-level SPI callbacks that call `SPI.transfer` or your HAL.
- The example `examples/BasicClient/BasicClient.ino` demonstrates how to register callbacks and perform a simple HTTP GET.
- This is intentionally minimal. For a production-quality Arduino library consider handling DHCP, DNS, non-blocking sockets, TLS, and better error mapping.

Limitations & next steps:
- No automatic netinfo configuration is provided (use `wizchip_setnetinfo()` from `wizchip_conf.h`).
- Only IPv4 TCP client example is included.
- You may want to add wrappers for UDP, DHCP, DNS and EthernetServer-like APIs.

How this maps to your existing code
- Your code references an `eth` object with `config()`, `setSPISpeed()`, `begin()`, and `connected()` and a global `Udp` with the UDP methods: the added `EthernetClass` and `EthernetUDP` provide those methods so your code can keep the same calls with minimal changes. Create `EthernetClass eth(<cs>, SPI, <reset>)` and use `extern EthernetUDP Udp` already provided.

Example minimal usage in setup():

```cpp
#include "ArduinoWiznet/Ethernet_Arduino.h"
#include "ArduinoWiznet/EthernetUDP_Arduino.h"

EthernetClass eth(17, SPI, 21);
// EthernetUDP Udp; // already instantiated in the wrapper

void setup() {
	IPAddress ip(169,254,100,100), gw(169,254,100,0), sn(255,255,0,0), dns(8,8,8,8);
	SPI.begin();
	eth.config(ip, gw, sn, dns);
	eth.setSPISpeed(36000000);
	eth.begin();
	Udp.begin(8888);
}
```

