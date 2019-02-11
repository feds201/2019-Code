#include "Climber.h"

Climber::Climber() {

    front.Set(frc::DoubleSolenoid::Value::kReverse);
    back.Set(frc::DoubleSolenoid::Value::kReverse);
    //This makes sure that all of the pistons are retracted by default

    tractionwheel.Set(ControlMode::PercentOutput, 0);
    armMotor.Set(ControlMode::PercentOutput, 0);
    //This makes sure that the motor for the wheel is off by default

    std::cout << "INFO: CLIMBER INIT COMPLETE" << std::endl;

}

void Climber::incStage(){

    if((stage+1) <= 5){
    stage++;
    }
    excStage();
    //This makes the stage increase by 1 if the resulting stage doesn't go above 5

}
  void Climber::decStage(){

      if((stage-1) >= 0){
      stage--;
    }
      excStage();
    //This makes the stage decrease by 1 if the resulting stage doesn't go below 0

  }
  void Climber::setWheel(double speed){

      if(stage == 2){
      tractionwheel.Set(ControlMode::PercentOutput, speed);
      }else{
        tractionwheel.Set(ControlMode::PercentOutput, 0);
        //This sets the speed of the traction wheel; if it's in any stage but 2, it sets the speed to 0
      }
  }

  void Climber::deployArm(double speed){

      if(stage == 1 ||stage == 2 || stage == 3){
            armMotor.Set(ControlMode::PercentOutput, speed);
        }else{
            armMotor.Set(ControlMode::PercentOutput, 0);
        }

  }

  void Climber::excStage(){

      switch(stage){

          case 1:

            liftFront();
            liftBack();

            break;
            //This case extends all of the pistons on the robot except for the one for the traction wheel
        case 3:
        
            liftFront();

            break;
            //This case retracts the pistons on the front of the robot and retracts the traction wheel
        case 4:

            liftBack();
            break;
            //This case retracts the pistons on the back of the robot

        default :

            liftFront(false);
            liftBack(false);
            deployArm(false);
            //This is the default case for the robot. All of the pistons on the robot are retracted.

      }

  }
  void Climber::liftFront(){

      if(front.Get() == frc::DoubleSolenoid::Value::kReverse){
          front.Set(frc::DoubleSolenoid::Value::kForward);
      }else{
          front.Set(frc::DoubleSolenoid::Value::kReverse);
      }
    //If this is set to true, the front 2 pistons deploy; if it is false, the front 2 pistons retract

  }
  void Climber::liftBack(){

      if(back.Get() == frc::DoubleSolenoid::Value::kReverse){
          back.Set(frc::DoubleSolenoid::Value::kForward);
      }else{
          back.Set(frc::DoubleSolenoid::Value::kReverse);
      }
    //If this is set to true, the back 2 pistons deploy; if it is false, the back 2 pistons retract

  }

void Climber::liftFront(bool state){

      if(state){
          front.Set(frc::DoubleSolenoid::Value::kForward);
      }else{
          front.Set(frc::DoubleSolenoid::Value::kReverse);
      }
    //If this is set to true, the front 2 pistons deploy; if it is false, the front 2 pistons retract

  }
  void Climber::liftBack(bool state){

      if(state){
          back.Set(frc::DoubleSolenoid::Value::kForward);
      }else{
          back.Set(frc::DoubleSolenoid::Value::kReverse);
      }
    //If this is set to true, the back 2 pistons deploy; if it is false, the back 2 pistons retract

  }

  bool Climber::getBackState(){

      if(back.Get() == frc::DoubleSolenoid::Value::kReverse){
          return false;
      }else{
          return true;
      }

  }

  bool Climber::getFrontState(){

      if(front.Get() == frc::DoubleSolenoid::Value::kReverse){
          return false;
      }else{
          return true;
      }

  }

  int Climber::getStage(){

      return stage;

  }