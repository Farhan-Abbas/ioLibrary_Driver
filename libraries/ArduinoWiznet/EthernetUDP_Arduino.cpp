#include "EthernetUDP_Arduino.h"
#include "wizchip_conf.h"

EthernetUDP Udp;

EthernetUDP::EthernetUDP()
{
  sock = 0; // default socket 0
  remote_port = 0;
  memset(remote_ip,0,4);
}

int EthernetUDP::begin(uint16_t port)
{
  int8_t r = socket(sock, Sn_MR_UDP, port, 0);
  return (r == sock) ? 1 : 0;
}

int EthernetUDP::parsePacket()
{
  uint16_t size = 0;
  int8_t ret = getsockopt(sock, SO_RECVBUF, &size);
  if(size == 0) return 0;
  // For simplicity, return available data size using getSn_RX_RSR via getsockopt
  uint16_t rsr = 0;
  getsockopt(sock, SO_RECVBUF, &rsr);
  return rsr;
}

int EthernetUDP::read(uint8_t* buf, int len)
{
  uint8_t addr[4];
  uint16_t port = 0;
  int32_t r = recvfrom(sock, buf, len, addr, &port);
  if(r > 0)
  {
    memcpy(remote_ip, addr, 4);
    remote_port = port;
  }
  return (int)r;
}

int EthernetUDP::beginPacket(IPAddress ip, uint16_t port)
{
  memcpy(remote_ip, ip.raw_address(), 4);
  remote_port = port;
  return 1;
}

int EthernetUDP::write(const uint8_t* buf, size_t size)
{
  int32_t r = sendto(sock, (uint8_t*)buf, size, remote_ip, remote_port);
  return (int)r;
}

int EthernetUDP::endPacket()
{
  // sendto already transmits; nothing additional required.
  return 1;
}

IPAddress EthernetUDP::remoteIP()
{
  return IPAddress(remote_ip);
}

uint16_t EthernetUDP::remotePort()
{
  return remote_port;
}
