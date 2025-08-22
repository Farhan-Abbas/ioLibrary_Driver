#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "../Ethernet/wizchip_conf.h"
#include <IPAddress.h>

class EthernetClass {
public:
  EthernetClass(uint8_t csPin, SPIClass &spi, uint8_t resetPin = 255);
  void config(IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress dns);
  void setSPISpeed(uint32_t hz) { spiSpeed = hz; }
  int begin();
  bool connected();
private:
  uint8_t _csPin;
  uint8_t _resetPin;
  SPIClass* _spi;
  uint32_t spiSpeed;
  wiz_NetInfo netinfo;
};

// Provide a default instance name used in many sketches if needed
// extern EthernetClass eth; // leave creation to user to match their usage
