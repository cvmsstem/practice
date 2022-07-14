using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor arm;
extern motor backHookMotor;
extern motor topHookMotor;

extern motor leftMotorA;
extern motor leftMotorB;
extern motor rightMotorA;
extern motor rightMotorB;

extern motor roller;
extern motor indexer;

extern controller Controller1;

extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;

extern motor_group shooter;

extern smartdrive Drivetrain;
extern inertial DrivetrainInertial;

extern bumper backBumper;
extern distance backDistance;
extern distance sideDistance;
extern optical rollerOptical;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
void quickTurn(float angle, int speed = 75, bool wait_complete=true);
void slideFor(float distance, int speed = 75, bool wait_complete=true);

void toggleOrientation();
void toggleTurn();

void turnSouth();
void turnNorth();
void turnWest();
void turnEast();

int checkTemperature();

// void slideFor(vex::directionType, float, float, bool );