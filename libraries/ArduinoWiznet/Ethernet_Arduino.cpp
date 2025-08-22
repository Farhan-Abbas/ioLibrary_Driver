#include "Ethernet_Arduino.h"
#include <SPI.h>
// Helper functions used as C-style callbacks for the wizchip library.
static uint8_t wiz_spi_read(void) { return SPI.transfer(0x00); }
static void wiz_spi_write(uint8_t b) { SPI.transfer(b); }
static uint8_t wiz_cs_pin = 255;
static void wiz_cs_select(void) { digitalWrite(wiz_cs_pin, LOW); }
static void wiz_cs_deselect(void) { digitalWrite(wiz_cs_pin, HIGH); }

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
  // register SPI callbacks using file-scope helper functions
  wiz_cs_pin = this->_csPin;
  reg_wizchip_cs_cbfunc(wiz_cs_select, wiz_cs_deselect);
  reg_wizchip_spi_cbfunc(wiz_spi_read, wiz_spi_write);

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
