/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include"RobotMap.h"


class CargoPickup {
 public:
  CargoPickup();
  void ToggleArm();
  void Intake(double input);
  
 private:
 
  enum poss {Up, Down};

  poss currentPos = Up;

  int masterID;

  int upPos = 0; //Dont Know Yet
  int downPos = 0; //Don't Know Yet

  WPI_TalonSRX shooter{shooterMotor};
  WPI_TalonSRX master{masterWristMotor};
  WPI_TalonSRX slave{slaveWristMotor};
  frc::DigitalInput swi{limitSwitch};
};

