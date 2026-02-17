#include <NasiBot_autonom.h>

NasiBot bot;

void setup() {
  Serial.begin(115200);

  bot.begin();

  // === MODUS WÄHLEN ===
  // bot.setDriveMode(DRIVE_DIRECT);      // ohne Beschleunigung (alt)
  bot.setDriveMode(DRIVE_ACCELERATED);   // mit Beschleunigung
}

void loop() {
  bot.update();       // Beschleunigung (falls aktiv)
  bot.autonomous();  // Fahrlogik
}
