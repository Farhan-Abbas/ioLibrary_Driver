#include <SPI.h>
#include "ArduinoWiznet.h"

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
  cs_deselect_cb();

  ArduinoWiznet::begin(spi_readbyte_cb, spi_writebyte_cb, cs_select_cb, cs_deselect_cb);

  ArduinoWiznet::SocketClient client(0);
  if(client.beginClient(0) != 0){
    Serial.println("socket open failed");
    while(1);
  }

  uint8_t ip[4] = {192,168,1,100}; // change to target
  if(client.connectIP(ip, 80) == 0){
    const char* msg = "GET / HTTP/1.0\r\nHost: 192.168.1.100\r\n\r\n";
    client.send((const uint8_t*)msg, strlen(msg));
    uint8_t buf[256];
    int r = client.receive(buf, sizeof(buf));
    if(r > 0) Serial.write(buf, r);
    client.close();
  } else {
    Serial.println("connect failed");
  }
}

void loop() {}
