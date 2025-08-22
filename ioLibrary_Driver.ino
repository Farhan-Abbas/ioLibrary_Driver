#include <SPI.h>
#include <IPAddress.h>
#include "libraries/Ethernet/socket.h"
#include "libraries/ArduinoWiznet/Ethernet_Arduino.h"

// Define CS control for WIZnet module (change pin as required)
const int CS_PIN = 10;

uint8_t spi_readbyte_cb(void)
{
  return SPI.transfer(0x00);
}

void spi_writebyte_cb(uint8_t b)
{
  SPI.transfer(b);
}

void cs_select_cb(void)
{
  digitalWrite(CS_PIN, LOW);
}

void cs_deselect_cb(void)
{
  digitalWrite(CS_PIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // Use the Arduino wrapper EthernetClass
  EthernetClass eth(CS_PIN, SPI);
  IPAddress ipaddr(192,168,1,100), gw(192,168,1,1), sn(255,255,255,0), dns(8,8,8,8);
  eth.config(ipaddr, gw, sn, dns);
  eth.setSPISpeed(36000000);
  if(eth.begin() != 0){
    Serial.println("wizchip init failed");
    while(1);
  }

  // use raw socket for simple TCP GET via socket API
  int8_t s = socket(0, Sn_MR_TCP, 0, 0);
  if(s != 0){
    Serial.println("socket open failed");
    while(1);
  }

  uint8_t dest[4] = {192,168,1,100};
  if(connect(0, dest, 80) == SOCK_OK){
    const char* msg = "GET / HTTP/1.0\r\nHost: 192.168.1.100\r\n\r\n";
    send(0, (uint8_t*)msg, strlen(msg));
    uint8_t buf[256];
    int32_t r = recv(0, buf, sizeof(buf));
    if(r > 0) Serial.write(buf, r);
    close(0);
  } else {
    Serial.println("connect failed");
  }
}

void loop() {}
