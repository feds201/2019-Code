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
  int downPos = 0;

  WPI_TalonSRX shooter{2};
  WPI_TalonSRX master{0};
  WPI_TalonSRX slave{1};
  frc::DigitalInput swi{2};
};

