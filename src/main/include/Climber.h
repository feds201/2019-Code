#pragma once

#include"RobotMap.h"

class Climber {
 public:
  Climber();
  void incStage();
  void decStage();
  void setWheel(double speed);
  bool getFrontState();
  bool getBackState();
  int getStage();
//Requirements for the function; these are what you have to provide when this function is called

  private:

  void excStage();
  void liftFront();
  void liftBack();
  void liftFront(bool state);
  void liftBack(bool state);
//Requirements for the function; these are what you have to provide when this function is called

  frc::DoubleSolenoid front{PCM2ID, extendFront, retractFront};
  frc::DoubleSolenoid back{PCM2ID, extendBack, retractBack};
  
//The first four control the four pneumatic pistons that lift the robot
//The last one controls the piston that deploys the traction wheel to pull the robot forward

  WPI_TalonSRX tractionwheel{tractionWheelID};
  WPI_TalonSRX tractionwheel2{tractionWheel2ID};
//Controls the wheel that will pull the robot forward after it lits itself up

  int stage = 0;
};
