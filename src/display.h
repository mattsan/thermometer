#ifndef DISPLAY_H__
#define DISPLAY_H__

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

#endif//DISPLAY_H__
