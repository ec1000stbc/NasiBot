#ifndef NASIBOT_H
#define NASIBOT_H

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Autonom.h>
#include <Led.h>
#include <IR_Sensor.h>

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
  void curveLeft();   // sanfte Linkskurve (für Linienfolger)
  void curveRight();  // sanfte Rechtskurve (für Linienfolger)
  void stop();

  // Modus
  void setDriveMode(DriveMode mode);

  // Geschwindigkeit (0–255)
  void setSpeed(int s);

  // Hilfsmethoden
  void wait(unsigned long ms);
  void processSerialCommands();

private:
  /* ===== Hardware ===== */
  Servo servo;
  MPU6050 mpu;

  /* ===== Pins ===== */
  const uint8_t SERVO_PIN = 12;
  const uint8_t LV       = 8;
  const uint8_t LR       = 7;
  const uint8_t RV       = 5;
  const uint8_t RR       = 6;
  const uint8_t L_SPEED  = 11;
  const uint8_t R_SPEED  = 3;
  const uint8_t TRIG     = 4;
  const uint8_t ECHO     = 2;

  /* ===== Bewegung ===== */
  int speed         = 230;
  int servoMid      = 93;
  int servoTurn     = 70;
  int minDistance   = 30;
  float curveRatio  = 0.5;  // Innenspur-Faktor für curveLeft/Right (0.0–1.0)
  MoveStatus currentStatus = STATUS_STOPPED;

  /* ===== PID-Parameter ===== */
  float Kp           = 1.2;
  float Ki           = 0.03;
  float Kd           = 0.08;
  float pidIntegral  = 0.0;
  float pidPrevError = 0.0;
  unsigned long pidPrevTime = 0;
  const int MAX_CORRECTION  = 80;
  const int LOOP_MS         = 10;  // 10ms = 100Hz

  /* ===== Geschwindigkeiten ===== */
  int currentSpeedL = 0;
  int currentSpeedR = 0;
  int targetSpeedL  = 0;
  int targetSpeedR  = 0;
  int minPWM        = 150;

  /* ===== Beschleunigung ===== */
  DriveMode driveMode      = DRIVE_DIRECT;
  int acceleration         = 15;
  unsigned long lastAccelUpdate = 0;
  const int accelInterval  = 80;

  /* ===== MPU6050 ===== */
  float gyroDriftZ = 0.0;

  /* ===== EMA-Filter ===== */
  float yawRateEMA      = 0.0;
  const float EMA_ALPHA = 0.3;

  /* ===== Private Methoden ===== */
  long  getDistance();
  void  calibrateGyro();
  float getYawRate();
  float getYawRateEMA();
  void  setTargetSpeed(int l, int r);
  void  updateAcceleration();
  void  applyPIDCorrection(float yawRate);
};

#endif
