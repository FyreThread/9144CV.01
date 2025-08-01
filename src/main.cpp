#include "autons.hpp"
#include "devices.h"
#include "includes.h"
#include "pros/abstract_motor.hpp"  // IWYU pragma: keep
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subroutines.h"  // IWYU pragma: keep

using namespace my_robot;

#define RED_HUE_MIN 0
#define RED_HUE_MAX 30
#define BLUE_HUE_MIN 180
#define BLUE_HUE_MAX 270

void sortAction() {
  pros::screen::print(pros::E_TEXT_MEDIUM, 0, "Action 1 triggered!");
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

void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
}

void initialize() {
  pros::lcd::initialize();                          // Initialize brain screen
  chassis.calibrate();                              // Calibrate sensors
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);  // Set brake mode for chassis motors

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      {"Tooning\n", high},
      {"High\n", high},
      {"Low\n", low},
      {"Skills\n", skills},
  });

  ez::as::initialize();  // Initialize autonomous selector
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  ez::as::auton_selector.selected_auton_call();  // Calls selected autonomous routine from autonomous selector
}

// OPERATOR CONTROL
pros::Controller controller(pros::E_CONTROLLER_MASTER);

void opcontrol() {
  bool prevButtonBState = false;
  bool will = false;

  bool prevButtonAState = false;
  bool lifter = false;

  bool prevButtonLEFTState = false;
  bool descorer = false;

  chassis.cancelAllMotions();  // Cancel all ongoing motions
  while (true) {
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);    // Get left joystick Y-axis value
    int leftX = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);    // Get left joystick X-axis value
    int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);  // Get right joystick X-axis value

    chassis.arcade(leftY, rightX * 0.7);

    // Check if the robot is not connected to the competition control
    if (!pros::competition::is_connected()) {
      // If both DIGITAL_UP and DIGITAL_LEFT buttons are pressed, run autonomous routine
      if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        autonomous();
      }
    }

    // Check R1/R2 for intake control
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {  // counter
      intake.move(127);
      top_roller.move(127);
      counter_roller.move(-127);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {  // swap main
      intake.move(90);
      top_roller.move(-50);
      counter_roller.move(-127);
    }

    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intake.move(127);
      top_roller.move(127);
      counter_roller.move(127);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intake.move(-80);
      top_roller.move(-127);
      counter_roller.move(-127);
    } else {
      intake.move(0);          // Stop intake motor
      top_roller.move(0);      // Stop top roller motor
      counter_roller.move(0);  // Stop counter roller motor
    }

    bool currentButtonAState = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
    if (currentButtonAState && !prevButtonAState) {
      lifter = !lifter;            // Toggle the state
      liftflap.set_value(lifter);  // Directly set the value
    }

    // Update the previous state after checking the button press
    prevButtonAState = currentButtonAState;

    bool currentButtonBState = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
    if (currentButtonBState && !prevButtonBState) {
      will = !will;           // Toggle the state
      willy.set_value(will);  // Directly set the value
    }

    // Update the previous state after checking the button press
    prevButtonBState = currentButtonBState;

    bool currentButtonLEFTState = master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);
    if (currentButtonBState && !prevButtonBState) {
      descorer = !descorer;         // Toggle the state
      descore.set_value(descorer);  // Directly set the value
    }

    // Update the previous state after checking the button press
    prevButtonLEFTState = currentButtonLEFTState;
  }

  pros::delay(25);  // Delay for the poor IC
}