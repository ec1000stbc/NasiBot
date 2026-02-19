#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <Arduino.h>

class IRSensor {
  public:
    enum Position {
      LEFT,
      RIGHT
    };

    IRSensor(int pin);

    void begin();

    bool detected();        // Linie erkannt?
    int  readRaw();         // digitalRead direkt
    int  getPin();

  private:
    int pin;
};

#endif
