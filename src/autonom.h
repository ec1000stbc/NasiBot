#ifndef AUTONOM_H
#define AUTONOM_H

/* ================= MODI ================= */

enum DriveMode {
  DRIVE_DIRECT,
  DRIVE_ACCELERATED
};

/* ================= STATUS ================= */

enum MoveStatus {
  STATUS_STOPPED,
  STATUS_FORWARD,
  STATUS_BACKWARD,
  STATUS_TURN_LEFT,
  STATUS_TURN_RIGHT
};

#endif
