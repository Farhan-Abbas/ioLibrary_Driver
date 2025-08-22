#pragma once

/*
  Arduino-friendly wrapper for WIZnet ioLibrary_Driver
  - Provides simple init(register SPI/CS callbacks) and small SocketClient class
  Notes: The user must provide SPI byte read/write callbacks that call Arduino SPI.transfer
*/

#include <Arduino.h>
#include "../../Ethernet/wizchip_conf.h"

namespace ArduinoWiznet {

// Configure the low-level callbacks used by the existing ioLibrary
// spi_rb should return a received byte (calls SPI.transfer(0x00))
// spi_wb should send a byte (calls SPI.transfer(b))
void begin(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t), void (*cs_sel)(void), void (*cs_desel)(void));

// A tiny TCP client wrapper using one socket
class SocketClient {
public:
  SocketClient(uint8_t sockNum = 0);
  int beginClient(uint16_t localPort = 0);
  int connectIP(const uint8_t ip[4], uint16_t port);
  int send(const uint8_t* buf, uint16_t len);
  int receive(uint8_t* buf, uint16_t len);
  int close();
private:
  uint8_t _sock;
};

} // namespace ArduinoWiznet
