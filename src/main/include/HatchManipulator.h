/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "RobotMap.h"

class HatchManipulator {
 public:

  HatchManipulator();
  void Run(bool state, bool isCargoMode);
  void Abort();
  bool hatchOn();
  void moveBackplate(bool inOrOut);
  void moveBackplate();
  void Eject(bool inOrOut);
  bool pinsOut();

  private:

  enum thingsThatCanHappen {PickingUp, DroppingOff, Nothing};

  thingsThatCanHappen whatIsHappening = Nothing;

  bool lastState = false;
  bool wasHatchOn = false;
  int dropOffStage = 0; // 0 = Not Dropping Off, 1 = Plate Extended, 2 = Ejector Pins Out

  

  frc::DoubleSolenoid Backplate{PCMID, plateExtend, plateRetract};
  frc::DoubleSolenoid Ejector{PCMID, ejectorsExtend , ejectorsRetract};

  frc::DigitalInput isHatch{hatchDetectionLimitSwitch};

};