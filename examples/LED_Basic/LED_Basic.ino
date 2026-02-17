/*2WD-Car - Leds
Methoden:
  on()
  off()
  isOn()
  toggle()
*/
#include "Led_Basic.h"

#define frontL A2
#define frontR A3
#define backL  9
#define backR  10

Led ledFrontL(frontL);
Led ledFrontR(frontR);
Led ledBackL(backL);
Led ledBackR(backR);

void setup() {
  // pinMode ist jetzt im Konstruktor 👍
}

void loop() {
  ledFrontL.on();
  ledFrontR.on();
  ledBackL.on();
  ledBackR.on();
}