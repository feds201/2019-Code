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
    motor.ConfigPeakOutputReverse(-0.3); //Change Back To -0.3 When Ready


    std::cout << "INFO: ELEVATOR INIT COMPLETE" << std::endl;

    if(!elevatorManualMode){

        motor.Config_kP(0, elevator_P);
        motor.Config_kI(0, elevator_I);
        motor.Config_kD(0, elevator_D);

        motor.SetSensorPhase(true);

        std::cout << "INFO: ELEVATOR IN POS MODE" << std::endl;

    }else{
        std::cout << "WARNING: ELEVATOR IN MANUAL MODE" << std::endl;
    }
}

void Elevator::Lift(){

    if(currentPos != 5 && !elevatorManualMode && !pinsOut){
        currentPos++;
        motor.Set(ControlMode::Position, posList[currentPos]);
    }

}

void Elevator::Lower(bool hasHatch){

    if(currentPos != 0 && !(currentPos == 1 && hasHatch) && !elevatorManualMode && !pinsOut){
        currentPos--;
        motor.Set(ControlMode::Position, posList[currentPos]);
    }

}

void Elevator::Override(double speed, bool isOverride, bool arePinsOut){

//If the elevator gets stuck and/or is off from the position to insirt ball or cargo, the override will allow the driver to hold and adjust joystick to correct hight.

    isOverridden = isOverride;

    frc::SmartDashboard::PutBoolean("Is Bottom Depressed", !bottomLimit.Get());
     frc::SmartDashboard::PutBoolean("Is Top Depressed", topLimit);
     frc::SmartDashboard::PutNumber("Elevator Output Percent", motor.GetMotorOutputPercent());

    pinsOut = arePinsOut;

     if(!bottomLimit.Get()){
         motor.SetSelectedSensorPosition(0);
     }

    if(!elevatorManualMode && speed > 0){
        speed /= 2;
    }else if(!elevatorManualMode && speed < 0){
        speed /= 6;
    }


    if(abs(getEncPos()) > 26500){
        topLimit = true;
    }

if(isOverride && !pinsOut){

        if(abs(speed) >= holdVoltage){
            if(speed > 0 && !topLimit){
                motor.Set(ControlMode::PercentOutput, speed);
            }else if(speed < 0  && bottomLimit.Get()){
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
        return "Home / Cargo Low";
    }else if(currentPos == 1){
        return "Hatch Sandstorm Low";
    }else if(currentPos == 2){
        return "Hatch Low";
    }else if(currentPos == 3){
        return "Cargo Mid";
    }else if(currentPos == 4){
        return "Hatch Mid";
    }else if(currentPos == 5){
        return "Cargo High";
    }else{
        return "Error: Unknown Pos";
    }

}

int Elevator::getErr(){

    return motor.GetClosedLoopError();

}

float Elevator::getOutput(){

    return motor.GetMotorOutputPercent();

}

double Elevator::getClosedLoopTarget(){

      return 0;


  }
  double Elevator::getIAccum(){

      return motor.GetIntegralAccumulator();


  }
  double Elevator::getDirivErr(){

      return motor.GetErrorDerivative();


  }

  void Elevator::down2k(){

    if(currentPos != 0 && getEncPos() > 2000){
      motor.Set(ControlMode::Position, posList[currentPos]-2000);
    }
  }