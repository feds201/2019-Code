/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HatchManipulator.h"

HatchManipulator::HatchManipulator() {

    Backplate.Set(frc::DoubleSolenoid::Value::kReverse);
    Ejector.Set(frc::DoubleSolenoid::Value::kReverse);

    std::cout << "INFO: HATCH MANIPULATOR INIT COMPLETE" << std::endl;

}

void HatchManipulator::Run(bool state, bool isCargoMode){

    if(whatIsHappening == PickingUp && state == false){
        whatIsHappening = Nothing;
    }else if(whatIsHappening == DroppingOff && dropOffStage == 2 && state == false){
        whatIsHappening = Nothing;
        dropOffStage = 0;
    }

    if(whatIsHappening == Nothing && isHatch.Get() && state == true){
        whatIsHappening = PickingUp;
    }

    if(whatIsHappening == Nothing && !isHatch.Get() && state == true && dropOffStage == 0){
        whatIsHappening = DroppingOff;
        dropOffStage = 1;
    }else if(whatIsHappening == DroppingOff && !isHatch.Get() && state == true && lastState == false && dropOffStage == 1){
        dropOffStage = 2;
    }

    if(isCargoMode){
        whatIsHappening = Nothing;
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
    wasHatchOn = !isHatch.Get();

    if(dropOffStage > 2){
        std::cout << "ERROR: HATCH MANIPULATOR STAGE HAS EXCEEDED MAX VALUE" << std::endl;
    }

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

bool HatchManipulator::hatchOn(){

    return !isHatch.Get();

}

void HatchManipulator::moveBackplate(){

    if(Backplate.Get() == frc::DoubleSolenoid::Value::kForward){
        Backplate.Set(frc::DoubleSolenoid::Value::kReverse);
    }else{
        Backplate.Set(frc::DoubleSolenoid::Value::kForward);
    }

}

bool HatchManipulator::pinsOut(){

    if(Ejector.Get() == frc::DoubleSolenoid::Value::kForward){
        return true;
    }else{
        return false;
    }

}