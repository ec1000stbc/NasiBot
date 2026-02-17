#ifndef AUTONOM_H
#define AUTONOM_H

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>

/* ================= MODI ================= */

enum DriveMode {
  DRIVE_DIRECT,
  DRIVE_ACCELERATED
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
  
  // PID-Tuning (optional über Serial)
  void processSerialCommands();

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

  /* ===== PID-Parameter ===== */
  float Kp = 1.2;           // Proportional
  float Ki = 0.03;          // Integral
  float Kd = 0.08;          // Derivative
  float pidIntegral = 0.0;
  float pidPrevError = 0.0;
  unsigned long pidPrevTime = 0;
  const int MAX_CORRECTION = 80;
  const int LOOP_MS = 10;   // 10ms = 100Hz

  /* ===== Geschwindigkeiten ===== */
  int currentSpeedL = 0;
  int currentSpeedR = 0;
  int targetSpeedL = 0;
  int targetSpeedR = 0;
  int minPWM = 150;

  /* ===== Beschleunigung ===== */
  DriveMode driveMode = DRIVE_DIRECT;
  int acceleration = 15;
  unsigned long lastAccelUpdate = 0;
  const int accelInterval = 80;

  /* ===== MPU6050 Variablen ===== */
  float gyroDriftZ = 0.0;   // Kalibrierter Offset
  
  /* ===== Sensorik ===== */
  long getDistance();
  
  /* ===== MPU-Funktionen ===== */
  void calibrateGyro();
  float getYawRate();       // Gibt die aktuelle Yaw-Rate zurück

  /* ===== Intern ===== */
  void setTargetSpeed(int l, int r);
  void applyMotorSpeed();
  void updateAcceleration();
  void applyPIDCorrection(float yawRate);  // NEU: PID-basierte Korrektur
};

#endif
