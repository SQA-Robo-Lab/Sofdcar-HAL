#ifndef SOFDCAR_HAL_HPP
#define SOFDCAR_HAL_HPP

#include <distanceSensor/UltrasonicDistanceSensor.hpp>

#include <driveController/FixedWheelDriveController.hpp>
#include <driveController/FixedWheelDriveControllerBoostMode.hpp>
#include <driveController/TurnSteeringDriveController.hpp>

#include <lineDetector/LinearSensorEdgeDetector.hpp>
#include <lineDetector/LinearSensorLineDetector.hpp>
#include <lineDetector/brightnessSensor/BrightnessSensorAnalog.hpp>
#include <lineDetector/brightnessSensor/BrightnessSensorDigital.hpp>

#include <lineFollower/LineFollower.hpp>

#include <motors/MotorDcHBridge.hpp>
#include <motors/PolyCurveMotorProfile.hpp>
#include <motors/ServoAxle.hpp>

#include <rpc/RpcChild.hpp>
#include <rpc/RpcClass.hpp>
#include <rpc/RpcClassMember.hpp>
#include <rpc/RpcManager.hpp>
#include <rpc/RpcRootMember.hpp>
#include <rpc/hal/RpcHal.hpp>

#endif