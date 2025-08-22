#pragma once
#include <Arduino.h>
#include "wizchip_conf.h"

class EthernetUDP {
public:
  EthernetUDP();
  int begin(uint16_t port);
  int parsePacket();
  int read(uint8_t* buf, int len);
  int beginPacket(IPAddress ip, uint16_t port);
  int write(const uint8_t* buf, size_t size);
  int endPacket();
  IPAddress remoteIP();
  uint16_t remotePort();
private:
  uint8_t sock;
  uint8_t remote_ip[4];
  uint16_t remote_port;
};

extern EthernetUDP Udp;
