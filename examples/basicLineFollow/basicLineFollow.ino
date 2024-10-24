#include <Sofdcar-HAL.hpp>
#include <Sofdcar-RPC.hpp>
#include "lineFollowing.hpp"
#include "Servo.h"
#include "Arduino.h"

// #define RED_CAR
#define BLUE_CAR

// #define PAPER
#define WOOD

#include "SerialDebug.hpp"
#include "WifiDebug.hpp"
WifiDebug *debug;
bool isManualMode = false;
uint16_t lastFrontDist = 0;

#define MEGA_STEERING_CAR
// #define MEGA_FIXED_CAR

#if defined(MEGA_STEERING_CAR)
// todo: calibrate
const PROGMEM int16_t rpmLut[] = {106, -902, -898, -894, -890, -886, -882, -878, -875, -871, -867, -863, -859, -855, -851, -847, -843, -839, -835, -831, -827, -823, -819, -815, -811, -808, -804, -800, -796, -792, -788, -784, -780, -776, -772, -768, -764, -760, -756, -752, -748, -745, -741, -737, -733, -729, -725, -721, -717, -713, -709, -705, -701, -697, -693, -689, -685, -681, -678, -674, -670, -666, -662, -658, -654, -650, -646, -642, -638, -634, -630, -626, -622, -618, -614, -611, -607, -603, -599, -595, -591, -587, -583, -579, -575, -571, -567, -563, -559, -555, -551, -548, -544, -540, -536, -532, -528, -524, -520, -516, -512, -508, -504, -500, -496, -492, -488, -484, -481, -477, -473, -469, -465, -461, -457, -453, -449, -445, -441, -437, -433, -429, -425, -421, -418, -414, -410, -406, -402, -398, -394, -390, -386, -382, -378, -374, -370, -366, -362, -358, -354, -351, -347, -343, -339, -335, -331, -327, -323, -319, -315, -311, -307, -303, -299, -295, -291, -288, -284, -280, -276, -272, -268, -264, -260, -256, -252, -248, -244, -240, -236, -232, -228, -224, -221, -217, -213, -209, -205, -201, -197, -193, -189, -185, -181, -177, -173, -169, -165, -161, -158, -154, -150, -146, -142, -138, -134, -130, -126, -122, -118, -114, -110, -106, -102, -98, -94, -91, -87, -83, -79, -75, -71, -67, -63, -59, -55, -51, -47, -43, -39, -35, -31, -28, -24, -20, -16, -12, -8, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 12, 16, 20, 24, 28, 31, 35, 39, 43, 47, 51, 55, 59, 63, 67, 71, 75, 79, 83, 87, 91, 94, 98, 102, 106, 110, 114, 118, 122, 126, 130, 134, 138, 142, 146, 150, 154, 158, 161, 165, 169, 173, 177, 181, 185, 189, 193, 197, 201, 205, 209, 213, 217, 221, 224, 228, 232, 236, 240, 244, 248, 252, 256, 260, 264, 268, 272, 276, 280, 284, 288, 291, 295, 299, 303, 307, 311, 315, 319, 323, 327, 331, 335, 339, 343, 347, 351, 354, 358, 362, 366, 370, 374, 378, 382, 386, 390, 394, 398, 402, 406, 410, 414, 418, 421, 425, 429, 433, 437, 441, 445, 449, 453, 457, 461, 465, 469, 473, 477, 481, 484, 488, 492, 496, 500, 504, 508, 512, 516, 520, 524, 528, 532, 536, 540, 544, 548, 551, 555, 559, 563, 567, 571, 575, 579, 583, 587, 591, 595, 599, 603, 607, 611, 614, 618, 622, 626, 630, 634, 638, 642, 646, 650, 654, 658, 662, 666, 670, 674, 678, 681, 685, 689, 693, 697, 701, 705, 709, 713, 717, 721, 725, 729, 733, 737, 741, 745, 748, 752, 756, 760, 764, 768, 772, 776, 780, 784, 788, 792, 796, 800, 804, 808, 811, 815, 819, 823, 827, 831, 835, 839, 843, 847, 851, 855, 859, 863, 867, 871, 875, 878, 882, 886, 890, 894, 898, 902};
#elif defined(MEGA_FIXED_CAR)
const PROGMEM int16_t rpmLut[] = {106, -902, -898, -894, -890, -886, -882, -878, -875, -871, -867, -863, -859, -855, -851, -847, -843, -839, -835, -831, -827, -823, -819, -815, -811, -808, -804, -800, -796, -792, -788, -784, -780, -776, -772, -768, -764, -760, -756, -752, -748, -745, -741, -737, -733, -729, -725, -721, -717, -713, -709, -705, -701, -697, -693, -689, -685, -681, -678, -674, -670, -666, -662, -658, -654, -650, -646, -642, -638, -634, -630, -626, -622, -618, -614, -611, -607, -603, -599, -595, -591, -587, -583, -579, -575, -571, -567, -563, -559, -555, -551, -548, -544, -540, -536, -532, -528, -524, -520, -516, -512, -508, -504, -500, -496, -492, -488, -484, -481, -477, -473, -469, -465, -461, -457, -453, -449, -445, -441, -437, -433, -429, -425, -421, -418, -414, -410, -406, -402, -398, -394, -390, -386, -382, -378, -374, -370, -366, -362, -358, -354, -351, -347, -343, -339, -335, -331, -327, -323, -319, -315, -311, -307, -303, -299, -295, -291, -288, -284, -280, -276, -272, -268, -264, -260, -256, -252, -248, -244, -240, -236, -232, -228, -224, -221, -217, -213, -209, -205, -201, -197, -193, -189, -185, -181, -177, -173, -169, -165, -161, -158, -154, -150, -146, -142, -138, -134, -130, -126, -122, -118, -114, -110, -106, -102, -98, -94, -91, -87, -83, -79, -75, -71, -67, -63, -59, -55, -51, -47, -43, -39, -35, -31, -28, -24, -20, -16, -12, -8, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 12, 16, 20, 24, 28, 31, 35, 39, 43, 47, 51, 55, 59, 63, 67, 71, 75, 79, 83, 87, 91, 94, 98, 102, 106, 110, 114, 118, 122, 126, 130, 134, 138, 142, 146, 150, 154, 158, 161, 165, 169, 173, 177, 181, 185, 189, 193, 197, 201, 205, 209, 213, 217, 221, 224, 228, 232, 236, 240, 244, 248, 252, 256, 260, 264, 268, 272, 276, 280, 284, 288, 291, 295, 299, 303, 307, 311, 315, 319, 323, 327, 331, 335, 339, 343, 347, 351, 354, 358, 362, 366, 370, 374, 378, 382, 386, 390, 394, 398, 402, 406, 410, 414, 418, 421, 425, 429, 433, 437, 441, 445, 449, 453, 457, 461, 465, 469, 473, 477, 481, 484, 488, 492, 496, 500, 504, 508, 512, 516, 520, 524, 528, 532, 536, 540, 544, 548, 551, 555, 559, 563, 567, 571, 575, 579, 583, 587, 591, 595, 599, 603, 607, 611, 614, 618, 622, 626, 630, 634, 638, 642, 646, 650, 654, 658, 662, 666, 670, 674, 678, 681, 685, 689, 693, 697, 701, 705, 709, 713, 717, 721, 725, 729, 733, 737, 741, 745, 748, 752, 756, 760, 764, 768, 772, 776, 780, 784, 788, 792, 796, 800, 804, 808, 811, 815, 819, 823, 827, 831, 835, 839, 843, 847, 851, 855, 859, 863, 867, 871, 875, 878, 882, 886, 890, 894, 898, 902};
#endif

