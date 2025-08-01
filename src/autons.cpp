#include "devices.h"      // IWYU pragma: keep
#include "main.h"         // IWYU pragma: keep
#include "pros/misc.hpp"  // IWYU pragma: keep
#include "pros/motors.h"  // IWYU pragma: keep
#include "pros/rtos.hpp"  // IWYU pragma: keep
#include "subroutines.h"  // IWYU pragma: keep

using namespace my_robot;
// AUTONOMOUS ROUTINES

void PIDtune() {
}

void high() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);

  // Start route
  chassis.moveToPose(-13.25, -28.5, 31, 1500);  // Move to first mogo
  chassis.waitUntilDone();
}

void low() {
}

void skills() {
}