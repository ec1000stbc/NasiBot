#include "IR_Sensor.h"

IRSensor::IRSensor(int pin) : pin(pin) {}

void IRSensor::begin() {
  pinMode(pin, INPUT);
}

bool IRSensor::detected() {
  // Die meisten IR-Line-Sensoren:
  // LOW  = Linie (schwarz)
  // HIGH = kein Linie (weiß)
  return digitalRead(pin) == LOW;
}

int IRSensor::readAnalog() {
  return analogRead(pin);  // 0–1023
}

bool IRSensor::detectedAnalog(int threshold) {
  return analogRead(pin) > threshold;  // hoch = dunkel = Linie
}

int IRSensor::readRaw() {
  return digitalRead(pin);
}

int IRSensor::getPin() {
  return pin;
}

