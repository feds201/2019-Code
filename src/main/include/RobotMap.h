#pragma once

#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include <iostream>

//
//
//
//General
//a
//
//

constexpr int PCMID = 0;
constexpr int PDPID = 0;
constexpr int PCM2ID = 6;

constexpr bool CONFIG_MODE = false;

constexpr bool SINGLE_OPERATOR_MODE = false;

constexpr bool HATCH_AND_BALL_AUTO_LOCKOUT = false;

constexpr int fwdChl = 1;
constexpr int trnChl = 4;

constexpr int shiftBtn = 1; //A

constexpr int hatchRun = 1; //A
constexpr int hatchAbort = 2; //B

constexpr int eleLift = 6; //Right Bumper
constexpr int eleLower = 5; //Left Bumper
constexpr int eleOverride = 3; //X
constexpr int eleOverrideAxis = 1; //Axis 1

constexpr int climbIncStage = 8; //Menu
constexpr int climbDecStage = 7; //View
constexpr int climbWheelAxis = 1; //Axis 1

constexpr int cargoToggle = 4; //Y
constexpr int cargoIntakeAxis = 2; //Eject Is Right Trigger, Intake Is Left Trigger
constexpr int cargoEjectAxis = 3;

constexpr int climberWheelsFwdAxis = 2;
constexpr int climberWheelsRevAxis = 3;

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

constexpr double Drivetrain_P = 0;
constexpr double Drivetrain_I = 0;
constexpr double Drivetrain_D = 0;

constexpr int maxSpeedLo = 0;
constexpr int maxSpeedHi = 0;
constexpr double gearRatioLo = 0;
constexpr double gearRatioHi = 0;
constexpr int wheelR = 0;

constexpr bool ACCEL_LIMIT_ENABLED = false;

constexpr bool drivetrainVoltageControl = true;
constexpr bool drivetrainAutoheading = false;

constexpr int setShifterFirstGear = 5;
constexpr int setShifterSecondGear = 4;


//
//
//
//Elevator
//
//
//

constexpr bool elevatorManualMode = false;

constexpr int elevatorMotor = 5;
constexpr int topLimitSwitch = 1;
constexpr int bottomLimitSwitch = 5;

constexpr double upCruseSpeed = 1;
constexpr double upApproachSpeed = 0.4;
constexpr double downCruseSpeed = -0.2;
constexpr double downApproachSpeed = 0;
constexpr double holdVoltage = 0.15;

constexpr double elevator_P = 0.7;
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
constexpr int retractFront = 3;
constexpr int extendBack = 2;
constexpr int retractBack = 1;
constexpr int tractionWheelID = 3;
constexpr int tractionWheel2ID = 1;

//
//
//
//Cargo Pickup
//
//
//

constexpr int shooterMotor = 2;
constexpr int masterWristMotor = 4;
constexpr int slaveWristMotor = 0;
constexpr int limitSwitch = 2; 

constexpr double Cargo_P = 0.6;
constexpr double Cargo_I = 0;
constexpr double Cargo_D = 0;

//
//
//
//Hatch Pickup
//
//
//

constexpr int plateExtend = 1;
constexpr int plateRetract = 0;
constexpr int ejectorsExtend = 2;
constexpr int ejectorsRetract = 3;
constexpr int hatchDetectionLimitSwitch = 3;

//
//
//