/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "frc/WPILib.h"
#include <iostream>
#include "ctre/Phoenix.h"

class HatchManipulator {
 public:

  HatchManipulator(int PCM, int intake, int reject);
  void eject();

 private:

  frc::DoubleSolenoid leftSolenoid{0, 0, 1}; //Fix for later
  frc::DoubleSolenoid rightSolenoid{0 , 0 , 1};

};