// To complete the VEXcode V5 Text project upgrade process, please follow the
// steps below.
//
// 1. You can use the Robot Configuration window to recreate your V5 devices
//   - including any motors, sensors, 3-wire devices, and controllers.
//
// 2. All previous code located in main.cpp has now been commented out. You
//   will need to migrate this code to the new "int main" structure created
//   below and keep in mind any new device names you may have set from the
//   Robot Configuration window.
//
// If you would like to go back to your original project, a complete backup
// of your original (pre-upgraded) project was created in a backup folder
// inside of this project's folder.

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// int main() {
//   // Initializing Robot Configuration. DO NOT REMOVE!
//   vexcodeInit();

// }

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftClaw             servo         B
// RightClaw            servo         H
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

vex::motor LeftMotor = vex::motor(vex::PORT1);
vex::motor RightMotor = vex::motor(vex::PORT10, true);
vex::motor liftfrontright = vex::motor(vex::PORT17);
vex::motor liftfrontleft = vex::motor(vex::PORT20, true);
vex::motor liftbackright = vex::motor(vex::PORT11, true);
vex::motor liftbackleft = vex::motor(vex::PORT9);
vex::motor clawR = vex::motor(vex::PORT19);
vex::motor clawL = vex::motor(vex::PORT12);

vex::motor_group BackLift(liftbackright, liftbackleft);
vex::motor_group FrontLift(liftfrontright, liftfrontleft);

drivetrain Drivetrain =
    drivetrain(LeftMotor, RightMotor, 4.0 * M_PI, 9.5, 13.0, inches, 1.0);

vex::controller Controller1 = vex::controller(controllerType::primary);
vex::controller Controller2 = vex::controller(controllerType::partner);

typedef enum { Red, Blue } allianceSelType;
const char *allianceText[] = {"Red", "Blue"};
allianceSelType allianceSelect = Red;

#define NUM_AUTO 5
const char *autoText[NUM_AUTO] = {"None", "Big", "Small", "Big1", "Small1"};
int autoSelect = 0;
// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Select the autonomous routine to run
  Controller1.rumble(rumbleShort);
  wait(50, msec);

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  Drivetrain.setDriveVelocity(60.0, rpm);
  Drivetrain.setTurnVelocity(60.0, rpm);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  Drivetrain.driveFor(42.0, inches);
  clawL.rotateTo(45.0, rotationUnits::deg, false);
  clawR.rotateTo(-45.0, rotationUnits::deg, false);
  Drivetrain.turnFor(-90, degrees);
  Drivetrain.driveFor(4.0, inches);
  clawL.rotateTo(90, rotationUnits::deg, false);
  clawR.rotateTo(-90, rotationUnits::deg, false);
  Drivetrain.driveFor(-4.0, inches);
  Drivetrain.turnFor(-90, degrees);
  Drivetrain.driveFor(34.0, inches);
  Drivetrain.turnFor(-90, degrees);
  Drivetrain.driveFor(16.0, inches);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void clawPinch(void) {
  clawL.rotateTo(90, rotationUnits::deg, false);
  clawR.rotateTo(90, rotationUnits::deg, false);
}
void clawOpen(void) {
  clawL.rotateTo(0, rotationUnits::deg, false);
  clawR.rotateTo(0, rotationUnits::deg, false);
}
void usercontrol(void) {
  // User control code here, inside the loop
  clawR.resetRotation();
  clawL.resetRotation();
  Controller2.ButtonR1.pressed(clawOpen);
  Controller2.ButtonL1.pressed(clawPinch);
  while (1) {
    LeftMotor.spin(vex::directionType::rev, Controller1.Axis2.position(),
                   vex::velocityUnits::pct);
    RightMotor.spin(vex::directionType::rev, Controller1.Axis3.position(),
                    vex::velocityUnits::pct);
    BackLift.spin(vex::directionType::rev, Controller2.Axis2.position() / 3.0,
                  vex::velocityUnits::pct);
    FrontLift.spin(vex::directionType::rev, Controller2.Axis3.position() / 3.0,
                   vex::velocityUnits::pct);
  }

  // This is the main execution loop for the user control program.
  // Each time through the loop your program should update motor + servo
  // values based on feedback from the joysticks.

  // ........................................................................
  // Insert user code here. This is where you use the joystick values to
  // update your motors, etc.
  // ........................................................................

  wait(20, msec); // Sleep the task for a short amount of time to
                  // prevent wasted resources.
}


//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}