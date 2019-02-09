/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HatchManipulator.h"
#include"frc/WPILib.h"

HatchManipulator::HatchManipulator() {}

void HatchManipulator::Run(bool state){

    if(whatIsHappening == PickingUp && state == false){
        whatIsHappening = Nothing;
    }else if(whatIsHappening == DroppingOff && dropOffStage == 2 && state == false){
        whatIsHappening = Nothing;
        dropOffStage = 0;
    }

    if(whatIsHappening == Nothing && !isHatch.Get() && state == true){
        whatIsHappening = PickingUp;
    }

    if(whatIsHappening == Nothing && isHatch.Get() && state == true && dropOffStage == 0){
        whatIsHappening = DroppingOff;
        dropOffStage = 1;
    }else if(whatIsHappening == DroppingOff && isHatch.Get() && state == true && lastState == false && dropOffStage == 1){
        dropOffStage = 2;
    }


    if(whatIsHappening == PickingUp){
       moveBackplate(true);
    }else if(whatIsHappening == DroppingOff && dropOffStage == 1){
       moveBackplate(true);
    }else if(whatIsHappening == DroppingOff && dropOffStage == 2){
        Eject(true);
    }else if(whatIsHappening == Nothing){
        moveBackplate(false);
        Eject(false);
    }

    lastState = state;
    wasHatchOn = isHatch.Get();

}

void HatchManipulator::Abort(){

    whatIsHappening = Nothing;
    dropOffStage = 0;

}

void HatchManipulator::moveBackplate(bool inOrOut){

    if(inOrOut){
        Backplate.Set(frc::DoubleSolenoid::Value::kForward);
    }else{
        Backplate.Set(frc::DoubleSolenoid::Value::kReverse);
    }


}

void HatchManipulator::Eject(bool inOrOut){

    if(inOrOut){
        Ejector.Set(frc::DoubleSolenoid::Value::kForward);
    }else{
        Ejector.Set(frc::DoubleSolenoid::Value::kReverse);
    }


}
