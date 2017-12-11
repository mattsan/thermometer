#include <DHT.h>
#include <DHT_U.h>
#include <TimerOne.h>
#include <digit.h>

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

void display(int value, bool dot) {
  digitWrite(FIGURE_1, value / 10, false);
  delay(10);
  digitWrite(FIGURE_2, value % 10, dot);
  delay(10);
}

void setup() {
  temperature = 0;
  humidity = 0;
  showingTemperature = true;

  Timer1.initialize(2000000);
  Timer1.attachInterrupt(fetchDHT);

  digitInit();
}

void loop() {
  if(showingTemperature) {
    display(temperature, false);
  } else {
    display(humidity, true);
  }
}
