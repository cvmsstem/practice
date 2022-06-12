#include "vex.h"
#include <string>

using namespace vex;

#define DUMPING_ANGLE 97
#define DUMPING_PAUSE 800

// A global instance of competition
competition Competition;

/* bool backHookOn = false;
void toggleBackHook() {
  if (backHookOn) {
    backHookMotor.rotateTo(0, degrees);
    backHookOn = false;
  } else {
    backHookMotor.rotateTo(85, degrees);
    backHookOn = true;
  }
}

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
  /*arm.setRotation(0, degrees);
  backHookMotor.setRotation(0, degrees);
  topHookMotor.setRotation(0, degrees);
*/
}
/*
void dump2Rings(int pause = DUMPING_PAUSE) {
  arm.spinToPosition(DUMPING_ANGLE, degrees, 25, velocityUnits::pct);
  wait(pause, msec);
}

void pickupRings() {
  topHookMotor.spinToPosition(-180, degrees, 100, velocityUnits::pct);
  arm.spinToPosition(0, degrees, 20, velocityUnits::pct);
  topHookMotor.spinToPosition(0, degrees, 25, velocityUnits::pct);
}

void dumpRings(int count, int pause = DUMPING_PAUSE) {
  double d = frontkDistance.objectDistance(inches);
  Drivetrain.driveFor(forward, d - 3, inches, 20, velocityUnits::pct);
  dump2Rings(pause);
  for (int i = 0; i < count - 1; i++) {
    pickupRings();
    dump2Rings(pause);
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

void getMogoBack(double distanceTo, double speedTo, double distanceBack,
                 double speedBack, bool try_on = false) {
  driveBackHook(distanceTo, speedTo, try_on);
  wait(200, msec);

  if (backHookOn) {
    Drivetrain.driveFor(forward, distanceBack, inches, speedBack,
                        velocityUnits::pct);
  } else {
    getMogoBack(3, 30, distanceBack, speedBack, true);
  }
}

void dumpRingBack(double distance, int speed = 75) {
  arm.spinToPosition(33, degrees, 15, velocityUnits::pct);
  driveBackHook(15, 30);
  Drivetrain.driveFor(forward, distance, inches, speed, velocityUnits::pct);
  // if (backHookOn) toggleBackHook();
  topHookMotor.spinToPosition(220, degrees, 30, velocityUnits::pct);
  wait(0.5, seconds);
  arm.spinToPosition(45, degrees, 50, velocityUnits::pct);
}

void seesawRings() {
  Drivetrain.setHeading(90, degrees);
  dumpRings(5);
  arm.spinToPosition(45, degrees, 50, velocityUnits::pct);
}

void solo() {
  Drivetrain.setHeading(270, degrees);
  arm.spinToPosition(30, degrees, 20, velocityUnits::pct, false);

  topHookMotor.spinToPosition(90, degrees, 15, velocityUnits::pct);
  wait(1, seconds);
  topHookMotor.spinToPosition(0, degrees, 20, velocityUnits::pct);

  double d = sideDistance.objectDistance(inches);
  slideFor(28 - d, 40);
  Drivetrain.driveFor(reverse, 87, inches, 50, velocityUnits::pct);

  dumpRingBack(20);
  turnWest();
  arm.spinToPosition(90, degrees, 40, velocityUnits::pct, false);
  Drivetrain.driveFor(forward, 40, inches, 75, velocityUnits::pct);
}

void awpRings() {
  Drivetrain.setHeading(0, degrees);
  dumpRings(4);
  turnSouth();
  getMogoBack(3, 30, 12, 100, true);
  turnSouth();
  if (backHookOn)
    toggleBackHook();
}
*/
int auton = 0;

const int program_color = red;

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
  /* arm.setBrake(hold);
   arm.setTimeout(3, seconds);
   backHookMotor.setBrake(hold);
   backHookMotor.setTimeout(3, seconds);
   topHookMotor.setBrake(hold);
   topHookMotor.setTimeout(3, seconds);
   backBumper.pressed(onBackBumperPressed);
   */
}

/*
void awpMiddle() {
  Drivetrain.setHeading(180, degrees);
  Drivetrain.driveFor(reverse, 52, inches, 100, velocityUnits::pct);

  slideFor(30);
  turnSouth();
  arm.spinToPosition(45, degrees, 20, velocityUnits::pct, false);
  Drivetrain.driveFor(forward, 17, inches, 50, velocityUnits::pct);
  slideFor(12, 30);
  turnSouth();
  getMogoBack(7, 30, 30, 50, true);
}
*/
void solo_easy(){};
void solo_hard(){};
void roller_easy(){};
void roller_hard(){};

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

/*void ringTest() {
  double d = frontkDistance.objectDistance(inches);
  Drivetrain.driveFor(forward, d - 3, inches, 20, velocityUnits::pct);
  pickupRings();
  dump2Rings(1000);
}

void armUp() {
  arm.spinToPosition(135, degrees, 40, velocityUnits::pct, false);
}

void armDown() {
  arm.spinToPosition(190, degrees, 40, velocityUnits::pct, false);
}
*/
bool armMotorStopped = true;
bool topHookMotorStopped = true;

void usercontrol(void) {

  Drivetrain.setHeading(90, degrees);

  /*arm.setVelocity(15, percent);
  arm.spinToPosition(45, degrees, 40, velocityUnits::pct, false);
  topHookMotor.spinToPosition(-90, degrees, 100, velocityUnits::pct, false);

  Controller1.ButtonB.pressed(toggleBackHook);
  Controller1.ButtonL1.pressed(armUp);
  Controller1.ButtonL2.pressed(armDown);
*/
  Controller1.ButtonLeft.pressed(turnWest);
  Controller1.ButtonRight.pressed(turnEast);
  Controller1.ButtonUp.pressed(turnNorth);
  Controller1.ButtonDown.pressed(turnSouth);

  Controller1.rumble(".");

  // Drivetrain.setStopping(hold);
  /* while (true) {

     // check the ButtonR1/ButtonR2 status to control arm
     if (Controller1.ButtonR1.pressing()) {
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
     }

     if (Controller1.ButtonX.pressing()) {
       topHookMotor.spin(forward);
       topHookMotorStopped = false;
     } else if (!topHookMotorStopped) {
       topHookMotor.stop();
       topHookMotorStopped = true;
     }
     wait(20, msec);
   }*/
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  menu();
  task checkTemperatureTask(checkTemperature);

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

int main() {

  setupRobot();

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