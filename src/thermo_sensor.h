#ifndef THERMO_SENSOR_H__
#define THERMO_SENSOR_H__

#include <DHT.h>
#include <DHT_U.h>

template<int DHT_PIN, int DHTTYPE>
class ThermoSensor {
public:
  static const int DhtPin = DHT_PIN;

  ThermoSensor() : dht_(DHT_PIN, DHTTYPE), humidity_(0), temperature_(0), heat_index_(0) {}

  void setup() {
    dht_.begin();

    humidity_ = 0;
    temperature_ = 0;
    heat_index_ = 0;
  }

  bool fetch() {
    float humidity = dht_.readHumidity();
    float temperature = dht_.readTemperature();

    if(isnan(humidity) || isnan(temperature)) {
      return false;
    }

    humidity_ = humidity;
    temperature_ = temperature_;
    heat_index_ = dht_.computeHeatIndex(temperature_, humidity_, false);

    return true;
  }

  float humidity() const { return humidity_; }
  float temperature() const { return temperature_; }
  float heat_index() const { return heat_index_; }

private:
  DHT dht_;
  float humidity_;
  float temperature_;
  float heat_index_;
};

#endif//THERMO_SENSOR_H__
