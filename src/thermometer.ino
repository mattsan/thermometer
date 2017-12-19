#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <DHT_U.h>

template<
  int DATA_PIN,
  int CLOCK_PIN,
  int LATCH_PIN,
  int FIG1_PIN,
  int FIG2_PIN>
class Display {
public:
  static const int DataPin = DATA_PIN;
  static const int ClockPin = CLOCK_PIN;
  static const int LatchPin = LATCH_PIN;
  static const int Fig1Pin = FIG1_PIN;
  static const int Fig2Pin = FIG2_PIN;

  void setup() {
    pinMode(DataPin, OUTPUT);
    pinMode(ClockPin, OUTPUT);
    pinMode(LatchPin, OUTPUT);
    pinMode(Fig1Pin, OUTPUT);
    pinMode(Fig2Pin, OUTPUT);
  }

  void show(int value, byte dps, int interval = 10) const {
    const byte dp1 = static_cast<byte>((dps << 7) & 0x80);
    const byte dp2 = static_cast<byte>((dps << 6) & 0x80);
    const byte fig1 = static_cast<byte>((value >> 8) & 0x0f) | dp1;
    const byte fig2 = static_cast<byte>(value & 0x0f) | dp2;

    showFigure(fig1, Fig1Pin, Fig2Pin);
    delay(interval);
    showFigure(fig2, Fig2Pin, Fig2Pin);
    delay(interval);
  }

private:
  void showFigure(byte value, int onPin, int offPin) const {
    digitalWrite(LatchPin, LOW);
    shiftOut(DataPin, ClockPin, LSBFIRST, value);
    digitalWrite(LatchPin, HIGH);
    digitalWrite(onPin, HIGH);
    digitalWrite(offPin, LOW);
  }
};

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
