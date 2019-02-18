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

    master.Config_kP(0, P, 10); //Don't Know Yet https://phoenix-documentation.readthedocs.io/en/latest/ch16_ClosedLoop.html
    master.Config_kI(0, I, 10); //THE SECOND NUMBER IS THE CONSTANT VALUE TO TUNE
    master.Config_kD(0, D, 10);

    master.Set(ControlMode::Position, 0);
    slave.Set(ControlMode::Follower, masterID);

    std::cout << "INFO: CARGO PICKUP INIT COMPLETE" << std::endl;

}

//How to know when arm is completely up or down?
//Dont know which direction to set motors 
void CargoPickup::ToggleArm() {
   
if(!configMode && !hatchMode){

   if(currentPos == Up){
       master.Set(ControlMode::Position, downPos);
       slave.Set(ControlMode::Follower, masterID);
       currentPos = Down;
   }else{
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

void CargoPickup::Intake(double intakeTrigger, double ejectTrigger, bool isHatchMode) {
    
    setPt = intakeTrigger-ejectTrigger;

    hatchMode = isHatchMode;

if(!hatchMode){

 if(ejectTrigger > 0.2 && ejectTrigger < 0.5){
     hasCargo = false;
 }

 if(setPt == 0 && !hasCargo){
        shooter.Set(ControlMode::PercentOutput, 0);
    }else if(setPt == 0 && hasCargo){
        shooter.Set(ControlMode::PercentOutput, -0.05);
    }

    if (setPt > 0.5) {
            shooter.Set(ControlMode::PercentOutput, setPt);
            hasCargo = false;
    }

    if (setPt < 0 && !hasCargo) {
            shooter.Set(ControlMode::PercentOutput, setPt);
            if(shooter.GetOutputCurrent() > 30){
                hasCargo = true;
            }
    }else if(setPt < 0 && hasCargo){
        shooter.Set(ControlMode::PercentOutput, -0.05);
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