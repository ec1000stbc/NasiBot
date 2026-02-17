/****************************************************************************
  2WD LED-Programm mit Objektorientierung

  Klasse: Led
  ------------------------------------
  Methoden:
    Led(int pin)              → Konstruktor, initialisiert Pin & PWM
    void on()                 → LED voll einschalten
    void off()                → LED ausschalten
    void dimm(int val)        → LED dimmen (0–255)
    void blink()              → Blinken mit Standardintervall (500 ms)
    void blink(int ms)        → Blinken mit frei wählbarem Intervall
    void status()             → Gibt aktuellen Zustand (0/1) über Serial aus

  Unterstützt:
    - Hardware-PWM (Pins 3,5,6,9,10,11)
    - Software-PWM (SoftPWM) für andere Pins
******************************************************************************/

#include <SoftPWM.h>
#include <Led_dimm.h>

// Objekte
Led frontLeft(A2);
Led frontRight(A3);
Led topLeft(9);
Led topRight(10);

void setup() {
  Serial.begin(9600);
  SoftPWMBegin();
}

void loop() {
  frontLeft.on();
  frontRight.dimm(10);
  topLeft.blink(500);
  topRight.on();
}
