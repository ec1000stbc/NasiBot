#ifndef NASIBOT_H
#define NASIBOT_H

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <MPU6050_tockn.h>

/* ================= MODI ================= */

enum DriveMode {
  DRIVE_DIRECT,      // sofortige PWM (altes Verhalten)
  DRIVE_ACCELERATED  // sanfte Beschleunigung
};

/* ================= KLASSE ================= */

class NasiBot {
public:
  NasiBot();

  void begin();
  void update();
  void autonomous();

  // Bewegung
  void forward();
  void backward();
  void turnLeft();
  void turnRight();
  void stop();

  // Modus
  void setDriveMode(DriveMode mode);

private:
  /* ===== Hardware ===== */
  Servo servo;
  MPU6050 mpu;

  /* ===== Pins ===== */
  const uint8_t SERVO_PIN = 12;
  const uint8_t LV = 8;
  const uint8_t LR = 7;
  const uint8_t RV = 5;
  const uint8_t RR = 6;
  const uint8_t L_SPEED = 11;
  const uint8_t R_SPEED = 3;
  const uint8_t TRIG = 4;
  const uint8_t ECHO = 2;

  /* ===== Bewegung ===== */
  int speed = 230;
  int servoMid = 93;
  int servoTurn = 70;
  int minDistance = 30;
  float diffFactor = 4.0;

  /* ===== Geschwindigkeiten ===== */
  int currentSpeedL = 0;
  int currentSpeedR = 0;
  int targetSpeedL = 0;
  int targetSpeedR = 0;
  int minPWM = 50;   // Startwert – evtl. feinjustieren

  /* ===== Beschleunigung ===== */
  DriveMode driveMode = DRIVE_DIRECT;
  int acceleration = 10;
  unsigned long lastAccelUpdate = 0;
  const int accelInterval = 80;

  /* ===== Sensorik ===== */
  long getDistance();

  /* ===== Intern ===== */
  void setTargetSpeed(int l, int r);
  void applyMotorSpeed();
  void updateAcceleration();
};

#endif