uint8_t pins[] = {A0, A1, A2};
#ifdef MEGA_STEERING_CAR
// left: weiß: 30-34, tape: 32-36, schwarz: 9-13, schwarzTape: 18-21 => min: 30 max: 21 => offset: 13, factor:
// middle: weiß: 42-47, tape: 42-48, schwarz: 13-19, schwarzTape: 23-28 => min: 42, max: 28 => offset: 0, factor: 1
// right: weiß: 24-30, tape: 22-32, schwarz: 7-10, schwarzTape: 13-19 => min: 22, max: 19 => offset: 18, factor:

// printed
//  left: weiß: 114-180, schwarz: 41 => min: 11 max: 31 => offset: 13, factor:
//  middle: weiß: 132-235, schwarz: 61 => min: 14, max: 44 => offset: 0, factor: 1
//  right: weiß: 80-170, schwarz: 31 => min: 8, max: 28 => offset: 18, factor:
#if defined(RED_CAR) and defined(WOOD)
BrightnessThresholds thresholds[] = { // car red
    {67, 52},
    {75, 47},
    {73, 55}};
#elif defined(BLUE_CAR) and defined(WOOD)
BrightnessThresholds thresholds[] = { // car blue
    {72, 210},
    {100, 245},
    {63, 189}};
#elif defined(RED_CAR) and defined(PAPER)
BrightnessThresholds thresholds[] = {
    {32, 130},
    {30, 138},
    {36, 136}};
