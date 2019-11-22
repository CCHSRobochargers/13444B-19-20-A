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

#include "field250.h" // Field graphic (250x250 pixels PNG)
#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

vex::motor LeftMotor = vex::motor(vex::PORT3, true);
vex::motor RightMotor = vex::motor(vex::PORT10);
vex::motor liftfrontright = vex::motor(vex::PORT17);
vex::motor liftfrontleft = vex::motor(vex::PORT20, true);
vex::motor liftbackright = vex::motor(vex::PORT14, true);
vex::motor liftbackleft = vex::motor(vex::PORT9);
// vex::motor clawR = vex::motor(vex::PORT19);
// vex::motor clawL = vex::motor(vex::PORT12);
vex::motor Claw = vex::motor(vex::PORT12);

vex::motor_group BackLift(liftbackright, liftbackleft);
vex::motor_group FrontLift(liftfrontright, liftfrontleft);

drivetrain Drivetrain =
    drivetrain(LeftMotor, RightMotor, 4.0 * M_PI, 9.5, 13.0, inches, 1.0);

vex::controller Controller1 = vex::controller(controllerType::primary);
vex::controller Controller2 = vex::controller(controllerType::partner);

#define FIELD_GRAPHIC_WIDTH 250
#define FIELD_GRAPHIC_HEIGHT 250
#define BRAIN_SCREEN_WIDTH 480
#define BRAIN_SCREEN_HEIGHT 272
// Center on the X axis
#define FIELD_LOC_X ((BRAIN_SCREEN_WIDTH / 2) - (FIELD_GRAPHIC_WIDTH / 2))
// Put on the bottom on the Y axis
#define FIELD_LOC_Y ((BRAIN_SCREEN_HEIGHT - 25) - FIELD_GRAPHIC_HEIGHT)

typedef struct {
  const char *label;
  int x;
  int y;
  int width;
  int height;
} hotspotType;

// The (0,0) origin of the screen is top left
#define NUM_AUTO 5
hotspotType autoHotspots[NUM_AUTO] = {
    {"None        ", (FIELD_GRAPHIC_HEIGHT / 2) - 25,
     (FIELD_GRAPHIC_WIDTH / 2) - 25, 50, 50},
    {"4 POINT RED ", 0, 0, 50, 50},
    {"1 POINT     ", 0, FIELD_GRAPHIC_WIDTH - 50, 50, 50},
    {"4 POINT BLUE", FIELD_GRAPHIC_HEIGHT - 50, 0, 50, 50},
    {"1 POINT     ", FIELD_GRAPHIC_HEIGHT - 50, FIELD_GRAPHIC_WIDTH - 50, 50,
     50},
};

// Which autonomous was selected (defaults to "none")
int autoSelect = 0;

