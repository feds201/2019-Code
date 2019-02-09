/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CargoPickup.h"
#include"frc/WPILib.h"

CargoPickup::CargoPickup() {

    masterID = master.GetDeviceID();

    shooter.Set(ControlMode::PercentOutput, 0);
    
    master.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute);
    master.SetSelectedSensorPosition(0);

    master.Config_kP(0, P); //Don't Know Yet https://phoenix-documentation.readthedocs.io/en/latest/ch16_ClosedLoop.html
    master.Config_kI(0, I); //THE SECOND NUMBER IS THE CONSTANT VALUE TO TUNE
    master.Config_kD(0, D);

    master.SetInverted(false);
    slave.SetInverted(true);

    master.Set(ControlMode::Position, 0);
    slave.Set(ControlMode::Follower, masterID);

}

//How to know when arm is completely up or down?
//Dont know which direction to set motors 
void CargoPickup::ToggleArm() {
   
if(!configMode){

   if(currentPos == Up){
       master.Set(ControlMode::Position, downPos);
       slave.Set(ControlMode::Follower, masterID);
       currentPos = Down;
   }else{
       master.Set(ControlMode::Position, upPos);
       slave.Set(ControlMode::Follower, masterID);
       currentPos = Up;
   }
}

}

void CargoPickup::Intake(double input) {
    
if(!configMode){

    if(input == 0){
        shooter.Set(ControlMode::PercentOutput, 0);
    }

    if (input > 0.5) {
        if (swi.Get()){
            shooter.Set(ControlMode::PercentOutput, input);
        }
    }

    if (input < 0) {
        if (!(swi.Get())) {
            shooter.Set(ControlMode::PercentOutput, input);
        }
    }
  }
}   

int CargoPickup::getEncPos(){

    return master.GetSelectedSensorPosition();

}


