/****************************************************************************
  NasiBot – Linienfolger (Beispiellösung)
****************************************************************************/

#include <Nasibot.h>

NasiBot bot;

const int IN_D0_L = A1;
const int IN_D0_R = A0;

IRSensor irLeft(IN_D0_L);
IRSensor irRight(IN_D0_R);

const int THRESHOLD = 300;  // Schwellwert zwischen weiß (~36) und schwarz (~1018)

void setup() {
  Serial.begin(115200);

  irLeft.begin();
  irRight.begin();

  bot.begin();
  bot.setSpeed(90);  // Geschwindigkeit (0–255) – höher = schärfere Kurven möglich
  bot.setDriveMode(DRIVE_DIRECT);
}

void loop() {
  bool left  = irLeft.detectedAnalog(THRESHOLD);
  bool right = irRight.detectedAnalog(THRESHOLD);

  if (!left && !right) {
    // Beide Sensoren auf weiß → Linie liegt zwischen den Sensoren → geradeaus
    Serial.println("LINE: CENTER");
    bot.forward();

  } else if (left && !right) {
    // Linker Sensor auf schwarz → zu weit nach links → nach rechts korrigieren
    Serial.println("LINE: TOO LEFT");
    bot.curveLeft();
    delay(120);

  } else if (!left && right) {
    // Rechter Sensor auf schwarz → zu weit nach rechts → nach links korrigieren
    Serial.println("LINE: TOO RIGHT");
    bot.curveRight();
    delay(120);

  } else {
    // Beide Sensoren auf schwarz → Kreuzung oder Linienende → anhalten
    Serial.println("LINE: BOTH BLACK");
    bot.stop();
  }

  bot.update();
  delay(30);
}
