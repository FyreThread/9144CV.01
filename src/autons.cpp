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
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  chassis.turnToHeading(90, 1000000);
  chassis.waitUntilDone();
  if (!pros::competition::is_connected()) {
    pros::delay(10000);
  }
}

void leftBlue() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  liftflap.set_value(true);
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(127);
  chassis.moveToPose(-4, 10, -30, 300);
  chassis.moveToPose(-10, 17, -47, 1100);
  chassis.waitUntilDone();
  chassis.moveToPose(-13.18, 17.86, -59.62, 500);
  chassis.waitUntilDone();
  chassis.turnToHeading(-43.89, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(-17.75, 23.29, -44.72, 800);
  chassis.waitUntilDone();
  chassis.turnToHeading(43.00, 600);
  chassis.waitUntilDone();
  chassis.moveToPose(-8.5, 28.8, 43, 1000);
  chassis.waitUntilDone();
  liftflap.set_value(false);
  intake.move(127);
  top_roller.move(39);
  counter_roller.move(-127);
  pros::delay(3000);
  intake.move(0);
  top_roller.move(0);
  counter_roller.move(0);
  chassis.turnToHeading(231.90, 600);
  chassis.waitUntilDone();
  chassis.moveToPose(-41.60, -1.85, 226, 1200);
  chassis.waitUntilDone();
  chassis.turnToHeading(176.14, 600);
  chassis.waitUntilDone();
  chassis.moveToPose(-41.19, -12.39, 176, 700);
  chassis.waitUntilDone();
  willy.set_value(true);
  chassis.moveToPoint(-40.68, -18.54, 700);

  if (!pros::competition::is_connected()) {
    pros::delay(10000);
  }
}

void rightBlue() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  chassis.moveToPose(0, 3, 0, 2000, {.maxSpeed = 10});
}

void highRedBackup() {
  liftflap.set_value(true);
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  chassis.moveToPose(5.9, 12, 33, 120);
  chassis.waitUntilDone();
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(127);
  chassis.moveToPose(12, 16.5, 50, 900);
  chassis.waitUntilDone();
  chassis.moveToPose(13.6, 15.6, 55.5, 900);
  chassis.waitUntilDone();
  chassis.turnToHeading(40, 1000);

  // willy.set_value(true);

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

void leftRed() {
  // liftflap.set_value(true);
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  chassis.moveToPose(23.75, 34.25, 90, 3000, {.maxSpeed = 75});
  chassis.waitUntilDone();
  intake.move(127);
  top_roller.move(127);
  counter_roller.move(-127);
  pros::delay(1200);
  intake.move(0);
  top_roller.move(0);
  counter_roller.move(-0);
  chassis.moveToPoint(14.25, 34.25, 1300, {.forwards = false, .maxSpeed = 100});
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 1000);
  willy.set_value(true);

  if (!pros::competition::is_connected())
    pros::delay(10000);
}

void skills() {
}