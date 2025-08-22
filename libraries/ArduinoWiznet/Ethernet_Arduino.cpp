#include "Ethernet_Arduino.h"
#include <SPI.h>

EthernetClass::EthernetClass(uint8_t csPin, SPIClass &spi, uint8_t resetPin)
  : _csPin(csPin), _resetPin(resetPin), _spi(&spi), spiSpeed(4000000)
{
}

void EthernetClass::config(IPAddress ip, IPAddress gateway, IPAddress subnet, IPAddress dns)
{
  memset(&netinfo, 0, sizeof(netinfo));
  // MAC left zero — user should set if required
  for(int i=0;i<4;i++){ netinfo.ip[i] = ip[i]; netinfo.gw[i]=gateway[i]; netinfo.sn[i]=subnet[i]; netinfo.dns[i]=dns[i]; }
  netinfo.dhcp = NETINFO_STATIC;
  wizchip_setnetinfo(&netinfo);
}

int EthernetClass::begin()
{
  // register SPI callbacks using simple Arduino SPI.transfer
  auto spi_rb = []()->uint8_t { return SPI.transfer(0x00); };
  auto spi_wb = [](uint8_t b)->void { SPI.transfer(b); };
  // CS callbacks
  auto cs_sel = [this](){ digitalWrite(this->_csPin, LOW); };
  auto cs_desel = [this](){ digitalWrite(this->_csPin, HIGH); };

  reg_wizchip_cs_cbfunc((void(*)())cs_sel, (void(*)())cs_desel);
  reg_wizchip_spi_cbfunc((uint8_t(*)(void))spi_rb, (void(*)(uint8_t))spi_wb);

  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);

  uint8_t txsize[_WIZCHIP_SOCK_NUM_];
  uint8_t rxsize[_WIZCHIP_SOCK_NUM_];
  for(int i=0;i<_WIZCHIP_SOCK_NUM_;i++){ txsize[i]=2; rxsize[i]=2; }
  if(wizchip_init(txsize, rxsize) != 0) return -1;
  return 0;
}

bool EthernetClass::connected()
{
  // minimal check: consider link up if gateway is not 0.0.0.0
  wiz_NetInfo ni;
  wizchip_getnetinfo(&ni);
  for(int i=0;i<4;i++) if(ni.gw[i]!=0) return true;
  return false;
}
