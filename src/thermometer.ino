#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include "display.h"
#include "thermo_sensor.h"

class Thermometer {
public:
  static const int DataPin = 2;
  static const int ClockPin = 3;
  static const int LatchPin = 4;
  static const int Fig1Pin = 5;
  static const int Fig2Pin = 6;
  static const int DhtPin = 7;
  static const int DhtType = DHT11;

  static const unsigned long SamplingInterval = 2000000; // usec

  static void sampling();

  Thermometer() : display_(), thermosensor_() {}

  void setup() {
    display_.setup();
    thermosensor_.setup();

    humidity_ = 0;
    temperature_ = 0;
    showing_temperature_ = true;
  }

  void readSerial() {
  }

  void process() {
    if(showing_temperature_) {
      display_.show(temperature_, false);
    } else {
      display_.show(humidity_, true);
    }
  }

private:
  Display<
    DataPin,
    ClockPin,
    LatchPin,
    Fig1Pin,
    Fig2Pin
  > display_;

  ThermoSensor<
    DhtPin,
    DhtType
  > thermosensor_;

  float humidity_;
  float temperature_;
  bool showing_temperature_;
};

Thermometer thermometer;

void Thermometer::sampling() {
  if(thermometer.thermosensor_.fetch()) {
    thermometer.humidity_ = thermometer.thermosensor_.humidity();
    thermometer.temperature_ = thermometer.thermosensor_.temperature();
  }

  thermometer.showing_temperature_ = !thermometer.showing_temperature_;
}

//----------------------------------------------------------------

void serialEvent() {
  thermometer.readSerial();
}

void setup() {
  thermometer.setup();
  Timer1.initialize(Thermometer::SamplingInterval);
  Timer1.attachInterrupt(Thermometer::sampling);
}

void loop() {
  thermometer.process();
}
