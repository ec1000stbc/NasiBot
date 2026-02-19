/****************************************************************************
  NasiBot – Linienfolger (Beispiellösung)
****************************************************************************/

#include <Nasibot.h>

NasiBot bot;

const int IN_D0_L = A1;
const int IN_D0_R = A0;

IRSensor irLeft(IN_D0_L);
IRSensor irRight(IN_D0_R);

void setup() {
  Serial.begin(9600);

  irLeft.begin();
  irRight.begin();

  bot.begin();
  bot.setSpeed(100);              // Geschwindigkeit begrenzen (0–255)
  bot.setDriveMode(DRIVE_ACCELERATED);
}

void loop() {
  bool left  = irLeft.detected();
  bool right = irRight.detected();

  if (!left && !right) {
    // Beide Sensoren auf weiß → Linie liegt zwischen den Sensoren → geradeaus
    Serial.println("LINE: CENTER");
    bot.forward();

  } else if (left && !right) {
    // Linker Sensor auf schwarz → zu weit nach links → nach rechts korrigieren
    Serial.println("LINE: TOO LEFT");
    bot.curveRight();

  } else if (!left && right) {
    // Rechter Sensor auf schwarz → zu weit nach rechts → nach links korrigieren
    Serial.println("LINE: TOO RIGHT");
    bot.curveLeft();

  } else {
    // Beide Sensoren auf schwarz → Kreuzung oder Linienende → anhalten
    Serial.println("LINE: BOTH BLACK");
    bot.stop();
  }

  bot.update();
  delay(50);
}
