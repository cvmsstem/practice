#include "vex.h"

#define K_STRAFE 1.07
#define K_TURN 5.0

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftMotorA = motor(PORT1, ratio18_1, false);
motor leftMotorB = motor(PORT11, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT10, ratio18_1, true);
motor rightMotorB = motor(PORT20, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
inertial DrivetrainInertial = inertial(PORT8);
distance backDistance = distance(PORT17);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart,
                                   DrivetrainInertial, 319.19, 320, 40, mm, 1);



motor shooterSetterMotor = motor(PORT6, ratio18_1, false);
motor shooterMotor1 = motor(PORT16, ratio6_1, false); 
motor shooterMotor2 = motor(PORT15, ratio6_1, true); 
motor_group shooter = motor_group(shooterMotor1, shooterMotor2);
motor roller = motor(PORT9, ratio18_1, false);
motor indexer = motor(PORT5, ratio18_1, false);

//bumper backBumper = bumper(Brain.ThreeWirePort.B);
//distance frontDistance = distance(PORT10);
optical rollerOptical = optical(PORT2);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs

bool DrivetrainNeedsToBeStopped = false;

int setupBand(int position) {
  int bandValue = position;
  int sign = position / abs(position);
  if (abs(position) < 5)
    bandValue = 0;
  if (abs(position) > 10 && abs(position) < 50) {
    bandValue = 10 * sign;
  } else if (abs(position) >= 50 && abs(position) < 90) {
    bandValue = 50 * sign;
  }
  return bandValue;
}

bool anyOrientation = false;

int getOrientation() {
  if (!anyOrientation)
    return 0;
  double heading = DrivetrainInertial.heading();
  if (heading > 45 && heading <= 90 + 45)
    return 1;
  if (heading > 90 + 45 && heading <= 180 + 45)
    return 2;
  if (heading > 180 + 45 && heading <= 270 + 45)
    return 3;
  return 0;
}

void toggleOrientation() { anyOrientation = !anyOrientation; }

bool noTurn = false;

void toggleTurn() { noTurn = !noTurn; }

void slideFor(float distance, int speed, bool wait_commplete) {
  float numberDeg;
  numberDeg = (distance / (4 * 3.1415)) * 360 * K_STRAFE;
  leftMotorA.spinFor(numberDeg, deg, speed, velocityUnits::pct, false);
  leftMotorB.spinFor(-numberDeg, deg, speed, velocityUnits::pct, false);
  rightMotorA.spinFor(-numberDeg, deg, speed, velocityUnits::pct, false);
  rightMotorB.spinFor(numberDeg, deg, speed, velocityUnits::pct,
                      wait_commplete);
}

void quickTurn(float degree, int speed, bool wait_commplete) {
  double numberDeg;
  numberDeg = degree * K_TURN;
  rightMotorA.spinFor(-1 * numberDeg, deg, speed, velocityUnits::pct, false);
  leftMotorA.spinFor(numberDeg, deg, speed, velocityUnits::pct, false);
  rightMotorB.spinFor(-1 * numberDeg, deg, speed, velocityUnits::pct, false);
  leftMotorB.spinFor(numberDeg, deg, speed, velocityUnits::pct, wait_commplete);
}

const double turn_threshold = 1;

void turnNorth() {
  int orientation = DrivetrainInertial.heading();
  if (orientation <= 180) {
    quickTurn(-orientation);
  } else {
    quickTurn(360 - orientation);
  }
}

void turnSouth() {
  int orientation = 180 - DrivetrainInertial.heading();

  if (abs(orientation) < turn_threshold)
    return;
  quickTurn(orientation);
}

void turnEast() {
  int orientation = DrivetrainInertial.heading();
  if (abs(90 - orientation) < turn_threshold)
    return;
  if (orientation < 270)
    quickTurn(90 - orientation);
  else
    quickTurn(180 - (orientation - 270));
}

void turnWest() {
  int orientation = DrivetrainInertial.heading();
  if (abs(270 - orientation) < turn_threshold)
    return;
  if (orientation > 90)
    quickTurn(270 - orientation);
  else
    quickTurn(-(orientation + 90));
}

void turnRight() { quickTurn(90); }

void turnLeft() { quickTurn(-90); }

void resetHeading() { DrivetrainInertial.setHeading(0, degrees); }

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while (true) {
    if (RemoteControlCodeEnabled) {
      // use heading to automatically switch axis control, e.g. reverse the
      // drive.

      int turnValue = setupBand(Controller1.Axis1.position());
      int axis3Value = setupBand(Controller1.Axis3.position());
      int axis4Value = setupBand(Controller1.Axis4.position());

      if (noTurn)
        turnValue = 0;

      int orientation = getOrientation();
      int tmp;
      switch (orientation) {
      case 0:
        // do nothing
        break;
      case 1:
        tmp = axis3Value;
        axis3Value = axis4Value;
        axis4Value = -tmp;
        break;
      case 2:
        axis3Value = -axis3Value;
        axis4Value = -axis4Value;
        break;
      case 3:
        tmp = axis3Value;
        axis3Value = -axis4Value;
        axis4Value = tmp;
        break;
      }

      if (turnValue == 0 && axis3Value == 0 && axis4Value == 0) {
        if (DrivetrainNeedsToBeStopped) {
          Drivetrain.stop();
          DrivetrainNeedsToBeStopped = false;
        }
      } else {
        int drivetrainLeftASpeed = axis3Value + turnValue + axis4Value;
        int drivetrainLeftBSpeed = axis3Value + turnValue - axis4Value;
        int drivetrainRightASpeed = axis3Value - turnValue - axis4Value;
        int drivetrainRightBSpeed = axis3Value - turnValue + axis4Value;

        leftMotorA.spin(forward, drivetrainLeftASpeed, percent);
        leftMotorB.spin(forward, drivetrainLeftBSpeed, percent);
        rightMotorA.spin(forward, drivetrainRightASpeed, percent);
        rightMotorB.spin(forward, drivetrainRightBSpeed, percent);

        DrivetrainNeedsToBeStopped = true;
      }

      // wait before repeating the process
      wait(20, msec);
    }
  }
  return 0;
}

void checkMotor(vex::motor &m, const char *name, int row) {

  Brain.Screen.setCursor(row, 1);

  int t = 0;
  int s = 0;
  int v = 0;

  t = m.temperature(celsius);
  s = m.velocity(velocityUnits::rpm);
  v = m.voltage();
  Brain.Screen.print("%s: %dC, %dRPM, %dV", name, t, s, v );
}

int checkTemperature() {

  while (true) {
    Brain.Screen.clearScreen();

    checkMotor(leftMotorA, "Left A", 1);

    checkMotor(leftMotorB, "Left B", 3);

    checkMotor(rightMotorA, "right A", 5);

    checkMotor(rightMotorB, "right B", 7);

    checkMotor(shooterMotor1, "shooter1", 9);

    checkMotor(shooterMotor2, "shooter2", 11);

    checkMotor(roller, "roller", 12);

    wait(500, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */

void vexcodeInit(void) {
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain Inertial
  wait(200, msec);
  DrivetrainInertial.calibrate();
  Brain.Screen.print("Calibrating Inertial for Drivetrain");
  // wait for the Inertial calibration process to finish
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();

  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
  task checkTemperatureTask(checkTemperature);

  wait(50, msec);
  Controller1.rumble("..");
}