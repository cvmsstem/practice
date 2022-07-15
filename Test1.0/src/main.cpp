#include "vex.h"
#include <cmath>
#include <string>

using namespace vex;

#define ALLIANCE_COLOR red

// A global instance of competition
competition Competition;

// double shot
void shootDisks(int speed = 400, int voltage = 9) {
  shooter.spin(forward, voltage, voltageUnits::volt);
  while (fabs(shooter.velocity(rpm)) < speed) {
    wait(10, msec);
  }
  indexer.spinTo(90, degrees, 20, velocityUnits::pct);
  indexer.spinTo(0, degrees, 100, velocityUnits::pct);
  while (fabs(shooter.velocity(rpm)) < speed) {
    wait(10, msec);
  }
  indexer.spinTo(90, degrees, 20, velocityUnits::pct);
  indexer.spinTo(0, degrees);
  shooter.stop(coast);
}

void shootStraight() { shootDisks(390, 10); }

void pivotSpin(double d, int s, bool wait_commplete) {
  rightMotorA.spinFor(d, deg, s, velocityUnits::pct, false);
  rightMotorB.spinFor(d, deg, s, velocityUnits::pct, wait_commplete);
}

void shootFeed() {
  pivotSpin(290, 50, false);
  shootDisks(390, 11);
  wait(300, msec);
  pivotSpin(-290, 50, true);
  slideFor(-2, 30);
}

bool isFeeding = false;
void toggleFeeding() {
  if (!isFeeding) {
    roller.spin(reverse, 100, percent);
  } else {
    roller.stop(coast);
  }
  isFeeding = !isFeeding;
}

void opticalRoller() {
  roller.spin(forward, 20, percent);
  waitUntil(rollerOptical.isNearObject() &&
            rollerOptical.color() == ALLIANCE_COLOR);
  roller.stop(brake);
}

// robot setup: align to the roller, not touching.
void roller_easy() {
  Drivetrain.setHeading(270, degrees);
  slideFor(-1, 50, false);
  opticalRoller();
  slideFor(7, 40);
  Drivetrain.turnFor(-12, degrees, 70, velocityUnits::pct, false);
  shootDisks(392, 10);
  turnWest();

  double d = backDistance.objectDistance(inches);
  Drivetrain.driveFor(reverse, 41.5, inches, 30, velocityUnits::pct);
  d = sideDistance.objectDistance(inches);
  slideFor(-d, 30);
}

// robot setup: align to tile edges and back a little bit.
void roller_hard() {
  Drivetrain.setHeading(180, degrees);
  Drivetrain.turnFor(19, degrees, 70, velocityUnits::pct, false);
  shootDisks(392, 11);
  Drivetrain.turnFor(-19, degrees, 75, velocityUnits::pct);
  Drivetrain.driveFor(-24, inches, 50, velocityUnits::pct);
  wait(500, msec);
  slideFor(-8.5, 40, false);
  opticalRoller();
  slideFor(5, 50);
  Drivetrain.driveFor(100, inches, 100, velocityUnits::pct);
  wait(300, msec);
  // read distance
  turnWest();
  Drivetrain.driveFor(60, inches, 100, velocityUnits::pct);
  wait(300, msec);
  turnWest();

  double d = sideDistance.objectDistance(inches);
  slideFor(-d, 50);
};

// robot setup: connector cable to screw align
void solo() {
  Drivetrain.setHeading(0, degrees);
  roller_easy();
  quickTurn(-34, 50);
  // Drivetrain.turnFor(-33, degrees, 50, velocityUnits::pct);
  slideFor(-4.8, 40);

  Drivetrain.driveFor(-130, inches, 75, velocityUnits::pct);
  wait(800, msec);
  turnWest();

  double d = backDistance.objectDistance(inches);
  while (d < 25 || d > 40) {
    d = backDistance.objectDistance(inches);
  }

  Drivetrain.driveFor(23.8 - d, inches, 40, velocityUnits::pct);
  // turnWest();
  d = sideDistance.objectDistance(inches);
  slideFor(1.5 - d, 40, false);
  opticalRoller();
};

bool rollerMotorStopped = true;

void usercontrol(void) {
  wait(3, seconds);

  Controller1.ButtonLeft.pressed(turnNorth);
  Controller1.ButtonRight.pressed(turnSouth);
  Controller1.ButtonUp.pressed(turnEast);
  Controller1.ButtonDown.pressed(turnWest);

  Controller1.ButtonB.pressed(toggleFeeding);
  Controller1.ButtonY.pressed(shootFeed);
  Controller1.ButtonX.pressed(shootStraight);

  Controller1.rumble(".");

  // roller_easy();
  roller_hard();
  // solo_easy();

  // Drivetrain.setStopping(hold);

  while (true) {

    if (Controller1.ButtonR1.pressing()) {
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

void resetMotors() {
  leftMotorA.resetPosition();
  leftMotorB.resetPosition();
  rightMotorA.resetPosition();
  rightMotorB.resetPosition();
  indexer.setRotation(0, degrees);
}

void setupRobot() {
  resetMotors();
  roller.setBrake(brake);
  /*
   roller.setTimeout(3, seconds);
   backBumper.pressed(onBackBumperPressed);
   */
}

int auton = 0;
void menu() {
  std::string txt[] = {"solo", "roller hard", "roller easy"};
  int auton_num = sizeof(txt) / sizeof(txt[0]);

  while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(ALLIANCE_COLOR);
    Brain.Screen.drawRectangle(20, 40, 120, 140);
    Brain.Screen.setPenColor(white);
    Brain.Screen.printAt(30, 70, false, "Prev");

    Brain.Screen.setFillColor(green);
    Brain.Screen.drawRectangle(180, 40, 120, 140);
    Brain.Screen.setPenColor(black);
    Brain.Screen.printAt(190, 70, false, "%s", txt[auton].c_str());

    Brain.Screen.setFillColor(ALLIANCE_COLOR);
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

void autonomous(void) {
  switch (auton) {
  case 0:
    solo();
    break;
  case 1:
    roller_hard();
    break;
  case 2:
    roller_easy();
    break;
  }
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

/*
void toggleRoller() {
  roller.spin(forward, 20, percent);
  waitUntil(!Controller1.ButtonL1.pressing());
  roller.stop(brake);
} */

/*
void onBackBumperPressed() {
  if (!backHookOn) {
    Drivetrain.stop();
    toggleBackHook();
    Controller1.rumble("..");
  }
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


*/
