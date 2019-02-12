/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include"CargoPickup.h"
#include"Climber.h"
#include"Drivetrain.h"
#include"Elevator.h"
#include"HatchManipulator.h"
#include"cameraserver/CameraServer.h"
#include"RobotMap.h"
#include"Logger.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;
  void singleOpMode();

 private:
  
  Drivetrain* Drive{};
  Elevator* Ele{};
  CargoPickup* Cargo{};
  HatchManipulator* Hatch{};
  Climber* Climb{};

  frc::PowerDistributionPanel pdp{PDPID};

  frc::Joystick Driver{0};
  frc::Joystick Op{1};
  frc::Joystick* singleOP;

  cs::UsbCamera Camera;

  int logTicker = 0;
  int logInterval = 20;
  bool logThisTime = false;
  bool singleOPDriverMode = true;

  int vibrationTicker = 0;
  int eleMode = 0;

};
