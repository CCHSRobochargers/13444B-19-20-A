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
// vex::servo RightClaw = vex::servo(Brain.ThreeWirePort.H);
// vex::servo LeftClaw = vex::servo(Brain.ThreeWirePort.B);

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
  int sel = 0;
  bool bLeft = false;
  bool bRight = false;
  bool bUp = false;
  bool bDown = false;

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Select the autonomous routine to run
  Controller1.rumble(rumbleShort);
  wait(50, msec);

  // Select the autonomous routine to run
  /*while (!Controller1.ButtonA.pressing()) {
    if ((!bUp && Controller1.ButtonUp.pressing()) ||
        (!bDown && Controller1.ButtonDown.pressing())) {
      if (sel == 0) {
        sel = 1;
      } else {
        sel = 0;
      }
    }

    if (sel == 0) {
      if ((!bRight && Controller1.ButtonRight.pressing()) ||
          (!bLeft && Controller1.ButtonLeft.pressing())) {
        if (allianceSelect == Blue) {
          allianceSelect = Red;
        } else {
          allianceSelect = Blue;
        }
      }
    }
    if (sel == 1) {
      if (!bRight && Controller1.ButtonRight.pressing()) {
        if (autoSelect == NUM_AUTO - 1) {
          autoSelect = 0;
        } else {
          autoSelect++;
        }
      }
      if (!bLeft && Controller1.ButtonLeft.pressing()) {
        if (autoSelect == 0) {
          autoSelect = NUM_AUTO - 1;
        } else {
          autoSelect--;
        }
      }
    }

    // Update the selection
    Controller1.Screen.clearLine(3);
    wait(50, msec);
    if (sel == 0) {
      Controller1.Screen.print("Alliance: %s", allianceText[allianceSelect]);
    } else {
      Controller1.Screen.print("Auto: %s", autoText[autoSelect]);
    }

    // Save the buttons for press detection
    bUp = Controller1.ButtonUp.pressing();
    bDown = Controller1.ButtonDown.pressing();
    bRight = Controller1.ButtonRight.pressing();
    bLeft = Controller1.ButtonLeft.pressing();

    wait(50, msec);
  }*/

  Controller1.Screen.clearLine(3);
  wait(50, msec);
  Controller1.Screen.print("%s | %s", allianceText[allianceSelect],
                           autoText[autoSelect]);

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
  if (allianceSelect == Blue && autoSelect == 1) {
    Drivetrain.driveFor(24.0 * 3.0, inches); // Three tiles forward
    Drivetrain.turnFor(180, degrees);
    Drivetrain.driveFor(24.0 * 2.0, inches); // Two tiles back
  }
  else if (allianceSelect == Red && autoSelect == 1){
    Drivetrain.driveFor(24.0 * 3.0, inches); // Three tiles forward
    Drivetrain.turnFor(-180, degrees);
    Drivetrain.driveFor(24.0 * 2.0, inches); // Two tiles back
  }
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

void usercontrol(void) {
  // User control code here, inside the loop
  clawR.resetRotation();
  clawL.resetRotation();
  while (1) {
    LeftMotor.spin(vex::directionType::rev, Controller1.Axis2.position(),
                   vex::velocityUnits::pct);
    RightMotor.spin(vex::directionType::rev, Controller1.Axis3.position(),
                    vex::velocityUnits::pct);
    BackLift.spin(vex::directionType::rev, Controller2.Axis2.position()/3.0,
                  vex::velocityUnits::pct);
    FrontLift.spin(vex::directionType::rev, Controller2.Axis3.position()/3.0,
                   vex::velocityUnits::pct);
    if (Controller2.ButtonR1.pressing()) {
      clawL.rotateTo(95, rotationUnits::deg, false);
      clawR.rotateTo(-95, rotationUnits::deg, false);
    } else if (Controller2.ButtonL1.pressing()) {
      clawL.rotateTo(45, rotationUnits::deg, false);
      clawR.rotateTo(-45, rotationUnits::deg, false);
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
