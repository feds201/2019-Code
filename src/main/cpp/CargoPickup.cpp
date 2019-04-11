/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CargoPickup.h"

CargoPickup::CargoPickup() {

    masterID = master.GetDeviceID();

    shooter.Set(ControlMode::PercentOutput, 0);

    master.ConfigFeedbackNotContinuous(true, 10);
    
    master.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute);
    sensorOffset = master.GetSelectedSensorPosition();

    master.SetSensorPhase(false);

    master.Config_kP(0, Cargo_P, 10); //Don't Know Yet https://phoenix-documentation.readthedocs.io/en/latest/ch16_ClosedLoop.html
    master.Config_kI(0, Cargo_I, 10); //THE SECOND NUMBER IS THE CONSTANT VALUE TO TUNE
    master.Config_kD(0, Cargo_D, 10);

    master.Set(ControlMode::Position, homePos + sensorOffset);
    slave.Follow(master);

    master.ConfigPeakOutputForward(0.4, 10);
    master.ConfigPeakOutputReverse(-0.5, 10);

    master.ConfigContinuousCurrentLimit(40, 10);
    master.ConfigPeakCurrentLimit(40, 10);
    master.ConfigPeakCurrentDuration(0, 10);

    slave.ConfigContinuousCurrentLimit(40, 10);
    slave.ConfigPeakCurrentLimit(40, 10);
    slave.ConfigPeakCurrentDuration(0, 10);

    std::cout << "INFO: CARGO PICKUP INIT COMPLETE" << std::endl;

}

void CargoPickup::ToggleArm() {
   
if(!CONFIG_MODE){

   
   if(currentPos == Home){
       master.Set(ControlMode::Position, downPos + sensorOffset);
       slave.Follow(master);
       currentPos = Down;
   }else if(currentPos == Down){
        master.Set(ControlMode::Position, homePos + sensorOffset);
        slave.Follow(master);
        currentPos = Home;
       }
   }

}

void CargoPickup::Intake(double intakeTrigger, double ejectTrigger, bool isHatchMode, int eleEncPos) {
    
    setPt = intakeTrigger-ejectTrigger;
    slave.Follow(master);

    hatchMode = isHatchMode;

    frc::SmartDashboard::PutNumber("Arm Percent Out", master.GetMotorOutputPercent());

    frc::SmartDashboard::PutBoolean("Cargo Lock Enabled", hasCargo);

    if(abs(eleEncPos) > 6000 && !eleAboveThreshold){
        eleAboveThreshold = true;
    }else if(abs(eleEncPos) <= 6000){
        eleAboveThreshold = false;
    }

    if(eleAboveThreshold && currentPos == Home){
        master.Set(ControlMode::Position, upPos + sensorOffset);
        slave.Follow(master);
        currentPos = Up;
    }else if(!eleAboveThreshold && currentPos == Up){
        master.Set(ControlMode::Position, homePos + sensorOffset);
        slave.Follow(master);
        currentPos = Home;
    }

if(!hatchMode){

 if(ejectTrigger > 0.2 && ejectTrigger < 0.5){
     hasCargo = false;
 }

 if(setPt == 0 && !hasCargo){
        shooter.Set(ControlMode::PercentOutput, 0);
    }else if(setPt == 0 && hasCargo){
        shooter.Set(ControlMode::PercentOutput, -0.1);
    }

    if (setPt > 0.5) {
            shooter.Set(ControlMode::PercentOutput, setPt);
            hasCargo = false;
    }

    if (setPt < 0 && !hasCargo) {
            shooter.Set(ControlMode::PercentOutput, setPt*0.66);
            if(shooter.GetOutputCurrent() > 32){
                hasCargo = true;
            }
    }else if(setPt < 0 && hasCargo){
        shooter.Set(ControlMode::PercentOutput, -0.1);
    }
}else{

    shooter.Set(ControlMode::PercentOutput, 0);

}




  }  

int CargoPickup::getEncPos(){

    return master.GetSelectedSensorPosition();

}

int CargoPickup::getEncVel(){

    return master.GetSelectedSensorVelocity();

}

double * CargoPickup::getWristCurrent(){

    current[0] = master.GetOutputCurrent();
    current[1] = slave.GetOutputCurrent();

    if(current[0] > 50 || current[1] > 50){

        std::cout << "WARNING: HIGH WRIST CURRENT" << std::endl;

    }

    return current;

}

double CargoPickup::getWheelsCurrent(){

    return shooter.GetOutputCurrent();

}

bool CargoPickup::isCargo(){

    return hasCargo;

}

int CargoPickup::getErr(){

    return master.GetClosedLoopError();

}

bool CargoPickup::isDown(){

    if(currentPos == Up){
        return false;
    }else{
        return true;
    }

}

float CargoPickup::getOutput(){

    return master.GetMotorOutputPercent();

}

  double CargoPickup::getClosedLoopTarget(){

      if(currentPos == Home){
          return homePos;
      }else if(currentPos == Up){
          return upPos;
      }else if(currentPos == Down){
          return downPos;
      }else{
          return -1;
      }


  }
  double CargoPickup::getIAccum(){

      return master.GetIntegralAccumulator();


  }
  double CargoPickup::getDirivErr(){

      return master.GetErrorDerivative();


  }

  void CargoPickup::forceArmReset(bool on){

      if(on){
          master.Set(ControlMode::PercentOutput, -0.3);
      }if(!on && wasOnOn){
          master.SetSelectedSensorPosition(0);
          master.Set(ControlMode::Position, homePos + sensorOffset);
      }

    wasOnOn = on;


  }