#elif defined(BLUE_CAR) and defined(PAPER)
BrightnessThresholds thresholds[] = { // Orig
    {90, 184},
    {107, 224},
    {85, 156}};
#endif
#if defined(RED_CAR)
ServoAxle serv(52, 40, 40, 47, 73, 98); // red car
#elif defined(BLUE_CAR)
ServoAxle serv(52, 40, 39, 29, 66, 102); // blue car
#endif
MotorDcHBridge rearLeft(MOTOR_SPEED_UNIT_CM_PER_SEC, 13, 12, 11);
MotorDcHBridge rearRight(MOTOR_SPEED_UNIT_CM_PER_SEC, 8, 9, 10);
TurnSteeringDriveController dc(
    rearLeft,
    rearRight,
    serv,
    120,
    100);
BrightnessSensorAnalog sensor(pins, 3, thresholds);
LinearSensorEdgeDetector ld(sensor, 16, false);
UltrasonicDistanceSensor frontDistance(48, 49);
UltrasonicDistanceSensor rearDistance(46, 47);
#elif defined(MEGA_FIXED_CAR)
BrightnessThresholds thresholds[] = {
    {684, 97},
    {720, 105},
    {392, 95}};

MotorDcHBridge frontLeft(MOTOR_SPEED_UNIT_CM_PER_SEC, 2, 4, 3);
MotorDcHBridge frontRight(MOTOR_SPEED_UNIT_CM_PER_SEC, 7, 5, 6);
MotorDcHBridge rearLeft(MOTOR_SPEED_UNIT_CM_PER_SEC, 8, 10, 9);
MotorDcHBridge rearRight(MOTOR_SPEED_UNIT_CM_PER_SEC, 13, 11, 12);
FixedWheelDriveControllerBoostMode dc(
    frontLeft,
    frontRight,
    rearLeft,
    rearRight,
    129,
    119);
BrightnessSensorAnalog sensor(pins, 3, thresholds);
LinearSensorEdgeDetector ld(sensor, 17, false);
UltrasonicDistanceSensor frontDistance(26, 27);
UltrasonicDistanceSensor rearDistance(24, 25);
#endif

PolyCurveMotorProfile profile(21 /*cm*/);
LineFollower follower(&ld, &dc, &frontDistance);

/* RPC */
EmergencyStop stop;
RpcClass *rootClass = nullptr;
RpcManager *mgr = nullptr;
/* /RPC */

