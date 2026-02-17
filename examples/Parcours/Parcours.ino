//Methoden
//forward(), backward(), turnLeft(), turnRight(), stop()

#include <Nasibot.h>

NasiBot bot;

void setup() {
  bot.begin();
  bot.setDriveMode(DRIVE_ACCELERATED);
  bot.update();
}

void loop() {
  // --- Start Parcours ---

  bot.forward();   // Ziel setzen: Vorwärts
  bot.wait(1000);  // Fährt 1s geradeaus (MIT PID-Korrektur & Beschleunigung!)

  bot.stop();      // Ziel setzen: Stop
  bot.wait(800);   // Steht 0.8s still

  bot.turnRight(); // Ziel setzen: Rechts drehen
  bot.wait(200);   // Dreht für 200ms

  bot.stop();
  bot.wait(500);

  bot.forward();
  bot.wait(1200);  // Fährt 1.2s geradeaus (MIT PID-Korrektur!)

  // --- Ende Parcours ---
  bot.stop();
  while(true) {
    bot.update(); // Falls er noch ausrollen muss
  }
}

