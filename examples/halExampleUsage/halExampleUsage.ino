#include <Sofdcar-HAL.hpp>

// ------------------- Driving hardware abstraction -------------------
// Motor Setup for drive motors
MotorDcHBridge simpleMotor(
    MOTOR_SPEED_UNIT_RPM,
    /*Enable Pin (pwm pin for speed)*/ 2,
    /*Polatity pin 1 (high on forward)*/ 4,
    /*Polarity pin 2 (high on reverse)*/ 3);

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
TurnSteeringDriveController dc(
    /*The rear left drive motor*/ simpleMotor,
    /*The front left drive motor (should - but isn't required to - be independantly motorized of the left one for driving optimization)*/
    simpleMotor,
    /*The servo steered front axle*/ steeredAxle,
    /*Distance of the wheel centers in the width of the car (perpendicular to straigt driving axis) in mm*/ 120,
    /*Distance of the wheel centers in the length of the car (parallel to straigt driving axis) in mm*/ 100);
// Drive controller for car with 4 motrized fixed directon wheels.
// Left and right motors MUST be independantly motorized, front and rear motors of one side may be combined
// BUT: All 4 wheels must be motorized
FixedWheelDriveController dc(
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

// Edge of line detection using an array of brightness sensors
LinearSensorEdgeDetector ld(
    /*One brightness sensor array or list of multiple sensor arrays (from left to right)*/ analogSensor,
    /*Distance between to adjacent brightness sensors in the array(s) in mm*/ 16,
    /*Optional: The edge to follow (true=left, false=right), can be changed later*/ true);

// ------------------- Distance/Other vehicle detection hardware abstraction -----------------
// Ultrasonic Distance Sensor module
UltrasonicDistanceSensor frontDistance(
    /*The pin the trigger of the sensor is connected to*/ 13,
    /*The pin the echo of the sensor returns on (may be the same as the trigger pin)*/ 4);

void setup()
{
}

void loop()
{
}