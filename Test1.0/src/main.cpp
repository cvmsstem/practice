#include "vex.h"
#include <cmath>
#include <string>

using namespace vex;

#define DUMPING_ANGLE 97
#define DUMPING_PAUSE 800

// A global instance of competition
competition Competition;

void toggleFlying() {
  shooter.spin(forward, 10, voltageUnits::volt);
  while (fabs(shooter.velocity(rpm)) < 200) {
    wait(50, msec);
  }
  indexer.rotateTo(90, degrees);
  wait(200, msec);
  indexer.rotateTo(0, degrees);
  shooter.stop(coast);
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

void toggleRoller() {
  roller.spin(forward, 20, percent);
  waitUntil(!Controller1.ButtonB.pressing());
  roller.stop(brake);
}

const int program_color = red;

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

void solo_easy(){};
void solo_hard(){};
void roller_easy() {
  slideFor(2);
  opticalRoller();
  slideFor(-2);
  Drivetrain.turnFor(-10, degrees);
  toggleFlying();
}
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

bool armMotorStopped = true;
bool topHookMotorStopped = true;

void usercontrol(void) {

  Drivetrain.setHeading(90, degrees);

  Controller1.ButtonLeft.pressed(turnWest);
  Controller1.ButtonRight.pressed(turnEast);
  Controller1.ButtonUp.pressed(turnNorth);
  Controller1.ButtonDown.pressed(turnSouth);
  Controller1.ButtonB.pressed(toggleRoller);
  Controller1.ButtonA.pressed(toggleFlying);
  Controller1.ButtonX.pressed(toggleFeeding);

  Controller1.rumble(".");

  // opticalRoller();

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