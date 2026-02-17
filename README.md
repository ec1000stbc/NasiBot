# NasiBot

Arduino Library zur Steuerung des **NasiBot Smartcars**.

Die Library fasst alle grundlegenden Funktionen für autonome Fahrzeuge
und LED-Steuerung in einer einheitlichen Struktur zusammen.

---

## 🚗 Features – NasiBot

- Autonomes Fahren mit Ultraschall-Hinderniserkennung
- Gyro-Stabilisierung (MPU6050)
- Sanfte Beschleunigung (Acceleration Mode)
- Drehungen auf der Stelle (Tank Turn)
- Vorwärts-, Rückwärts- und Drehbewegungen

---

## 💡 Features – LED

### Led_Basic
- Ein/Aus
- Toggle
- Statusabfrage

### Led_Dimm
- PWM-Dimmen
- SoftPWM-Unterstützung für nicht-PWM-Pins
- Blinken (nicht blockierend)
- Statusabfrage

---

## 📦 Installation

1. Repository herunterladen oder klonen
2. Arduino IDE öffnen
3. **Sketch → Bibliothek einbinden → ZIP-Bibliothek hinzufügen**
4. Projekt neu starten

---

## 🧪 Beispiel – Autonomes Fahren

```cpp
#include <NasiBot.h>

NasiBot bot;

void setup() {
  bot.begin();
  bot.setDriveMode(DRIVE_ACCELERATED);
}

void loop() {
  bot.update();
  bot.autonomous();
}

## 📜 Lizenz
MIT License
