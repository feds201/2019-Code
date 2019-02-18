/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Elevator.h"
#include"math.h"

Elevator::Elevator() {

    motor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute);
    motor.SetSelectedSensorPosition(0);
    motor.Set(ControlMode::PercentOutput, 0);

    std::cout << "INFO: ELEVATOR INIT COMPLETE" << std::endl;

    if(!manualMode){

        motor.Config_kP(0, P);
        motor.Config_kI(0, I);
        motor.Config_kD(0, D);

        motor.SetSensorPhase(true);

        std::cout << "INFO: ELEVATOR IN POS MODE" << std::endl;

    }else{
        std::cout << "WARNING: ELEVATOR IN MANUAL MODE" << std::endl;
    }
}

void Elevator::Lift(){

    if(currentPos < 6){
        currentPos++;
        motor.Set(ControlMode::Position, posList[currentPos]);
    }

}

void Elevator::Lower(){

    if(currentPos > 0){
        currentPos--;
        motor.Set(ControlMode::Position, posList[currentPos]);
    }

}

void Elevator::Override(double speed, bool isOverride){

//If the elevator gets stuck and/or is off from the position to insirt ball or cargo, the override will allow the driver to hold and adjust joystick to correct hight.

    isOverridden = isOverride;

    if(isOverride){
    
        if(abs(speed) >= holdVoltage){
            if(speed < 0 && topLimit.Get()){
                motor.Set(ControlMode::PercentOutput, speed);
            }else if(speed > 0 && bottomLimit.Get()){
                motor.Set(ControlMode::PercentOutput, speed);
            }
        }else if(bottomLimit.Get()){
            motor.Set(ControlMode::PercentOutput, holdVoltage);
        }else{
            motor.Set(ControlMode::PercentOutput, 0);
        }
    
        wasOverridden = true;

        overridePos = getEncPos();
    
    }else if(wasOverridden){

        motor.Set(ControlMode::Position, overridePos);
        wasOverridden = false;

    }
}

int Elevator::getEncPos(){

    return motor.GetSelectedSensorPosition();

}

int Elevator::getEncVel(){

    return motor.GetSelectedSensorVelocity();

}

double Elevator::getCurrent(){

    if(motor.GetOutputCurrent() > 50){
        std::cout << "WARNING: HIGH ELEVATOR CURRENT: " << motor.GetOutputCurrent() << std::endl;  
    }

    return motor.GetOutputCurrent();

}

std::string Elevator::getTarget(){

    if(currentPos == 0){
        return "Home";
    }else if(currentPos == 1){
        return "Hatch Low";
    }else if(currentPos == 2){
        return "Cargo Low";
    }else if(currentPos == 3){
        return "Hatch Mid";
    }else if(currentPos == 4){
        return "Cargo Mid";
    }else if(currentPos == 5){
        return "Hatch High";
    }else if(currentPos == 6){
        return "Cargo High";
    }else{
        return "Error: Unknown Pos";
    }

}

int Elevator::getErr(){

    return motor.GetClosedLoopError();

}