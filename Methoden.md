Hier ist eine übersichtliche Dokumentation aller Fahr-Methoden deines NasiBot:

---

# 📋 NasiBot Fahr-Methoden – Übersicht

## **Grundlegende Bewegungsmethoden**

### 🔹 `forward()`
```cpp
void forward()
```
**Funktion:** Geradeausfahrt mit PID-Korrektur

**Was passiert:**
- Setzt Motorrichtung auf vorwärts (LV/RV = HIGH)
- Liest aktuelle Yaw-Rate vom MPU6050 aus
- Wendet PID-Regler an, um Geradeausfahrt zu stabilisieren
- Korrigiert automatisch Abweichungen nach links/rechts

**Besonderheit:** 
- Nutzt Gyroskop für präzise Geradeausfahrt
- Motor-Geschwindigkeiten werden dynamisch angepasst

---

### 🔹 `backward()`
```cpp
void backward()
```
**Funktion:** Rückwärtsfahrt mit fester Geschwindigkeit

**Was passiert:**
- Setzt Motorrichtung auf rückwärts (LR/RR = HIGH)
- Beide Motoren laufen mit eingestellter `speed`
- **Keine PID-Korrektur** (einfache Rückwärtsfahrt)
- PWM wird sofort gesetzt (keine Beschleunigung)

**Verwendung:** Beim Ausweichmanöver nach Hinderniserkennung

---

### 🔹 `turnLeft()`
```cpp
void turnLeft()
```
**Funktion:** Drehung auf der Stelle nach links

**Was passiert:**
- Linker Motor: rückwärts (LR = HIGH)
- Rechter Motor: vorwärts (RV = HIGH)
- Geschwindigkeit: `speed * 0.6` (60% der Normalgeschwindigkeit)
- PWM wird sofort gesetzt

**Drehrichtung:** Gegen den Uhrzeigersinn ↺

---

### 🔹 `turnRight()`
```cpp
void turnRight()
```
**Funktion:** Drehung auf der Stelle nach rechts

**Was passiert:**
- Linker Motor: vorwärts (LV = HIGH)
- Rechter Motor: rückwärts (RR = HIGH)
- Geschwindigkeit: `speed * 0.6` (60% der Normalgeschwindigkeit)
- PWM wird sofort gesetzt

**Drehrichtung:** Im Uhrzeigersinn ↻

---

### 🔹 `stop()`
```cpp
void stop()
```
**Funktion:** Kompletter Motorstopp

**Was passiert:**
- Alle Motor-Pins auf LOW
- PWM auf 0 (analogWrite = 0)
- Zielgeschwindigkeiten auf 0 gesetzt
- Aktuelle Geschwindigkeiten auf 0 gesetzt

**Wichtig:** Sofortiger Stopp ohne Auslaufen

---

## **Steuerungs- und Update-Methoden**

### 🔹 `autonomous()`
```cpp
void autonomous()
```
**Funktion:** Hauptlogik für autonome Hinderniserkennung und Navigation

**Ablauf:**
1. **Distanz messen** mit Ultraschallsensor
2. **Wenn Hindernis < minDistance (30cm):**
   - Stopp → Rückwärts → Stopp
   - Servo nach rechts → Distanz messen
   - Servo nach links → Distanz messen
   - Servo zurück zur Mitte
   - Drehe zur Seite mit mehr Platz
   - PID zurücksetzen
3. **Sonst:** Vorwärtsfahrt mit PID-Korrektur

---

### 🔹 `update()`
```cpp
void update()
```
**Funktion:** Motor-Update im Hauptloop

**Was passiert:**
- **DRIVE_ACCELERATED Modus:** 
  - Ruft `updateAcceleration()` auf
  - Sanfte Beschleunigung zu Zielgeschwindigkeit
- **DRIVE_DIRECT Modus:**
  - PWM wird sofort gesetzt
  - Keine Rampe

**Aufruf:** Muss in `loop()` regelmäßig aufgerufen werden!

---

### 🔹 `setDriveMode(DriveMode mode)`
```cpp
void setDriveMode(DriveMode mode)
```
**Funktion:** Fahrmodus wechseln

**Parameter:**
- `DRIVE_DIRECT` – Geschwindigkeit sofort setzen (alte Methode)
- `DRIVE_ACCELERATED` – Sanfte Beschleunigung (empfohlen)

**Beispiel:**
```cpp
bot.setDriveMode(DRIVE_ACCELERATED);
```

---

## **Interne Hilfsmethoden**

### 🔹 `setTargetSpeed(int l, int r)`
```cpp
void setTargetSpeed(int l, int r)
```
**Funktion:** Zielgeschwindigkeiten für linken/rechten Motor setzen

**Parameter:**
- `l` – Linker Motor (0–255)
- `r` – Rechter Motor (0–255)

**Intern:** Werte werden mit `constrain()` begrenzt

---

### 🔹 `updateAcceleration()`
```cpp
void updateAcceleration()
```
**Funktion:** Sanfte Beschleunigung/Verzögerung

