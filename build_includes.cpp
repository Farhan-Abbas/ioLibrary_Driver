// Force compilation of library implementation files that the Arduino build system isn't picking up.
// Including C sources inside an extern "C" block ensures correct linkage.

#include "libraries/ArduinoWiznet/Ethernet_Arduino.cpp"

extern "C" {
#include "libraries/Ethernet/socket.c"
#include "libraries/Ethernet/wizchip_conf.c"
#include "libraries/Ethernet/W6300/w6300.c"
}
