#include <Nasibot.h>

/* ================= KONSTRUKTOR ================= */

NasiBot::NasiBot() {}

/* ================= SETUP ================= */

void NasiBot::begin() {
  pinMode(LV, OUTPUT);
  pinMode(LR, OUTPUT);
  pinMode(RV, OUTPUT);
  pinMode(RR, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(servoMid);

  Wire.begin();
  Wire.setClock(400000);  // 400kHz I²C-Takt

  Serial.println(F("MPU6050 wird initialisiert..."));

  // MPU6050 manuell aufwecken
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);  // PWR_MGMT_1 Register
  Wire.write(0);     // Sleep-Bit löschen → MPU aufwecken
  Wire.endTransmission(true);
  delay(100);

  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println(F("FEHLER: MPU6050 antwortet nicht!"));
    Serial.println(F("Versuche trotzdem fortzufahren..."));
  } else {
    Serial.println(F("MPU6050 OK"));
  }

  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

  calibrateGyro();

  pidPrevTime = millis();

  stop();

  Serial.println(F("\n=== NasiBot bereit! ==="));
  Serial.println(F("Serial-Befehle: p/i/d + Zahl, r=Reset, k=Kalib"));
}

/* ================= KALIBRIERUNG ================= */

void NasiBot::calibrateGyro() {
  const int CAL_SAMPLES = 300;
  Serial.print(F("Kalibrierung ("));
  Serial.print(CAL_SAMPLES);
  Serial.println(F(" Samples) – Roboter still halten!"));

  long sumGZ = 0;
  for (int i = 0; i < CAL_SAMPLES; i++) {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    sumGZ += gz;
    if (i % 50 == 0) Serial.print(F("."));
    delay(5);
  }

  gyroDriftZ = (sumGZ / (float)CAL_SAMPLES) / 131.0;

  Serial.println();
  Serial.print(F("Drift-Offset Z: "));
  Serial.print(gyroDriftZ, 4);
  Serial.println(F(" °/s"));

  pidIntegral  = 0.0;
  pidPrevError = 0.0;
}

/* ================= YAW-RATE AUSLESEN ================= */

float NasiBot::getYawRate() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  return (gz / 131.0) - gyroDriftZ;
}

/* ================= YAW-RATE MIT EMA-FILTER ================= */

float NasiBot::getYawRateEMA() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float yawRate = (gz / 131.0) - gyroDriftZ;
  yawRateEMA = (EMA_ALPHA * yawRate) + ((1.0 - EMA_ALPHA) * yawRateEMA);
  return yawRateEMA;
}

/* ================= PID-KORREKTUR ================= */

void NasiBot::applyPIDCorrection(float yawRate) {
  unsigned long now = millis();
  float dt = (now - pidPrevTime) / 1000.0;

  if (dt < (LOOP_MS / 1000.0)) return;
  pidPrevTime = now;

  float error = yawRate;

  pidIntegral += error * dt;
  pidIntegral = constrain(pidIntegral, -50.0, 50.0);

  float derivative = (error - pidPrevError) / dt;
  pidPrevError = error;

  float correction = (Kp * error) + (Ki * pidIntegral) + (Kd * derivative);
  correction = constrain(correction, -MAX_CORRECTION, MAX_CORRECTION);

  int speedL = constrain(speed + (int)correction, 0, 255);
  int speedR = constrain(speed - (int)correction, 0, 255);

  setTargetSpeed(speedL, speedR);

  static unsigned long lastPrint = 0;
  if (now - lastPrint > 100) {
    lastPrint = now;
    Serial.print(F("Yaw: ")); Serial.print(yawRate, 2);
    Serial.print(F("\tCorr: ")); Serial.print(correction, 2);
    Serial.print(F("\tL: ")); Serial.print(speedL);
    Serial.print(F("\tR: ")); Serial.println(speedR);
  }
}

/* ================= MODUS ================= */

