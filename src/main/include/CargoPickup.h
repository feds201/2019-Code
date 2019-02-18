/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include"RobotMap.h"


class CargoPickup {
 public:
  CargoPickup();
  void ToggleArm();
  void Intake(double intakeTrigger, double ejectTrigger, bool isHatchMode, int eleEncPos);
  int getEncPos();
  int getEncVel();
  double * getWristCurrent();
  double getWheelsCurrent();
  bool isCargo();
  int getErr();
  bool isDown();
  
 private:

  enum poss {Up, Down, Home};

  poss currentPos = Up;

  int masterID;

  bool hasCargo = false;

  bool hatchMode = false;

  bool eleAboveThreshold = false;

  double setPt;

  int upPos = 70;
  int downPos = 900;
  int homePos = -50;

  double current[2];

  WPI_TalonSRX shooter{shooterMotor};
  WPI_TalonSRX master{masterWristMotor};
  WPI_TalonSRX slave{slaveWristMotor};
  frc::DigitalInput swi{limitSwitch};
};

