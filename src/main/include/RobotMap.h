#pragma once

#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include <iostream>

//
//
//
//General
//
//
//

constexpr int PCMID = 0;
constexpr int PDPID = 0;

constexpr bool CONFIG_MODE = false;

constexpr bool SINGLE_OPERATOR_MODE = false;

constexpr bool HATCH_AND_BALL_AUTO_LOCKOUT = true;

constexpr int fwdChl = 1;
constexpr int trnChl = 4;

constexpr int shiftBtn = 1; //A

constexpr int hatchRun = 1; //A
constexpr int hatchAbort = 2; //B

constexpr int eleLift = 6; //Right Bumper
constexpr int eleSwitch = 5; //Left Bumper
constexpr int eleOverride = 3; //X
constexpr int eleOverrideAxis = 1; //Axis 1

constexpr int climbIncStage = 8; //Menu
constexpr int climbDecStage = 7; //View
constexpr int climbWheelAxis = 1; //Axis 1

constexpr int cargoToggle = 4; //Y
constexpr int cargoIntakeAxis = 2; //Eject Is Right Trigger, Intake Is Left Trigger
constexpr int cargoEjectAxis = 3;

constexpr int pressureTransID = 0;


//
//
//
//Drivetrain
//
//
//

constexpr int leftDriveMotor1 = 9;
constexpr int leftDirveMotor2 = 8;
constexpr int rightDriveMotor1 = 7;
constexpr int rightDriveMotor2 = 6;

constexpr int Pigeon = 0;

constexpr int Drivetrain_P = 0;
constexpr int Drivetrain_I = 0;
constexpr int Drivetrain_D = 0;

constexpr int maxRobotVelocityLo = 0;
constexpr int maxRobotVelocityHi = 0;
constexpr double gearRatioToEncodersFromWheelsLowGear = 0;
constexpr double gearRatioToEncodersFromWheelsHighGear = 0;
constexpr int wheelRadius = 0;

constexpr bool accelLimit = false;

constexpr bool drivetrainVoltageControl = true;
constexpr bool drivetrainAutoheading = false;

constexpr int setShifterFirstGear = 0;
constexpr int setShifterSecondGear = 0;


//
//
//
//Elevator
//
//
//

constexpr bool elevatorPosMode = false;

constexpr int elevatorMotor = 5;
constexpr int topLimitSwitch = 1;
constexpr int bottomLimitSwitch = 0;

constexpr double upCruseSpeed = 1;
constexpr double upApproachSpeed = 0.4;
constexpr double downCruseSpeed = -0.2;
constexpr double downApproachSpeed = 0;
constexpr double holdVoltage = 0.1;

constexpr double elevator_P = 0;
constexpr double elevator_I = 0;
constexpr double elevator_D = 0;

//
//
//
//Climber
//
//
//

constexpr int extendFront = 0;
constexpr int retractFront = 0;
constexpr int extendBack = 0;
constexpr int retractBack = 0;
constexpr int tractionWheelID = 0;
constexpr int armMotorID = 0;

//
//
//
//Cargo Pickup
//
//
//

constexpr int shooterMotor = 2;
constexpr int masterWristMotor = 3;
constexpr int slaveWristMotor = 4;
constexpr int limitSwitch = 2;

constexpr int Cargo_P = 0.6;
constexpr int Cargo_I = 0;
constexpr int Cargo_D = 0;

//
//
//
//Hatch Pickup
//
//
//

constexpr int plateExtend = 0;
constexpr int plateRetract = 0;
constexpr int ejectorsExtend = 0;
constexpr int ejectorsRetract = 0;
constexpr int hatchDetectionLimitSwitch = 3;

//
//
//