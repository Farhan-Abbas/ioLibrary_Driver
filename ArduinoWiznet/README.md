# ArduinoWiznet wrapper

This folder contains a small Arduino-friendly wrapper around the WIZnet ioLibrary_Driver included in the repository.

What it provides:
- `ArduinoWiznet::begin(spi_rb, spi_wb, cs_sel, cs_desel)` — register SPI and CS callbacks.
- `ArduinoWiznet::SocketClient` — a tiny single-socket TCP client wrapper (open/connect/send/receive/close).

Usage notes:
- The wrapper does not depend on ArduinoSPI directly; you must provide byte-level SPI callbacks that call `SPI.transfer` or your HAL.
- The example `examples/BasicClient/BasicClient.ino` demonstrates how to register callbacks and perform a simple HTTP GET.
- This is intentionally minimal. For a production-quality Arduino library consider handling DHCP, DNS, non-blocking sockets, TLS, and better error mapping.

Limitations & next steps:
- No automatic netinfo configuration is provided (use `wizchip_setnetinfo()` from `wizchip_conf.h`).
- Only IPv4 TCP client example is included.
- You may want to add wrappers for UDP, DHCP, DNS and EthernetServer-like APIs.
