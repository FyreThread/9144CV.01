#include "devices.h"      // IWYU pragma: keep
#include "main.h"         // IWYU pragma: keep
#include "pros/misc.hpp"  // IWYU pragma: keep
#include "pros/motors.h"  // IWYU pragma: keep
#include "pros/rtos.hpp"  // IWYU pragma: keep
#include "subroutines.h"  // IWYU pragma: keep

using namespace my_robot;

pros::v5::Optical colorSort(14);

#define RED_HUE_MIN 0
#define RED_HUE_MAX 30
#define BLUE_HUE_MIN 180
#define BLUE_HUE_MAX 270

void sortAction() {
  top_roller.move_relative(-600, -127);
}

void colorSortBlue() {
  extern pros::Optical colorSort;  // Sensor defined in robot-config.cpp
  bool isBlueMode = true;          // Start in blue mode (toggle as needed)

  while (true) {
    double hue = colorSort.get_hue();
    if (colorSort.get_proximity() < 50) {  // No object detected
      pros::delay(100);
      continue;
    }

    if (isBlueMode && ((hue >= RED_HUE_MIN && hue <= RED_HUE_MAX) || hue >= 330)) {
      sortAction();
    } else if (!isBlueMode && (hue >= BLUE_HUE_MIN && hue <= BLUE_HUE_MAX)) {
      sortAction();
    }

    pros::delay(100);  // Prevent CPU hogging
  }
}
void colorSortRed() {
  extern pros::Optical colorSort;  // Sensor defined in robot-config.cpp
  bool isBlueMode = false;         // Start in blue mode (toggle as needed)

  while (true) {
    double hue = colorSort.get_hue();

    if (isBlueMode && ((hue >= RED_HUE_MIN && hue <= RED_HUE_MAX) || hue >= 330)) {
      sortAction();
    } else if (!isBlueMode && (hue >= BLUE_HUE_MIN && hue <= BLUE_HUE_MAX)) {
      sortAction();
    }

    pros::delay(100);  // Prevent CPU hogging
  }
}

// AUTONOMOUS ROUTINES

void PIDtune() {
}

void highBlue() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  pros::Task colorSort(colorSortBlue);

  // Start route
  chassis.moveToPose(-13.25, -28.5, 31, 1500);  // Move to first mogo
  chassis.waitUntilDone();
}

void lowBlue() {
}

void highRed() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  pros::Task colorSort(colorSortRed);

  // Start route
  chassis.moveToPose(-13.25, -28.5, 31, 1500);  // Move to first mogo
  chassis.waitUntilDone();
}

void lowRed() {
}

void skills() {
}