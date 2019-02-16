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

    if(posMode){

        motor.Config_kP(0, P);
        motor.Config_kI(0, I);
        motor.Config_kD(0, D);

        std::cout << "INFO: ELEVATOR IN POS MODE" << std::endl;

    }else{
        std::cout << "INFO: ELEVATOR IN REG MODE" << std::endl;
    }
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

int Elevator::Switch(){

//The switch will change from hatch to cargo or cargo to hatch mode. Once changed, the position will go straight to home.

    if(currentMode == Hatch){
        currentMode = Cargo;
        isAuto = false;
    }else if(currentMode == Cargo){
        currentMode = Auto;
        isAuto = true;
    }else if(currentMode == Auto){
        currentMode = Hatch;
        isAuto = false;
    }

    Home();

    if(currentMode == Hatch){
        return 0;
    }else if(currentMode == Cargo){
        return 1;
    }else{
        return 2;
    }
   
}

void Elevator::Move(int position){

//Once the elevator hits the top limit switch, the elevator can not go more up. When the elevator hits the bottom limit switch, the elevator can not go more down.

if(!configMode & !posMode){

    if(bottomLimit.Get()){
        motor.SetSelectedSensorPosition(0);
    }

    if(isMoving && position != 0){

        if(goingUp && motor.GetSelectedSensorPosition() < posList[position-1] && !topLimit.Get()){
            if(abs(posList[position-1] - motor.GetSelectedSensorPosition()) < bufferDist){
                motor.Set(ControlMode::PercentOutput, goingUpApproachSpeed);
            }
            motor.Set(ControlMode::PercentOutput, goingUpCruseSpeed);
        }else if(goingUp && topLimit.Get()){
            isMoving = false;
        }else if(goingUp && motor.GetSelectedSensorPosition() >= posList[position-1]){
            isMoving = false;
        }

        if(!goingUp && motor.GetSelectedSensorPosition() > posList[position-1] && !bottomLimit.Get()){
             if(abs(posList[position-1] - motor.GetSelectedSensorPosition()) < bufferDist){
                motor.Set(ControlMode::PercentOutput, goingDownApproachSpeed);
            }
            motor.Set(ControlMode::PercentOutput, goingDownCruseSpeed);
        }else if(!goingUp && bottomLimit.Get()){
            isMoving = false;
        }else if(!goingUp && motor.GetSelectedSensorPosition() <= posList[position-1]){
            isMoving = false;
        }


    }else if(isMoving && position == 0){
        if(!bottomLimit.Get()){
            motor.Set(ControlMode::PercentOutput, goingDownCruseSpeed);
        }else{
           isMoving = false;
        }
    }
        if(!bottomLimit.Get() && !isMoving){
            motor.Set(ControlMode::PercentOutput, holdVoltage);
        }else if(bottomLimit.Get() && !isMoving){
            motor.Set(ControlMode::PercentOutput, 0);
        }
  }


//
//
//
//
//
//

if(!configMode && posMode){

    if(bottomLimit.Get()){
        motor.SetSelectedSensorPosition(0);
    }

    if(position != 0){
        motor.Set(ControlMode::Position, posList[position-1]);
    }else{
        motor.Set(ControlMode::Position, 0);
    }
    
  }



}

void Elevator::Refresh(bool isHatch, bool isCargo){

    if(currentPos > 5 || currentPos < 0){
        std::cout << "ERROR: ELEVATOR POS OUT OF BOUNDS: " << currentPos << std::endl;
    }

    if(currentMode == Auto || isAuto){
        if(isHatch){
            currentMode = Hatch;
        }else if(isCargo){
            currentMode = Cargo;
        }else{
            currentMode = Auto;
            }
        }
    


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
        return "Hatch Mid";
    }else if(currentPos == 2){
        return "Hatch High";
    }else if(currentPos == 3){
        return "Cargo Low";
    }else if(currentPos == 4){
        return "Cargo Mid";
    }else if(currentPos == 5){
        return "Cargo High";
    }else{
        return "Error Unknown Pos";
    }

}