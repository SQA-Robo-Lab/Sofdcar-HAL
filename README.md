# New Motor Driver (Hardware abstraction libarary)
Library to abstract away the interaction with the hardware of the car and unify utilization between different car types.

## Installation
[Download this repository as ZIP file](https://github.com/SQA-Robo-Lab/Sofdcar-HAL/archive/refs/heads/main.zip) and install it in your Arduino IDE via `Sketch -> Include library -> Add .ZIP library`

## Class diagram
### Simplified (Functional block diagram)

### Full class diagrm
#### Driving hardware abstraction
```mermaid
classDiagram   
    class DriveController{
        <<abstract>>
        #int16_t speed
        #int8_t angle

        +drive(int8_t angle, int16_t speed)
        +pause()
        +resume()

        +setSpeed(int16_t speed)
        +setAngle(int8_t angle)
        +getSpeed() int16_t
        +getAngle() int8_t 

        +loop()*

        #setSpeedInternal(int16_t speed)*
        #setAngleInternal(int8_t angle)*
    }

    class FixedWheelDriveController{
        -uint8_t carWidthMm
        -uint8_t carLengthMm

        +loop()

        #setSpeedInternal(int16_t speed)
        #setAngleInternal(int8_t angle)
    }

    class TurnSteeringDriveController{
        -uint8_t carWidthMm
        -uint8_t carLengthMm

        +loop()

        #setSpeedInternal(int16_t speed)
        #setAngleInternal(int8_t angle)
    }

    class DriveControllerState {
        <<enumeration>>
        DRIVE_CONTROLLER_STATE_STOPPED
        DRIVE_CONTROLLER_STATE_PAUSED
        DRIVE_CONTROLLER_STATE_DRIVING
    }


    class Motor {
        <<abstract>>
        +setProfile(MotorProfile *profile)
        +setSpeed(int32_t speed)
        +loop()

        #setSpeedRatioInternal(int16_t speedRatio)*
    }

    class MotorSpeedUnit {
        <<enumeration>>
        MOTOR_SPEED_UNIT_RATIO
        MOTOR_SPEED_UNIT_RPM
        MOTOR_SPEED_UNIT_CM_PER_SEC
    }

    class MotorDcHBridge {
        -uint8_t speedPin
        -uint8_t polarityPin1
        -uint8_t polarityPin2

        #setSpeedRatioInternal(int16_t speedRatio)
    }

    class MotorProfile {
        <<abstract>>
        #uint16_t clipRpmForward
        #uint16_t clipRpmBackward

        setMaxRpmForward(uint16_t rpm)
        setMaxRpmBackward(uint16_t rpm)

        rpmToRatio(int16_t rpm) int16_t*
        getMaxPossibleRpmForward() uint16_t*
        getMaxPossibleRpmBackward() uint16_t*

        loop()
    }

    class PolyCurveMotorProfile {
        #int16_t *rpmLut

        -int16_t getLutValueAt(int16_t i)

        +rpmToRatio(int16_t rpm) int16_t
        +getMaxPossibleRpmForward() uint16_t
        +getMaxPossibleRpmBackward() uint16_t

        +calculateMotorCurve(struct MotorKnownRpm points[], uint8_t len)
        +setMotorCurve(int16_t *lut, MotorCurveLutType type)
    }
    PolyCurveMotorProfile --> "1 #type" MotorCurveLutType

    class MotorCurveLutType {
        <<enumeration>>
        MOTOR_CURVE_TYPE_NONE,
        MOTOR_CURVE_TYPE_RAM,
        MOTOR_CURVE_TYPE_RAM_ALLOCATED,
        MOTOR_CURVE_TYPE_PROGMEM
    }

    class SteerableAxle {
        <<abstract>>
        #uint8_t maxLeftAngle
        #uint8_t maxRightAngle

        setAngle(int8_t angle)*
    }

    class ServoAxle {
        #Servo servoInstance
        #uint8_t pin
        #uint8_t servoLeft
        #uint8_t servoStraight
        #uint8_t servoRight

        +setAngle(int8_t angle)
        +reattachServo(uint8_t servoPin)
    }

    DriveController <|-- TurnSteeringDriveController
    DriveController <|-- FixedWheelDriveController
    Motor <|-- MotorDcHBridge
    MotorProfile <|-- PolyCurveMotorProfile
    SteerableAxle <|-- ServoAxle

    DriveController --> "1 #state" DriveControllerState
    FixedWheelDriveController --> "2..4 -[front/rear][Left/Right]" Motor
    TurnSteeringDriveController --> "2 -rear[Left/Right]" Motor
    TurnSteeringDriveController --> "1 -steering" SteerableAxle
    Motor --> "1 #unit" MotorSpeedUnit
    Motor --> "0..1 #profile" MotorProfile
```

#### Line/Lane detection hardware abstraction
```mermaid
classDiagram
    class LineDetector {
        <<abstract>>
        +getLegacyPosition() LinePosition
        +positionToLegacy(int8_t pos, int8_t angle) LinePosition

        +getLinePositionMm() int8_t*
        +getLineAngle() int8_t*
    }

    class LinePosition {
        <<enumeration>>
        LEFT_OF_LINE,
        ON_LINE,
        RIGHT_OF_LINE,
        LINE_POSITION_UNKNOWN
    }

    class LinearSensorLineDetector {
        -uint8_t sensorsLen
        -uint8_t totalSensors
        -uint8_t sensorDistance

        +getLinePositionMm() int8_t
        +getLineAngle() int8_t
    }

    class LinearSensorEdgeDetector {
        -uint8_t sensorsLen
        -uint8_t totalSensors
        -uint8_t sensorDistance
        -bool leftEdge

        +getLinePositionMm() int8_t
        +getLineAngle() int8_t
        +setFollowEdge(bool followLeftEdge)
    }

    class BrightnessSensor {
        <<abstract>>
        getValues(float *array, uint8_t maxLen) uint8_t*
        numberOfSensors() uint8_t*
    }

    class BrightnessSensorAnalog {
        -uint8_t pinsLen
        -uint8_t *pins

        +getValues(float *array, uint8_t maxLen) uint8_t
        +numberOfSensors() uint8_t
        +setTranslationFunctions(MappingFunction *functions)
        +setThresholds(BrightnessThresholds *briThretholds)
    }

    class BrightnessSensorDigital {
        -uint8_t pinsLen
        -uint8_t *pins

        +getValues(float *array, uint8_t maxLen) uint8_t
        +numberOfSensors() uint8_t
    }

    class BrightnessThresholds {
        <<struct>>
        +uint16_t blackThreshold
        +uint16_t whiteThreshold
    }

    class MappingFunction {
        <<function_type>>
        +(uint16_t oldVal) float
    }

    LineDetector <|-- LinearSensorEdgeDetector
    LineDetector <|-- LinearSensorLineDetector
    BrightnessSensor <|-- BrightnessSensorAnalog
    BrightnessSensor <|-- BrightnessSensorDigital

    LinearSensorEdgeDetector --> "1..* -sensors" BrightnessSensor
    LinearSensorLineDetector --> "1..* -sensors" BrightnessSensor
    BrightnessSensorAnalog --> "0..1 -thresholds" BrightnessThresholds
    BrightnessSensorAnalog --> "0..1 -mappingFunctions" MappingFunction
```

#### Distance/Other vehicle/obstacle hardware abstraction
```mermaid
classDiagram
    class DistanceSensor {
        <<abstract>>
        +getDistanceToClosestMm() uint16_t*
        +getAngleToClosest() int8_t*
    }

    class UltrasonicDistanceSensor {
        -uint8_t trigger
        -uint8_t echo
        -uint16_t timeoutUs

        #echoTimeToMm(unsigned long echoTimeUs) uint16_t

        +getDistanceToClosestMm() uint16_t
        +getAngleToClosest() int8_t
    }

    DistanceSensor <|-- UltrasonicDistanceSensor
```