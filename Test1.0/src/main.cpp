#include "vex.h"
#include <cmath>
#include <string>

using namespace vex;

#define DUMPING_ANGLE 97
#define DUMPING_PAUSE 800

// A global instance of competition
competition Competition;

int toggleFlying() {
  int targetV = 400;

  shooter.spin(forward, 9, voltageUnits::volt);
  while (fabs(shooter.velocity(rpm)) < targetV) {
    wait(10, msec);
  }
  indexer.spinTo(90, degrees, 20, velocityUnits::pct);
  // wait(100, msec);
  indexer.spinTo(0, degrees, 100, velocityUnits::pct);
  while (fabs(shooter.velocity(rpm)) < targetV) {
    wait(10, msec);
  }
  indexer.spinTo(90, degrees, 20, velocityUnits::pct);
  // wait(100, msec);
  indexer.spinTo(0, degrees);
  shooter.stop(coast);
  return 0;
}

bool isFeeding = false;

void toggleFeeding() {
  if (!isFeeding) {
    roller.spin(forward, 100, percent);
  } else {
    roller.stop(coast);
  }
  isFeeding = !isFeeding;
}

/* void toggleRoller() {
  roller.spin(forward, 20, percent);
  waitUntil(!Controller1.ButtonB.pressing());
  roller.stop(brake);
} */

const int program_color = blue;

void opticalRoller() {
  roller.spin(forward, 20, percent);

  waitUntil(rollerOptical.isNearObject() && rollerOptical.color() == red);
  roller.stop(brake);
}

/*

void onBackBumperPressed() {
  if (!backHookOn) {
    Drivetrain.stop();
    toggleBackHook();
    Controller1.rumble("..");
  }
}
 */
void resetMotors() {
  leftMotorA.resetPosition();
  leftMotorB.resetPosition();
  rightMotorA.resetPosition();
  rightMotorB.resetPosition();
  indexer.setRotation(0, degrees);
}
/*

void dumpRings(int count, int pause = DUMPING_PAUSE) {
  double d = frontkDistance.objectDistance(inches);
  Drivetrain.driveFor(forward, d - 3, inches, 20, velocityUnits::pct);

}

void driveBackHook(double distance, double speed, bool try_on = true) {
  Drivetrain.driveFor(reverse, distance, inches, speed, velocityUnits::pct,
                      false);
  while (!backHookOn) {
    if (Drivetrain.isDone())
      break;
    wait(50, msec);
  }
  if (try_on && !backHookOn)
    toggleBackHook();
}


void solo() {
  Drivetrain.setHeading(270, degrees);
}

*/
int auton = 0;

void menu() {
  std::string txt[] = {"solo easy", "solo hard", "roller easy", "roller hard"};
  int auton_num = sizeof(txt) / sizeof(txt[0]);

  while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(program_color);
    Brain.Screen.drawRectangle(20, 40, 120, 140);
    Brain.Screen.setPenColor(white);
    Brain.Screen.printAt(30, 70, false, "Prev");

    Brain.Screen.setFillColor(green);
    Brain.Screen.drawRectangle(180, 40, 120, 140);
    Brain.Screen.setPenColor(black);
    Brain.Screen.printAt(190, 70, false, "%s", txt[auton].c_str());

    Brain.Screen.setFillColor(program_color);
    Brain.Screen.drawRectangle(340, 40, 120, 140);
    Brain.Screen.setPenColor(white);
    Brain.Screen.printAt(350, 70, false, "Next");

    while (!(Brain.Screen.pressing())) {
    }
    int x = Brain.Screen.xPosition();
    if (x > 320) {
      auton = (auton + 1) % auton_num;
      while (Brain.Screen.pressing()) {
      }
      continue;
    }

    else if (x < 160) {
      auton = (auton - 1 + auton_num) % auton_num;
      while (Brain.Screen.pressing()) {
      }
      continue;
    } else {
      Brain.Screen.clearScreen();
      break;
    }
  }
}