void onWifiMessage(char *data, uint16_t len)
{
    if (len < 5)
    {
        return;
    }
    isManualMode = true;
    switch (data[0])
    {
    case 'd': // drive
        dc.drive(data[5], (data[6] << 8) | data[7]);
        break;
    case 'p': // pause
        dc.pause();
        break;
    case 'r': // resume
        dc.resume();
        break;
    default:
        break;
    }
}

void setup()
{ // middle: 66, right: 109, left: 19
    debug = new WifiDebug();
    debug->setOnMessage(onWifiMessage);
    Serial.begin(115200);

    /* RPC */
    Serial1.begin(115200);
    rootClass = rpc_createForHal();
    mgr = new RpcManager(Serial1, rootClass);
    ((RpcRootMember *)rootClass->getMember("dc"))->updateValue(&dc);
    ((RpcRootMember *)rootClass->getMember("stop"))->updateValue(&stop);
    Serial.println("RPC init");
    /* /RPC */

    profile.setMotorCurve(const_cast<int16_t *>(rpmLut), MOTOR_CURVE_TYPE_PROGMEM);
    rearLeft.setProfile(&profile);
    rearRight.setProfile(&profile);
#ifdef MEGA_FIXED_CAR
    frontLeft.setProfile(&profile);
    frontRight.setProfile(&profile);

    dc.setSimpleMode(true);
#endif
    Serial.println("Started");
    dc.setAngle(0);

#if defined(RED_CAR)
    dc.setSpeed(63);
#elif defined(BLUE_CAR)
    dc.setSpeed(40);
#endif

    follower.setLineToFollow(1);
}

unsigned long lastFrontContact = 0;

void loop()
{
    SerialDebug_loop(dc, sensor, ld);
    mgr->loop();

    dc.loop();

    /*int8_t pos = ld.getLinePositionMm();
    int8_t angle = ld.getLineAngle();
    Serial.print("Detected line pos: ");
    Serial.print(pos);
    Serial.print("mm; Legacy: ");
    Serial.println(ld.positionToLegacy(pos, angle));*/

    /*uint16_t d = frontDistance.getDistanceToClosestMm();
    if (d > lastFrontDist + 10 || d < lastFrontDist - 10)
    {
        uint8_t distData[9];
        distData[0] = 'd';
        distData[1] = 'i';
        distData[2] = 's';
        distData[3] = 't';
        distData[4] = '#';
        distData[5] = (511 >> 8) & 0xff;
        distData[6] = 511 & 0xff;
        distData[7] = '\n';
        distData[8] = 0;
        Serial.print("Sending i2c ");
        Serial.print(strlen((char *)distData));
        Serial.print(": ");
        Serial.println((char *)distData);
        // debug->write(distData, 8);
        Serial.println("Sent front distance");
        lastFrontDist = d;
    }*/

    if (!isManualMode)
    {
        // lineFollowing(dc, ld, frontDistance, rearDistance);
        uint16_t dR = rearDistance.getDistanceToClosestMm();
        Serial.print("Rear distance: ");
        Serial.println(dR);
#if defined(RED_CAR)
        if (dc.getState() != DRIVE_CONTROLLER_STATE_PAUSED)
        {
            unsigned long timeSinceLast = millis() - lastFrontContact;
            if ((lastFrontContact == 0 || timeSinceLast > 3000) && frontDistance.getDistanceToClosestMm() < 200)
            {
                Serial.println("Switching line to left");
                lastFrontContact = millis();
                follower.setLineToFollow(0);
            }
            else
            {
                if (lastFrontContact > 0 && timeSinceLast > 5500)
                {
                    Serial.println("Switching line back right");
                    follower.setLineToFollow(1);
                    lastFrontContact = 0;
                }
            }
        }
#endif
        follower.loop();
    }
    else
    {
        /*if (d < 100 && dc.getSpeed() > 0)
        {
            dc.pause();
        }
        else if ((d > 110 || dc.getNonPausedSpeed() < 0) && dc.getSpeed() == 0)
        {
            dc.resume();
        }*/
    }
    delay(50);
}