// Button pressed (or not) math
bool isPressed(int idx, int X, int Y) {
  if ((X >= autoHotspots[idx].x) &&
      (X <= autoHotspots[idx].x + autoHotspots[idx].width) &&
      (Y >= autoHotspots[idx].y) &&
      (Y <= autoHotspots[idx].y + autoHotspots[idx].width)) {
    return true;
  } else {
    return false;
  }
}
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
  int fingerX;
  int fingerY;

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  // "Accept" button
  Brain.Screen.drawCircle(55, FIELD_LOC_Y + (FIELD_GRAPHIC_HEIGHT / 2), 50,
                          color::green);

  // Set the screen origin to where we want the field grapic. Makes the math
  // easier.
  Brain.Screen.setOrigin(FIELD_LOC_X, FIELD_LOC_Y);

  Brain.Screen.drawImageFromBuffer(field_250_png, 0, 0, field_250_png_len);
  // Draw squares for the buttons
  for (int j = 0; j < NUM_AUTO; j++) {
    Brain.Screen.drawRectangle(autoHotspots[j].x, autoHotspots[j].y,
                               autoHotspots[j].width, autoHotspots[j].height,
                               color::transparent);
  }

  while (true) {
    if (Brain.Screen.pressing()) {
      fingerX = Brain.Screen.xPosition();
      fingerY = Brain.Screen.yPosition();
      // Press to the left of the field graphic to accept
      if (fingerX < -10) {
        break;
      }
      for (int j = 0; j < NUM_AUTO; j++) {
        if (isPressed(j, fingerX, fingerY)) {
          autoSelect = j;
          // Need to reset the origin to print the label under the green button
          Brain.Screen.setOrigin(0, 0);
          Brain.Screen.setCursor(11, 1);
          Brain.Screen.print(autoHotspots[j].label);
          Brain.Screen.setOrigin(FIELD_LOC_X, FIELD_LOC_Y);
        }
      }
    }
    wait(20, msec);
  }

  // Redraw with only the selected routine
  Brain.Screen.drawImageFromBuffer(field_250_png, 0, 0, field_250_png_len);
  Brain.Screen.drawRectangle(
      autoHotspots[autoSelect].x, autoHotspots[autoSelect].y,
      autoHotspots[autoSelect].width, autoHotspots[autoSelect].height,
      color::transparent);

  Brain.Screen.setOrigin(0, 0);
  // Clear the temporary label
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print("          ");
  // Print the selected autonomous on the button
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print(autoHotspots[autoSelect].label);

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
  if (autoSelect == 0) {
    return;
  } else if (autoSelect == 1) {
    BackLift.rotateTo(-100, degrees);
    Drivetrain.turnFor(90, degrees);
    Drivetrain.turnFor(-90, degrees);

    // scores 4 points in (uknown) corner
  } else if (autoSelect == 2) {
    Drivetrain.driveFor(34.0, inches);
    Drivetrain.driveFor(-10, inches);
    // scores 1 point in any corner
  } else if (autoSelect == 3) {
    Drivetrain.driveFor(-9, inches);
     Drivetrain.turnFor(-10, degrees);
    Drivetrain.driveFor(27, inches);
    Drivetrain.turnFor(-90, degrees);
    Drivetrain.driveFor(22, inches);
    Claw.rotateTo(900, degrees, false);
    wait(0.5, sec);
    BackLift.rotateTo(-100, degrees);
    Drivetrain.driveFor(-3, inches);
    Drivetrain.turnFor(90, degrees);
    Drivetrain.driveFor(36, inches);
    Drivetrain.turnFor(90, degrees);
    Drivetrain.driveFor(22, inches);
    BackLift.rotateTo(0, degrees);
    Claw.rotateTo(400, degrees);
    // scores 4 points in (unknown) corner
  } else if (autoSelect == 4) {
    
    // scores 1 point in any corner
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
void clawPinch(void) {
  // clawL.rotateTo(-115, rotationUnits::deg, false);
  // clawR.rotateTo(115, rotationUnits::deg, false);
  Claw.rotateTo(900, rotationUnits::deg, false);
}
void clawOpen(void) {
  // clawL.rotateTo(-33, rotationUnits::deg, false);
  // clawR.rotateTo(33, rotationUnits::deg, false);
  Claw.rotateTo(350, rotationUnits::deg, false);
}
void usercontrol(void) {
  // User control code here, inside the loop
  // clawR.resetRotation();
  // clawL.resetRotation();
  Claw.resetRotation();

  Controller2.ButtonR1.pressed(clawOpen);
  Controller2.ButtonL1.pressed(clawPinch);
  while (1) {
    LeftMotor.spin(vex::directionType::fwd, Controller1.Axis2.position() * 0.75,
                   vex::velocityUnits::pct);
    RightMotor.spin(vex::directionType::fwd,
                    Controller1.Axis3.position() * 0.75,
                    vex::velocityUnits::pct);
    BackLift.spin(vex::directionType::rev, Controller2.Axis2.position() / 2.0,
                  vex::velocityUnits::pct);
    FrontLift.spin(vex::directionType::rev, Controller2.Axis3.position() / 3.0,
                   vex::velocityUnits::pct);

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