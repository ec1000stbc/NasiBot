/****************************************************************************
  NasiBot – Linienfolger
  
  Aufgabe:
  Ergänze die fehlenden Fälle, damit der NasiBot der Linie folgt.
  Nutze dafür die Methoden:
    bot.forward()      → geradeaus fahren
    bot.curveLeft()    → sanfte Kurve nach links
    bot.curveRight()   → sanfte Kurve nach rechts
    bot.turnLeft()     → auf der Stelle nach links drehen
    bot.turnRight()    → auf der Stelle nach rechts drehen
    bot.stop()         → anhalten
****************************************************************************/

#include <Nasibot.h>

NasiBot bot;

// IR-Sensor Pins
const int IN_D0_L = A1;   // linker Sensor
const int IN_D0_R = A0;   // rechter Sensor

IRSensor irLeft(IN_D0_L);
IRSensor irRight(IN_D0_R);

void setup() {
  Serial.begin(9600);

  irLeft.begin();
  irRight.begin();

  bot.begin();
  bot.setSpeed(150);              // Geschwindigkeit begrenzen (0–255)

  // bot.setDriveMode(DRIVE_DIRECT);       // ohne Beschleunigung (direkt)
  bot.setDriveMode(DRIVE_ACCELERATED);    // mit sanfter Beschleunigung
}

void loop() {
  bool left  = irLeft.detected();
  bool right = irRight.detected();

  if (!left && !right) {
    // Beide Sensoren auf weiß → Linie liegt zwischen den Sensoren → geradeaus
    Serial.println("LINE: CENTER");
    bot.forward();

  } else if (left && !right) {
    // Linker Sensor auf schwarz → zu weit nach links → Korrektur nötig!
    Serial.println("LINE: TOO LEFT");

    // TODO: Welche Methode passt hier?
    // bot.???

  } else if (!left && right) {
    // Rechter Sensor auf schwarz → zu weit nach rechts → Korrektur nötig!
    Serial.println("LINE: TOO RIGHT");

    // TODO: Welche Methode passt hier?
    // bot.???

  } else {
    // Beide Sensoren auf schwarz → Kreuzung oder Linienende
    Serial.println("LINE: BOTH BLACK");

    // TODO: Was soll der Bot hier tun?
    // bot.???
  }

  bot.update();   // Geschwindigkeit aktualisieren (wichtig bei DRIVE_ACCELERATED)
  delay(50);
}