void NasiBot::setDriveMode(DriveMode mode) {
  driveMode = mode;
}

void NasiBot::setSpeed(int s) {
  speed = constrain(s, 0, 255);
}

/* ================= LOOP ================= */

void NasiBot::update() {
  if (driveMode == DRIVE_ACCELERATED) {
    updateAcceleration();
  } else {
    analogWrite(L_SPEED, targetSpeedL);
    analogWrite(R_SPEED, targetSpeedR);
    currentSpeedL = targetSpeedL;
    currentSpeedR = targetSpeedR;
  }
}

/* ================= AUTONOM ================= */

void NasiBot::autonomous() {
  long distance = getDistance();

  if (distance < minDistance) {
    stop();
    delay(800);

    backward();
    delay(250);
    stop();
    delay(1000);

    servo.write(servoMid - servoTurn);
    delay(800);
    long distRight = getDistance();
    delay(100);

    servo.write(servoMid + servoTurn);
    delay(800);
    long distLeft = getDistance();
    delay(100);

    servo.write(servoMid);
    delay(800);

    if (distLeft > distRight) {
      turnLeft();
    } else {
      turnRight();
    }

    delay(300);
    stop();
    delay(800);

    // Nach Drehung: PID zurücksetzen
    pidIntegral  = 0.0;
    pidPrevError = 0.0;
    pidPrevTime  = millis();

  } else {
    forward();
  }
}

/* ================= FAHREN ================= */

void NasiBot::forward() {
  currentStatus = STATUS_FORWARD;
  digitalWrite(LV, HIGH);
  digitalWrite(LR, LOW);
  digitalWrite(RV, HIGH);
  digitalWrite(RR, LOW);

  float yawRate = getYawRateEMA();
  applyPIDCorrection(yawRate);
}

void NasiBot::backward() {
  currentStatus = STATUS_BACKWARD;
  setTargetSpeed(speed, speed);

  analogWrite(L_SPEED, speed);
  analogWrite(R_SPEED, speed);
  currentSpeedL = speed;
  currentSpeedR = speed;

  digitalWrite(LV, LOW);
  digitalWrite(LR, HIGH);
  digitalWrite(RV, LOW);
  digitalWrite(RR, HIGH);
}

void NasiBot::turnLeft() {
  currentStatus = STATUS_TURN_LEFT;
  int turnSpeed = speed * 0.6;
  setTargetSpeed(turnSpeed, turnSpeed);

  analogWrite(L_SPEED, turnSpeed);
  analogWrite(R_SPEED, turnSpeed);
  currentSpeedL = turnSpeed;
  currentSpeedR = turnSpeed;

  digitalWrite(LV, LOW);
  digitalWrite(LR, HIGH);
  digitalWrite(RV, HIGH);
  digitalWrite(RR, LOW);
}

void NasiBot::turnRight() {
  currentStatus = STATUS_TURN_RIGHT;
  int turnSpeed = speed * 0.6;
  setTargetSpeed(turnSpeed, turnSpeed);

  analogWrite(L_SPEED, turnSpeed);
  analogWrite(R_SPEED, turnSpeed);
  currentSpeedL = turnSpeed;
  currentSpeedR = turnSpeed;

  digitalWrite(LV, HIGH);
  digitalWrite(LR, LOW);
  digitalWrite(RV, LOW);
  digitalWrite(RR, HIGH);
}

void NasiBot::curveLeft() {
  currentStatus = STATUS_TURN_LEFT;
  int outer = (int)(speed * curveRatio * 4);
  int inner = (int)(speed * curveRatio);  // 0.0 → Innenrad stoppt

  digitalWrite(LV, HIGH);
  digitalWrite(LR, LOW);
  digitalWrite(RV, HIGH);
  digitalWrite(RR, LOW);

  analogWrite(L_SPEED, inner);
  analogWrite(R_SPEED, outer);
  setTargetSpeed(inner, outer);
}

