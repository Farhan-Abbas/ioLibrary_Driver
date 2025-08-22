// Force-link C implementations into the ArduinoWiznet library so the Arduino build system
// compiles and links the C sources (socket, wizchip_conf, W6300) as part of the library.

extern "C" {
#include "../Ethernet/socket.c"
#include "../Ethernet/wizchip_conf.c"
#include "../Ethernet/W6300/w6300.c"
}
