/****************************************************************************
  2WD LED-Programm mit Objektorientierung (Enum-Version)
****************************************************************************/

#include <SoftPWM.h>
#include <Led.h>

// Objekte (keine Pins mehr nötig!)
Led frontLeft(LED_FRONT_LEFT);
Led frontRight(LED_FRONT_RIGHT);
Led topLeft(LED_TOP_LEFT);
Led topRight(LED_TOP_RIGHT);

// Optional:
// Led front(LED_FRONT);

void setup() {
  Serial.begin(9600);
  SoftPWMBegin();
}

void loop() {
  frontLeft.on();
  frontRight.dimm(20);
  topLeft.blink(500);
  topRight.on();

  // front.blink(300);   // beide Front-LEDs gleichzeitig
}
