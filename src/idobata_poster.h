#ifndef IDOBATA_POSTER_H__
#define IDOBATA_POSTER_H__

#include <arduino.h>
#include <SoftwareSerial.h>
#include "ESP8266.h"

#define SSID        "SSID"
#define PASSWORD    "PASSWORD"
#define HOST_NAME   "HOST_NAME"
#define PATH        "PATH"
#define HOST_PORT   80

class IdobataPoster {
public:
  static const int RxPin = 12;
  static const int TxPin = 11;

  IdobataPoster() : serial_(RxPin, TxPin), wifi_(serial_) {}

  void setup() {
    wifi_.getVersion();
    wifi_.setOprToStationSoftAP();
    wifi_.joinAP(SSID, PASSWORD);
    wifi_.getLocalIP();
    wifi_.disableMUX();
  }

  String post(const String& s) {
    wifi_.createTCP(HOST_NAME, HOST_PORT);

    String request =
      "POST " PATH " HTTP/1.1\r\n"
      "HOST: " HOST_NAME "\r\n"
      "Content-Type: application/x-www-form-urlencoded\r\n"
      "Content-Length: ";

    request.concat(String(s.length(), DEC));
    request.concat("\r\n\r\n");
    request.concat(s);

    wifi_.send(reinterpret_cast<const uint8_t*>(request.c_str()), request.length());

    uint8_t buffer[512];
    String response = "";
    uint32_t response_length = wifi_.recv(buffer, sizeof(buffer), 10000);

    if(response_length > 0) {
      for(uint32_t i = 0; i < response_length; ++i) {
        response += static_cast<char>(buffer[i]);
      }
    }

    return response;
  }

private:
  SoftwareSerial serial_;
  ESP8266 wifi_;
};

#endif//IDOBATA_POSTER_H__
