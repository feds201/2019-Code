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
    
    master.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute);
    master.SetSelectedSensorPosition(0);

    master.SetSensorPhase(true);

    master.Config_kP(0, Cargo_P, 10); //Don't Know Yet https://phoenix-documentation.readthedocs.io/en/latest/ch16_ClosedLoop.html
    master.Config_kI(0, Cargo_I, 10); //THE SECOND NUMBER IS THE CONSTANT VALUE TO TUNE
    master.Config_kD(0, Cargo_D, 10);

    master.Set(ControlMode::Position, homePos);
    slave.Set(ControlMode::Follower, masterID);

    std::cout << "INFO: CARGO PICKUP INIT COMPLETE" << std::endl;

}

void CargoPickup::ToggleArm() {
   
if(!CONFIG_MODE && !hatchMode && !eleAboveThreshold){

   if(currentPos == Up && !eleAboveThreshold){
       master.Set(ControlMode::Position, homePos);
       slave.Set(ControlMode::Follower, masterID);
       currentPos = Home;
   }else if(currentPos == Up && eleAboveThreshold){
        master.Set(ControlMode::Position, downPos);
       slave.Set(ControlMode::Follower, masterID);
       currentPos = Down;
   }else if(currentPos == Home){
       master.Set(ControlMode::Position, downPos);
       slave.Set(ControlMode::Follower, masterID);
       currentPos = Down;
   }else if(currentPos == Down){
        master.Set(ControlMode::Position, upPos);
        slave.Set(ControlMode::Follower, masterID);
        currentPos = Up;
       }
   }else if(hatchMode){
    master.Set(ControlMode::Position, upPos);
    slave.Set(ControlMode::Follower, masterID);
    currentPos = Up;
}

}

void CargoPickup::Intake(double intakeTrigger, double ejectTrigger, bool isHatchMode, int eleEncPos) {
    
    setPt = intakeTrigger-ejectTrigger;

    hatchMode = isHatchMode;

    frc::SmartDashboard::PutBoolean("Cargo Lock Enabled", hasCargo);

    if(abs(eleEncPos) > 10 && !eleAboveThreshold){
        eleAboveThreshold = true;
        if(currentPos == Home){
            master.Set(ControlMode::Position, upPos);
            slave.Set(ControlMode::Follower, masterID);
            currentPos = Up;
        }
    }else if(abs(eleEncPos) <= 10){
        eleAboveThreshold = false;
    }

if(!hatchMode){

 if(ejectTrigger > 0.2 && ejectTrigger < 0.5){
     hasCargo = false;
 }

 if(setPt == 0 && !hasCargo){
        shooter.Set(ControlMode::PercentOutput, 0);
    }else if(setPt == 0 && hasCargo){
        shooter.Set(ControlMode::PercentOutput, -0.07);
    }

    if (setPt > 0.5) {
            shooter.Set(ControlMode::PercentOutput, setPt);
            hasCargo = false;
    }

    if (setPt < 0 && !hasCargo) {
            shooter.Set(ControlMode::PercentOutput, setPt);
            if(shooter.GetOutputCurrent() > 40){
                hasCargo = true;
            }
    }else if(setPt < 0 && hasCargo){
        shooter.Set(ControlMode::PercentOutput, -0.07);
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