void NasiBot::curveRight() {
  currentStatus = STATUS_TURN_RIGHT;
  int outer = (int)(speed * curveRatio * 4);
  int inner = (int)(speed * curveRatio);  // 0.0 → Innenrad stoppt

  digitalWrite(LV, HIGH);
  digitalWrite(LR, LOW);
  digitalWrite(RV, HIGH);
  digitalWrite(RR, LOW);

  analogWrite(L_SPEED, outer);
  analogWrite(R_SPEED, inner);
  setTargetSpeed(outer, inner);
}

void NasiBot::stop() {
  currentStatus = STATUS_STOPPED;
  setTargetSpeed(0, 0);
  currentSpeedL = 0;
  currentSpeedR = 0;
  analogWrite(L_SPEED, 0);
  analogWrite(R_SPEED, 0);

  digitalWrite(LV, LOW);
  digitalWrite(LR, LOW);
  digitalWrite(RV, LOW);
  digitalWrite(RR, LOW);
}

void NasiBot::wait(unsigned long ms) {
  unsigned long start = millis();

  while (millis() - start < ms) {
    update();

    if (currentStatus == STATUS_FORWARD) {
      float yawRate = getYawRateEMA();
      applyPIDCorrection(yawRate);
    }

    processSerialCommands();
    delay(1);
  }
}

/* ================= GESCHWINDIGKEIT ================= */

void NasiBot::setTargetSpeed(int l, int r) {
  targetSpeedL = constrain(l, 0, 255);
  targetSpeedR = constrain(r, 0, 255);
}

void NasiBot::updateAcceleration() {
  if (millis() - lastAccelUpdate < accelInterval) return;
  lastAccelUpdate = millis();

  currentSpeedL += constrain(targetSpeedL - currentSpeedL, -acceleration, acceleration);
  currentSpeedR += constrain(targetSpeedR - currentSpeedR, -acceleration, acceleration);

  int pwmL = currentSpeedL;
  int pwmR = currentSpeedR;

  if (targetSpeedL > 0 && pwmL < minPWM) pwmL = minPWM;
  if (targetSpeedR > 0 && pwmR < minPWM) pwmR = minPWM;

  if (targetSpeedL == 0) pwmL = 0;
  if (targetSpeedR == 0) pwmR = 0;

  analogWrite(L_SPEED, pwmL);
  analogWrite(R_SPEED, pwmR);
}

/* ================= DISTANZ ================= */

long NasiBot::getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  long distance = duration * 0.034 / 2;

  if (distance <= 0 || distance > 400) distance = 400;
  return distance;
}

/* ================= SERIAL COMMANDS ================= */

void NasiBot::processSerialCommands() {
  if (!Serial.available()) return;

  String input = Serial.readStringUntil('\n');
  input.trim();
  if (input.length() == 0) return;

  char  cmd = input.charAt(0);
  float val = input.substring(1).toFloat();

  switch (cmd) {
    case 'p': case 'P':
      Kp = val;
      Serial.print(F("Kp = ")); Serial.println(Kp);
      break;
    case 'i': case 'I':
      Ki = val;
      Serial.print(F("Ki = ")); Serial.println(Ki);
      break;
    case 'd': case 'D':
      Kd = val;
      Serial.print(F("Kd = ")); Serial.println(Kd);
      break;
    case 's': case 'S':
      speed = constrain((int)val, 0, 255);
      Serial.print(F("Speed = ")); Serial.println(speed);
      break;
    case 'r': case 'R':
      pidIntegral  = 0.0;
      pidPrevError = 0.0;
      Serial.println(F("PID zurückgesetzt"));
      break;
    case 'k': case 'K':
      stop();
      Serial.println(F("Neu-Kalibrierung..."));
      calibrateGyro();
      break;
    default:
      Serial.println(F("Befehle: p/i/d/s + Zahl, r=Reset, k=Kalib"));
  }
}
