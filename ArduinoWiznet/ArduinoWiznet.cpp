#include "ArduinoWiznet.h"

using namespace ArduinoWiznet;

void ArduinoWiznet::begin(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t), void (*cs_sel)(void), void (*cs_desel)(void))
{
  // register CS and SPI callbacks into existing driver
  reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
  reg_wizchip_spi_cbfunc(spi_rb, spi_wb);
}

SocketClient::SocketClient(uint8_t sockNum)
{
  _sock = sockNum;
}

int SocketClient::beginClient(uint16_t localPort)
{
  int8_t ret = socket(_sock, Sn_MR_TCP, localPort, 0);
  return ret == _sock ? 0 : -1;
}

int SocketClient::connectIP(const uint8_t ip[4], uint16_t port)
{
  int8_t ret = connect(_sock, (uint8_t*)ip, port);
  return ret == SOCK_OK ? 0 : ret;
}

int SocketClient::send(const uint8_t* buf, uint16_t len)
{
  int32_t r = ::send(_sock, (uint8_t*)buf, len);
  return (int)r;
}

int SocketClient::receive(uint8_t* buf, uint16_t len)
{
  int32_t r = recv(_sock, buf, len);
  return (int)r;
}

int SocketClient::close()
{
  int8_t r = ::close(_sock);
  return r == SOCK_OK ? 0 : -1;
}
