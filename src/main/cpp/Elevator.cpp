/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Elevator.h"
#include"frc/WPILib.h"
#include"ctre/Phoenix.h"
#include"math.h"

Elevator::Elevator() {

    motor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute);
    motor.SetSelectedSensorPosition(0);
    motor.Set(ControlMode::PercentOutput, 0);

}

void Elevator::Home(){

//Home is also known as 0 position to stimulate the lowest it can go. The lowest it can go is also the lowest hatch needs.

    goingUp = true;
    isMoving = true;
    
    currentPos = 0;

}

void Elevator::Lift(){

//For when the mode is in hatch mode, a button is used to go up one untill at max hight. Once at max height, the same button will be used to go down then stop at home.

    if(currentMode == Hatch && currentPos == 2){ 
        goingUp = false;
    }else if(currentMode == Hatch && currentPos == 0){
        goingUp = true;
    }else if(currentMode == Cargo && currentPos == 5){
        goingUp = false;
    }else if(currentMode == Cargo && currentPos == 0){
        goingUp = true;
    }

//For when the mode is in cargo mode, a button is used to go up one untill at max hight. Once at max hight, the same button will be used to go down then stop at low hight.

    if(currentMode == Hatch && currentPos < 2 && goingUp){
        currentPos++;
    }else if(currentMode == Hatch && currentPos > 0 && !goingUp){
       currentPos--;
    }else if(currentMode == Cargo && currentPos < 5 && goingUp){
        currentPos++;
    }else if(currentMode == Cargo && currentPos > 3 && !goingUp){
        currentPos--;
    }else if(currentMode == Cargo && currentPos == 3 && !goingUp){
        currentPos = 0;
    }

    isMoving = true;

}

bool Elevator::Switch(){

//The switch will change from hatch to cargo or cargo to hatch mode. Once changed, the position will go straight to home.

    if(currentMode == Hatch){
        currentMode = Cargo;
    }else{
        currentMode = Hatch;
    }

    Home();

    if(currentMode == Hatch){
        return true;
    }else{
        return false;
    }
   
}

void Elevator::Move(int position){

//Once the elevator hits the top limit switch, the elevator can not go more up. When the elevator hits the bottom limit switch, the elevator can not go more down.

    if(bottomLimit.Get()){
        motor.SetSelectedSensorPosition(0);
    }

    if(isMoving && position != 0){

        if(goingUp && motor.GetSelectedSensorPosition() < posList[position-1] && !topLimit.Get()){
            if(abs(posList[position-1] - motor.GetSelectedSensorPosition()) < bufferDist){
                motor.Set(ControlMode::PercentOutput, 0.4);
            }
            motor.Set(ControlMode::PercentOutput, 1);
        }else if(goingUp && topLimit.Get()){
            isMoving = false;
        }else if(goingUp && motor.GetSelectedSensorPosition() >= posList[position-1]){
            isMoving = false;
        }

        if(!goingUp && motor.GetSelectedSensorPosition() > posList[position-1] && !bottomLimit.Get()){
             if(abs(posList[position-1] - motor.GetSelectedSensorPosition()) < bufferDist){
                motor.Set(ControlMode::PercentOutput, 0);
            }
            motor.Set(ControlMode::PercentOutput, -0.2);
        }else if(!goingUp && bottomLimit.Get()){
            isMoving = false;
        }else if(!goingUp && motor.GetSelectedSensorPosition() <= posList[position-1]){
            isMoving = false;
        }


    }else if(isMoving && position == 0){
        if(!bottomLimit.Get()){
            motor.Set(ControlMode::PercentOutput, -0.2);
        }else{
           isMoving = false;
        }
    }
        if(!bottomLimit.Get() && !isMoving){
            motor.Set(ControlMode::PercentOutput, 0.2);
        }else if(bottomLimit.Get() && !isMoving){
            motor.Set(ControlMode::PercentOutput, 0);
        }
    }

void Elevator::Refresh(){

    if(!isOverridden){
        Move(currentPos);
    }
}

void Elevator::Override(double speed, bool isOverride){

//If the elevator gets stuck and/or is off from the position to insirt ball or cargo, the override will allow the driver to hold and adjust joystick to correct hight.

    isOverridden = isOverride;

    if(isOverride){

        isMoving = true;
        currentPos = 0;
    
        if(speed != 0){
            if(speed > 0 && !topLimit.Get()){
                motor.Set(ControlMode::PercentOutput, speed);
            }else if(speed < 0 && !bottomLimit.Get()){
                motor.Set(ControlMode::PercentOutput, speed);
            }
        }else if(!bottomLimit.Get()){
            motor.Set(0.2);
        }else{
            motor.Set(0);
        }
    
    
    }
}