void setupRobot() {
  resetMotors();
  roller.setBrake(brake);
  /*
  arm.setBrake(hold);
   arm.setTimeout(3, seconds);
   backBumper.pressed(onBackBumperPressed);
   */
}

void solo_hard(){};

void roller_easy() {
  slideFor(-1, 50, false);
  opticalRoller();
  slideFor(7, 40);
  Drivetrain.turnFor(-12, degrees, 70, velocityUnits::pct, false);
  toggleFlying();
}

//robot setup: connector cable to screw align

void solo_easy() {
  Drivetrain.setHeading(0, degrees);
  roller_easy();
  quickTurn(-34, 50);

  // Drivetrain.turnFor(-33, degrees, 50, velocityUnits::pct);
  slideFor(-4.8, 40);

  Drivetrain.driveFor(-130, inches, 75, velocityUnits::pct);
  wait(800, msec);
  turnWest();
  // turnWest();

  double d = backDistance.objectDistance(inches);
  while (d < 25 || d > 40) {
    d = backDistance.objectDistance(inches);
  }

  Drivetrain.driveFor(23.8 - d, inches, 40, velocityUnits::pct);
 // turnWest();
d = sideDistance.objectDistance(inches);
  slideFor(1.5 - d, 40, false);
  opticalRoller();

  //  Drivetrain.turnFor(-45, degrees, 50, velocityUnits::pct);
};
void roller_hard(){
  Drivetrain.turnFor(19, degrees, 70, velocityUnits::pct, false);
  toggleFlying(); 
   Drivetrain.turnFor(-19, degrees, 70, velocityUnits::pct);
   Drivetrain.driveFor(-24, inches, 50, velocityUnits::pct);
   wait(800, msec);
slideFor(-8.5, 30, false);
opticalRoller();
};

void autonomous(void) {

  switch (auton) {
  case 0:
    solo_easy();
    break;
  case 1:
    solo_hard();
    break;
  case 2:
    roller_easy();
    break;
  case 3:
    roller_hard();
    break;
  }
}

bool armMotorStopped = true;
bool rollerMotorStopped = true;

void usercontrol(void) {

  // Drivetrain.setHeading(90, degrees);
  wait(3, seconds);

  Controller1.ButtonLeft.pressed(turnWest);
  Controller1.ButtonRight.pressed(turnEast);
  Controller1.ButtonUp.pressed(turnNorth);
  Controller1.ButtonDown.pressed(turnSouth);
  // Controller1.ButtonA.pressed(toggleFlying);
  Controller1.ButtonB.pressed(toggleFeeding);

  Controller1.ButtonY.pressed(opticalRoller);

  Controller1.rumble(".");

  // roller_easy();
 roller_hard();
  //solo_easy();
  // Drivetrain.setStopping(hold);
  while (true) {

    // check the ButtonR1/ButtonR2 status to control arm
    /*if (Controller1.ButtonR1.pressing()) {
      double angle = arm.position(degrees);
      if (angle > 5) {
        arm.spin(reverse);
        armMotorStopped = false;
      } else if (!armMotorStopped) {
        arm.stop();
        armMotorStopped = true;
      }
    } else if (Controller1.ButtonR2.pressing()) {
      double angle = arm.position(degrees);
      if (angle < 200) {
        arm.spin(forward);
        armMotorStopped = false;
      } else if (!armMotorStopped) {
        arm.stop();
        armMotorStopped = true;
      }

    } else if (!armMotorStopped) {
      arm.stop();
      armMotorStopped = true;
    } */

    if (Controller1.ButtonX.pressing()) {
      roller.spin(forward, 20, percent);
      rollerMotorStopped = false;
    } else if (!rollerMotorStopped) {
      roller.stop();
      rollerMotorStopped = true;
    }
    wait(20, msec);
  }
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // menu();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

int main() {

  setupRobot();

  // Run the pre-autonomous function.

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}