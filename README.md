# New Motor Driver (Hardware abstraction libarary)
Library to abstract away the interaction with the hardware of the car and unify utilization between different car types.

## Installation
[Download this repository as ZIP file](https://github.com/SQA-Robo-Lab/Sofdcar-HAL/archive/refs/heads/main.zip) and install it in your Arduino IDE via `Sketch -> Include library -> Add .ZIP library`

## Class diagram
### Simplified (Functional block diagram)

### Full class diagrm
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
    DriveController --> "1 #state" DriveControllerState

    DriveController <|-- FixedWheelDriveController
    class FixedWheelDriveController{
        -uint8_t carWidthMm
        -uint8_t carLengthMm

        +loop()

        #setSpeedInternal(int16_t speed)
        #setAngleInternal(int8_t angle)
    }
    FixedWheelDriveController --> "2..4 -[front/rear][Left/Right]" Motor

    DriveController <|-- TurnSteeringDriveController
    class TurnSteeringDriveController{
        -uint8_t carWidthMm
        -uint8_t carLengthMm

        +loop()

        #setSpeedInternal(int16_t speed)
        #setAngleInternal(int8_t angle)
    }
    TurnSteeringDriveController --> "2 -rear[Left/Right]" Motor
    TurnSteeringDriveController --> "1 -steering" SteerableAxle

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
    Motor --> "1 #unit" MotorSpeedUnit
    Motor --> "0..1 #profile" MotorProfile

    class MotorSpeedUnit {
        <<enumeration>>
        MOTOR_SPEED_UNIT_RATIO
        MOTOR_SPEED_UNIT_RPM
        MOTOR_SPEED_UNIT_CM_PER_SEC
    }

    Motor <|-- MotorDcHBridge
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

    MotorProfile <|-- PolyCurveMotorProfile
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

    SteerableAxle <|-- ServoAxle
    class ServoAxle {
        #Servo servoInstance
        #uint8_t pin
        #uint8_t servoLeft
        #uint8_t servoStraight
        #uint8_t servoRight

        +setAngle(int8_t angle)
        +reattachServo(uint8_t servoPin)
    }
```