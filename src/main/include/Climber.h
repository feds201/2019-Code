#pragma once

#include"frc/WPILib.h"
#include"ctre/Phoenix.h"

class Climber {
 public:
  Climber();
  void incStage();
  void decStage();
  void setWheel(double speed);
  void deployArm(double speed);
//Requirements for the function; these are what you have to provide when this function is called

  private:

  void excStage();
  void liftFront();
  void liftBack();
  void liftFront(bool state);
  void liftBack(bool state);
//Requirements for the function; these are what you have to provide when this function is called

  frc::DoubleSolenoid front{0, 0, 0};
  frc::DoubleSolenoid back{0, 0, 0};
  
//The first four control the four pneumatic pistons that lift the robot
//The last one controls the piston that deploys the traction wheel to pull the robot forward

  WPI_TalonSRX tractionwheel{0};
  WPI_TalonSRX armMotor{0};
//Controls the wheel that will pull the robot forward after it lits itself up

  int stage = 0;
};
