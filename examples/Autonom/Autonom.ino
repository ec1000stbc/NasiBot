#include <Nasibot.h>

NasiBot bot;

void setup() {
  Serial.begin(115200);
  
  bot.begin();
  bot.setSpeed(180);  // Spitzengeschwindigkeit für autonomen Modus
  
  // bot.setDriveMode(DRIVE_DIRECT);      // ohne Beschleunigung
  bot.setDriveMode(DRIVE_ACCELERATED);   // mit Beschleunigung
}

void loop() {
  bot.update();          // Motor-Updates
  bot.autonomous();      // Fahrlogik
}
