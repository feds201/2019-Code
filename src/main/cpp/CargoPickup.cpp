/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CargoPickup.h" //Pulls code from header file

CargoPickup::CargoPickup() {

    masterID = master.GetDeviceID(); //declares masterID as device id of the main wrist motor

    shooter.Set(ControlMode::PercentOutput, 0); //changes the control mode of the shootey wheel motors

    master.ConfigFeedbackNotContinuous(true, 10); //stops the arm from going the wrong way
    
    master.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute); //sets the ID of the encoder for the master wrist motor
    sensorOffset = master.GetSelectedSensorPosition(); //sets what we want as the rest position to zero

    master.SetSensorPhase(false); //turns off this setting

    master.Config_kP(0, Cargo_P, 10); //Don't Know Yet https://phoenix-documentation.readthedocs.io/en/latest/ch16_ClosedLoop.html
    master.Config_kI(0, Cargo_I, 10); //THE SECOND NUMBER IS THE CONSTANT VALUE TO TUNE
    master.Config_kD(0, Cargo_D, 10); //PID Tuning

    master.Set(ControlMode::Position, homePos + sensorOffset); //Changes the control mode of the master wrist motor to follow specific positions
    slave.Follow(master); //this tells the slave wrist motor to follow the commands that the master recieves

    master.ConfigPeakOutputForward(0.4, 10); //limits the speed of the motor going forward
    master.ConfigPeakOutputReverse(-0.5, 10); //limits the speed of the motor going backwards

    master.ConfigContinuousCurrentLimit(40, 10); //limits the current draw of the motor continously
    master.ConfigPeakCurrentLimit(40, 10); //limits the current draw of the motor non-continuously
    master.ConfigPeakCurrentDuration(0, 10); //defines when the current draw becomes considered continuous

    slave.ConfigContinuousCurrentLimit(40, 10); /* ------------------------------------------------------------*/
    slave.ConfigPeakCurrentLimit(40, 10);       /* this does all that stuff that the master does for the slave */
    slave.ConfigPeakCurrentDuration(0, 10);     /* ------------------------------------------------------------*/

    std::cout << "INFO: CARGO PICKUP INIT COMPLETE" << std::endl; //sends the message "INFO: CARGO PICKUP INIT COMPLETE"

}

void CargoPickup::ToggleArm() { //this runs when someone hits the button to change the arm position
   
if(!CONFIG_MODE){ //this always runs

   
   if(currentPos == Home){ //if the current position of the wrist is the Home position
       master.Set(ControlMode::Position, downPos + sensorOffset); //change the arm position to the down position
       slave.Follow(master); //do the same thing with the other motor
       currentPos = Down; //change the variable to down
   }else if(currentPos == Down){ //if the current position of the motor is the Down position
        master.Set(ControlMode::Position, homePos + sensorOffset); //change the arm position to the home position
        slave.Follow(master); //do the same thing with the other motor
        currentPos = Home; //change the variable to home
       }
   }

}

void CargoPickup::Intake(double intakeTrigger, double ejectTrigger, bool isHatchMode, int eleEncPos) { //this is called when the operator tries to pick up cargo
    
    setPt = intakeTrigger-ejectTrigger; //sets this variable to 
    slave.Follow(master); //does whatever the master just did

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