#include <Sofdcar-HAL.hpp>

// ------------------- Driving hardware abstraction -------------------
// Motor Setup for drive motors
MotorDcHBridge simpleMotor(
    MOTOR_SPEED_UNIT_RPM,
    /*Enable Pin (pwm pin for speed)*/ 2,
    /*Polatity pin 1 (high on forward)*/ 4,
    /*Polarity pin 2 (high on reverse)*/ 3);

// Motor profile to adjust behaviour of the motor speed
// either takes measurement points and does a polynomial curve fitting
// OR takes a lookup table that provides one rpm for every possible pwm value of the motor
PolyCurveMotorProfile motorProfile(
    /*Circumference of the wheel attached to the motor in cm*/ 21);

// A pre-calculated motor-rpm-lookup-table stored in program memory (flash) to save on ram usage
// Must have EXACTLY 512 elements (256 for backwards rotation from -256 to -1, 1 for 0, 255 for forwards from 1 to 255 in that order)
// only available on AVR Microprocessors
const PROGMEM int16_t rpmLut[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Steering Servo setup
ServoAxle steeredAxle(
    /*Servo pin (must be supported in Servo-lib)*/ 2,
    /*Maximum angle the wheels can turn from straight to the left*/ 40,
    /*Maximum angle the wheels can turn from straingt to the right*/ 39,
    /*Angle/Value of the servo motor to have the wheels turn to the left maximum*/ 19,
    /*Optional: Angle/Value of the servo motor to have the wheels point exactly straight (drift correction/trim)*/ 66,
    /*Angle/Value of the servo motor to have the wheels turn to the right maximum*/ 109);

// Drive controller setup
// Drive controller for car with 2 motorized back wheels and a steered (unmotorized) front axle
TurnSteeringDriveController steeredDriveController(
    /*The rear left drive motor*/ simpleMotor,
    /*The front left drive motor (should - but isn't required to - be independantly motorized of the left one for driving optimization)*/
    simpleMotor,
    /*The servo steered front axle*/ steeredAxle,
    /*Distance of the wheel centers in the width of the car (perpendicular to straigt driving axis) in mm*/ 120,
    /*Distance of the wheel centers in the length of the car (parallel to straigt driving axis) in mm*/ 100);
// Drive controller for car with 4 motrized fixed directon wheels.
// Left and right motors MUST be independantly motorized, front and rear motors of one side may be combined
// BUT: All 4 wheels must be motorized
FixedWheelDriveController fixedDriveController(
    /*The front left drive motor*/ simpleMotor,
    /*The front right drive motor*/ simpleMotor,
    /*Optional: The rear left drive motor*/ simpleMotor,
    /*Optional: The rear right drive motor*/ simpleMotor,
    /*Distance of the wheel centers in the width of the car (perpendicular to straigt driving axis) in mm*/ 129,
    /*Distance of the wheel centers in the length of the car (parallel to straigt driving axis) in mm*/ 119);

// ------------------- Line/Lane detection hardware abstraction ---------------------
// Mapping of analog read brightness values of the sensors to (no line)-(line)
BrightnessThresholds thresholds[] = {
    /*Array of structs with one entry per sensor (from left to right)*/
    {
        /*Analog sensor value when fully seeing the line (usually black)*/ 41,
        /*Analog sensor value when fully off the line (usually white)*/ 114},
    {61, 140},
    {31, 81}};

// (Analog) pin definitions where the brightness sensors are connected (from left to right)
uint8_t pins[] = {A0, A1, A2};

// Analog sensor array (multiple analog brightness sensors connected to the adc pins of the hardware)
BrightnessSensorAnalog analogSensor(
    /*The list of analog pins with brightness sensors*/ pins,
    /*The number of brightness sensors (must exactly match the length of the supplied lists)*/ 3,
    /*Optional: The list of brightness threshold values for on/off the line*/ thresholds);

// Digital brightness sensor array (NOT recommended as it decreases the resolution of the line detection)
BrightnessSensorDigital digitalSensor(
    /*The list of digital pins the sensors are connnected to (left to right)*/ pins,
    /*The number of brightness sensors (must exactly match the length of the supplied lists)*/ 3,
    /*Set to true to enable internal pullups on all pins*/ false);

// Edge of line detection using an array of brightness sensors
LinearSensorEdgeDetector edgeDetector(
    /*One brightness sensor array or list of multiple sensor arrays (from left to right) followed by number of sensors*/ analogSensor,
    /*Distance between to adjacent brightness sensors in the array(s) in mm*/ 16,
    /*Optional: The edge to follow (true=left, false=right), can be changed later*/ true);

// (Middle of) line detector using sensor array
LinearSensorLineDetector lineDetector(
    /*One brightness sensor array or list of multiple sensor arrays (from left to right) followed by number of sensors*/ analogSensor,
    /*Distance between to adjacent brightness sensors in the array(s) in mm*/ 16);

// ------------------- Distance/Other vehicle detection hardware abstraction -----------------
// Ultrasonic Distance Sensor module
UltrasonicDistanceSensor frontDistance(
    /*The pin the trigger of the sensor is connected to*/ 13,
    /*The pin the echo of the sensor returns on (may be the same as the trigger pin)*/ 4);

void setup()
{
    // -------------------- Motor profile initialization --------------------
    MotorKnownRpm knownRpms[] = {{-255, -900}, {-27, 0}, {27, 0}, {255, 900}};
    // Currently doesn't take into account, that there might a (bigger) voltage range around 0V where the speed of the motor is always 0
    motorProfile.calculateMotorCurve(knownRpms, 4);
    // Set the progmem allocated lookup table. It's also possible to pass a array from ram (either local or global). Change the type accordingly
    motorProfile.setMotorCurve(const_cast<int16_t *>(rpmLut), MOTOR_CURVE_TYPE_PROGMEM);
    simpleMotor.setProfile(&motorProfile);
}

void loop()
{
    // -------------------- Driving --------------------------
    steeredDriveController.setSpeed(/*Drive speed in the speed unit of the motors, negative is backward*/ 100); // Not recommmended for stopping (temporarily)
    steeredDriveController.setAngle(/*The angle (in degrees) to turn at (minus is left, 0 is straight)*/ -10);
    steeredDriveController.drive(/*angle*/ -10, /*speed*/ 100);
    steeredDriveController.pause();  // Stops the car but reatins the last set speed for resume
    steeredDriveController.resume(); // If paused and last speed != 0, resumes driving at the speed before pausing

    // -------------------- Detecting lanes -------------------
    edgeDetector.getLinePositionMm(); // Returns the offset of the followed lane in mm from the center (negative is left)
    edgeDetector.getLineAngle();      // Returns the angle of the line relative to the straight axis of the car (not implemented in any detector yet)
    edgeDetector.getLegacyPosition(); // DEPRECATED: Returns the position as a enum value as implemented before the HAL for easy transition

    // -------------------- Detecting other vehicles/obstacles --------------
    frontDistance.getDistanceToClosestMm(); // Returns the distance in mm to the closest object the sensor sees (DISTANCE_SENSOR_NO_OBJECT if none found)
    frontDistance.getAngleToClosest();      // Returns the angle to the closest object in degrees (not implemented in any detector yet)
}