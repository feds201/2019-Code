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

class Robot : public frc::TimedRobot{
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;
  void singleOpMode();
  void Teleop();
  Robot():TimedRobot(0.06){}

 private:
  
  Drivetrain Drive{};
  Elevator Ele{};
  CargoPickup Cargo{};
  HatchManipulator Hatch{};
  Climber Climb{};

  frc::PowerDistributionPanel pdp{PDPID};
  
  frc::AnalogInput pressure{pressureTransID};

  frc::Joystick Driver{0};
  frc::Joystick Op{1};
  //frc::Compressor comp{0};

  //cs::UsbCamera Camera;

  cs::UsbCamera camera{"Camera", "/dev/video0"};
  cs::MjpegServer mjpegerver{"serve_usbcam_0", 1181};

  int logTicker = 0;
  int logInterval = 20;
  bool logThisTime = false;
  bool singleOPDriverMode = true;
  bool hatchAndBallLockoutOverride = HATCH_AND_BALL_AUTO_LOCKOUT;

  bool isHatchPresent;
  bool isCargoPresent;
  bool isCargoWristDown;


  int press;

  int vibrationTicker = 0;
  int eleMode = 0;

};
