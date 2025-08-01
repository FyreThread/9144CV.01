#include "devices.h"      // IWYU pragma: keep
#include "main.h"         // IWYU pragma: keep
#include "pros/misc.hpp"  // IWYU pragma: keep
#include "pros/motors.h"  // IWYU pragma: keep
#include "pros/rtos.hpp"  // IWYU pragma: keep
#include "subroutines.h"  // IWYU pragma: keep

using namespace my_robot;

void intaking() {
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(127);
}

void outtaking() {
  intake.move(-80);
  top_roller.move(-127);
  counter_roller.move(-127);
}

void R1() {
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(-127);
}

void R2() {
  intake.move(90);
  top_roller.move(-50);
  counter_roller.move(-127);
}

pros::v5::Optical colorSort(14);

#define RED_HUE_MIN 10
#define RED_HUE_MAX 20
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
    }
    pros::delay(100);  // Prevent CPU hogging
  }
}
void colorSortRed() {
  extern pros::Optical colorSort;  // Sensor defined in robot-config.cpp
  bool isBlueMode = false;         // Start in blue mode (toggle as needed)

  while (true) {
    double hue = colorSort.get_hue();

    if (!isBlueMode && (hue >= BLUE_HUE_MIN && hue <= BLUE_HUE_MAX)) {
      sortAction();
    }

    pros::delay(100);  // Prevent CPU hogging
  }
}
//
//
//
//
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
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  chassis.moveToPose(0, 3, 0, 2000, {.maxSpeed = 10});
}

void highRedBackup() {
  liftflap.set_value(true);
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  chassis.moveToPose(10, 15, 65, 1100, {.maxSpeed = 60});
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(127);
  chassis.moveToPoint(20.8, 28, 1250, {.maxSpeed = 50});
  // chassis.turnToHeading(140, 1000);
  // chassis.waitUntilDone();
  // chassis.moveToPose(41, -9, 104, 1000, {.maxSpeed = 60});
  // chassis.waitUntilDone();
  // chassis.turnToHeading(120, 1000);
  // chassis.waitUntilDone();
  willy.set_value(true);
  // chassis.moveToPoint(50, -15, 1000, {.maxSpeed = 60});

  // chassis.moveToPose(10, 0, 65, 1100, {.maxSpeed = 60});
  // liftflap.set_value(false);
  // intake.move(127);
  // top_roller.move(127);
  // counter_roller.move(-127);

  pros::delay(100000);

  if (!pros::competition::is_connected()) {
    pros::delay(10000);
  }
}

void highRed() {
  liftflap.set_value(true);
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  chassis.moveToPose(10, 15, 65, 1100, {.maxSpeed = 60});
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(127);
  chassis.moveToPoint(20.8, 28, 1250, {.maxSpeed = 50});
  chassis.turnToHeading(140, 1000);
  chassis.waitUntilDone();
  chassis.moveToPoint(46.35, -9, 2000, {.maxSpeed = 60});
  chassis.waitUntilDone();
  chassis.turnToHeading(0, 800);
  chassis.waitUntilDone();
  // chassis.moveToPoint(28, 8, 1750);
  chassis.arcade(50, 0);
  pros::delay(775);
  chassis.arcade(-25, 0);
  pros::delay(40);
  chassis.arcade(0, 0);
  liftflap.set_value(false);
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(-127);
  pros::delay(10000);

  if (!pros::competition::is_connected()) {
    pros::delay(10000);
  }
}

void lowRed() {
}

void skills() {
}