**Was passiert:**
- Wird alle `accelInterval` ms (80ms) ausgeführt
- Nähert `currentSpeed` an `targetSpeed` an
- Schrittweise um max. `acceleration` (15 PWM-Einheiten)
- Erzwingt Mindest-PWM (`minPWM = 150`), damit Motoren anlaufen

**Vorteil:** Sanfteres Fahrverhalten, weniger Ruck

---

### 🔹 `applyPIDCorrection(float yawRate)`
```cpp
void applyPIDCorrection(float yawRate)
```
**Funktion:** PID-Regler für Geradeausfahrt

**Eingabe:** Yaw-Rate (Drehgeschwindigkeit in °/s)

**Berechnung:**
```
correction = Kp·error + Ki·∫error·dt + Kd·(derror/dt)
```

**Ausgabe:**
- Korrekturwert wird auf Motorgeschwindigkeiten addiert/subtrahiert
- Positive Yaw-Rate (Drehung rechts) → linker Motor schneller
- Negative Yaw-Rate (Drehung links) → rechter Motor schneller

**Debug:** Gibt Yaw-Rate, Korrektur und Geschwindigkeiten über Serial aus

---

## **Sensor-Methoden**

### 🔹 `getDistance()`
```cpp
long getDistance()
```
**Funktion:** Ultraschall-Distanzmessung

**Rückgabe:** Entfernung in cm (1–400)

**Ablauf:**
1. TRIG-Pin 10µs HIGH-Impuls
2. ECHO-Pin-Dauer messen (max. 30ms Timeout)
3. Umrechnung: `Dauer * 0.034 / 2`
4. Fehlerhafte Werte (≤0 oder >400) → 400cm zurückgeben

---

### 🔹 `getYawRate()`
```cpp
float getYawRate()
```
**Funktion:** Aktuelle Drehgeschwindigkeit auslesen

**Rückgabe:** Yaw-Rate in °/s (kalibriert)

**Ablauf:**
1. MPU6050 Gyro Z-Wert auslesen
2. Umrechnung: `gz / 131.0` (bei ±250°/s Range)
3. Drift-Offset abziehen (`gyroDriftZ`)

---

### 🔹 `calibrateGyro()`
```cpp
void calibrateGyro()
```
**Funktion:** Gyro-Kalibrierung beim Start

**Ablauf:**
1. 300 Messungen durchführen (Roboter muss stillstehen!)
2. Durchschnitt berechnen
3. Als `gyroDriftZ` speichern
4. PID-Variablen zurücksetzen

**Wichtig:** Roboter während Kalibrierung nicht bewegen!

---

## **Debug & Tuning**

### 🔹 `processSerialCommands()`
```cpp
void processSerialCommands()
```
**Funktion:** Live-Tuning über Serial Monitor (115200 Baud)

**Befehle:**
| Befehl | Funktion | Beispiel |
|--------|----------|----------|
| `p` + Zahl | Kp setzen | `p1.5` |
| `i` + Zahl | Ki setzen | `i0.05` |
| `d` + Zahl | Kd setzen | `d0.1` |
| `s` + Zahl | Geschwindigkeit | `s200` |
| `r` | PID zurücksetzen | `r` |
| `k` | Neu-Kalibrierung | `k` |

**Beispiel:**
```
p1.2      → Kp = 1.2
i0.03     → Ki = 0.03
s180      → Speed = 180
r         → PID Reset
```

---

## **Fahrverhalten-Übersicht**

```
┌──────────────────────────────────────────────────┐
│  Methode        Richtung    PID    Geschw.      │
├──────────────────────────────────────────────────┤
│  forward()      Vorwärts    ✓      Dynamisch    │
│  backward()     Rückwärts   ✗      100%         │
│  turnLeft()     Links ↺     ✗      60%          │
│  turnRight()    Rechts ↻    ✗      60%          │
│  stop()         —           ✗      0%           │
│  autonomous()   Gemischt    ✓      Auto         │
└──────────────────────────────────────────────────┘
```

---

## **Typischer Programmablauf**

```cpp
void loop() {
  bot.update();              // 1. Motor-Updates
  bot.autonomous();          // 2. Fahrlogik
  bot.processSerialCommands(); // 3. Optional: Tuning
}
```

**autonomous() ruft intern auf:**
1. `getDistance()` → Hindernis prüfen
2. Bei Hindernis:
   - `stop()` → `backward()` → `stop()`
   - Servo schwenken + `getDistance()`
   - `turnLeft()` oder `turnRight()`
3. Sonst: `forward()` (mit PID)

---

## **Motor-Pin-Logik**

```
Vorwärts:   LV=1, LR=0  |  RV=1, RR=0
Rückwärts:  LV=0, LR=1  |  RV=0, RR=1
Links:      LV=0, LR=1  |  RV=1, RR=0
Rechts:     LV=1, LR=0  |  RV=0, RR=1
Stop:       Alle = 0
```

---

Möchtest du eine bestimmte Methode noch detaillierter erklärt haben? 🤖