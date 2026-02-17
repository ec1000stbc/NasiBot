#include <autonom.h>

/* ================= KONSTRUKTOR ================= */

NasiBot::NasiBot() : mpu(Wire, 0.1, 0.9) {}

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
  mpu.begin();
  mpu.calcGyroOffsets();

  stop();
}

/* ================= MODUS ================= */

void NasiBot::setDriveMode(DriveMode mode) {
  driveMode = mode;
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

    delay(200);
    stop();
    delay(500);
    // MPU update mehrere Male in kurzer Schleife
    for (int i = 0; i < 5; i++) {
        mpu.update();
        delay(10);
    }
  } else {
    forward();
  }
}


/* ================= FAHREN ================= */

void NasiBot::forward() {
  mpu.update();
  float z = mpu.getGyroAngleZ();

  int l = speed;
  int r = speed;

  if (z > 0) r -= diffFactor * z;
  else if (z < 0) l += diffFactor * z;

  setTargetSpeed(l, r);

  digitalWrite(LV, HIGH);
  digitalWrite(LR, LOW);
  digitalWrite(RV, HIGH);
  digitalWrite(RR, LOW);
}

void NasiBot::backward() {
  setTargetSpeed(speed, speed);

  digitalWrite(LV, LOW);
  digitalWrite(LR, HIGH);
  digitalWrite(RV, LOW);
  digitalWrite(RR, HIGH);
}

void NasiBot::turnLeft() {
  int turnSpeed = speed * 0.6;
  setTargetSpeed(turnSpeed, turnSpeed);

  // linker Motor rückwärts
  digitalWrite(LV, LOW);
  digitalWrite(LR, HIGH);

  // rechter Motor vorwärts
  digitalWrite(RV, HIGH);
  digitalWrite(RR, LOW);
}

void NasiBot::turnRight() {
  int turnSpeed = speed * 0.6;
  setTargetSpeed(turnSpeed, turnSpeed);

  // linker Motor vorwärts
  digitalWrite(LV, HIGH);
  digitalWrite(LR, LOW);

  // rechter Motor rückwärts
  digitalWrite(RV, LOW);
  digitalWrite(RR, HIGH);
}

void NasiBot::stop() {
  setTargetSpeed(0, 0);

  digitalWrite(LV, LOW);
  digitalWrite(LR, LOW);
  digitalWrite(RV, LOW);
  digitalWrite(RR, LOW);
}

/* ================= GESCHWINDIGKEIT ================= */

void NasiBot::setTargetSpeed(int l, int r) {
  targetSpeedL = constrain(l, 0, 255);
  targetSpeedR = constrain(r, 0, 255);
}


void NasiBot::updateAcceleration() {
  if (millis() - lastAccelUpdate < accelInterval) return;
  lastAccelUpdate = millis();

  // Ziel annähern
  currentSpeedL += constrain(targetSpeedL - currentSpeedL, -acceleration, acceleration);
  currentSpeedR += constrain(targetSpeedR - currentSpeedR, -acceleration, acceleration);

  // Mindest-PWM erzwingen (nur wenn Ziel > 0)
  int pwmL = currentSpeedL;
  int pwmR = currentSpeedR;

  if (targetSpeedL > 0 && pwmL < minPWM) pwmL = minPWM;
  if (targetSpeedR > 0 && pwmR < minPWM) pwmR = minPWM;

  // Stop wirklich = 0
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
