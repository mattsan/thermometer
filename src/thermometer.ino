//  - DHT11 x1
//    1. input pin
//  - 7seg (2231B) x1 + 8bit shift register x1 + 2SC1815 x2
//    1. data pin
//    2. clock pin
//    3. latch pin
//    4. fig.1 pin
//    5. fig.2 pin
//  - common
//    1. Vcc
//    2. GND

#include <DHT.h>
#include <DHT_U.h>
#include <TimerOne.h>

class Digits {
public:
  Digits(int data_pin, int clock_pin, int latch_pin, int fig1_pin, int fig2_pin) :
    data_pin_(data_pin),
    clock_pin_(clock_pin),
    latch_pin_(latch_pin),
    fig1_pin_(fig1_pin),
    fig2_pin_(fig2_pin) {}

  void init() {
    pinMode(data_pin_, OUTPUT);
    pinMode(clock_pin_, OUTPUT);
    pinMode(latch_pin_, OUTPUT);
  }

  void show(int value, bool dot1, bool dot2) {
    showFig1(value / 10, dot1);
    showFig2(value % 10, dot2);
  }

  void showFig1(int value, bool dot) {
    digitalWrite(latch_pin_, LOW);
    shiftOut(data_pin_, clock_pin_, LSBFIRST, value);
    digitalWrite(fig2_pin_, LOW);
    digitalWrite(fig1_pin_, HIGH);
    digitalWrite(latch_pin_, HIGH);
  }

  void showFig2(int value, bool dot) {
    digitalWrite(latch_pin_, LOW);
    shiftOut(data_pin_, clock_pin_, LSBFIRST, value);
    digitalWrite(fig1_pin_, LOW);
    digitalWrite(fig2_pin_, HIGH);
    digitalWrite(latch_pin_, HIGH);
  }

private:
  const int data_pin_;
  const int clock_pin_;
  const int latch_pin_;
  const int fig1_pin_;
  const int fig2_pin_;
};

static const int DATA_PIN = 1;
static const int CLOCK_PIN = 2;
static const int LATCH_PIN = 3;
static const int FIG1_PIN = 4;
static const int FIG2_PIN = 5;

Digits digits(DATA_PIN, CLOCK_PIN, LATCH_PIN, FIG1_PIN, FIG2_PIN);

static const int DHTPIN = 8;
static const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);
static volatile int temperature;
static volatile int humidity;

static bool showingTemperature;

void fetchDHT() {
  temperature = dht.readTemperature(true);
  humidity = dht.readTemperature();

  showingTemperature = !showingTemperature;
}

void setup() {
  temperature = 0;
  humidity = 0;
  showingTemperature = true;

  Timer1.initialize(2000000);
  Timer1.attachInterrupt(fetchDHT);

  digits.init();
}

void loop() {
  if(showingTemperature) {
    digits.show(temperature, false, false);
  } else {
    digits.show(humidity, false, true);
  }
}
