#include "Drivetrain.h"
#include"frc/WPILib.h"
#include<iostream>
#include"Math.h"

Drivetrain::Drivetrain(){

	gyro.SetFusedHeading(0, 10);

	countsPerIn = encCountsPerRev*(gearRatio*(1/(2*pi*wheelR)));

	metersPerCount = 1/(countsPerIn*inchesPerMeter);

	metersPerCount *= maxSpeed;

	maxV = metersPerCount*10;

	accelTime = maxV/MAXAccel;

    if(ACCEL_LIMIT_ENABLED){

	Left2.ConfigClosedloopRamp(accelTime, 10);
	Right2.ConfigClosedloopRamp(accelTime, 10);

    }else{
        std::cout << "WARNING: ACCEL LIMIT NOT ENABLED" << std::endl;
    }
	Left1.Set(ControlMode::PercentOutput, 0);
	Left2.Set(ControlMode::Follower, Left1.GetDeviceID());
	Right1.Set(ControlMode::PercentOutput, 0);
	Right2.Set(ControlMode::Follower, Right1.GetDeviceID());

    lmasterID = Left1.GetDeviceID();
    rmasterID = Right1.GetDeviceID();

	Left2.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);
	Right2.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);

	Left1.Config_kP(0, P);
	Left1.Config_kI(0, I);
	Left1.Config_kD(0, D);
	
    Right1.Config_kP(0, P);
    Right1.Config_kI(0, I);
    Right1.Config_kD(0, D);

	std::cout << "Drivetrain Init Done" << std::endl;

}

void Drivetrain::Drive(float fwd, float trn, bool autoHeading, bool voltageControl){

	if(trn != 0 && autoHeading){
		tempHeadingZero = gyro.GetFusedHeading();
    }
	if(autoHeading){
        angleErr = gyro.GetFusedHeading()-tempHeadingZero;

		leftSet = ((trn+(angleErr*0.07)) - fwd);
		rightSet = ((trn+(angleErr*0.07)) + fwd);
	}else{
		leftSet = (trn-fwd);
		rightSet = (trn+fwd);
	}

    if(!voltageControl){
        leftSet *= maxSpeed;
        rightSet *= maxSpeed;
    }

	Set(leftSet, rightSet, voltageControl);



}

void Drivetrain::Set(float Left, float Right, bool voltageControl){

    if(!voltageControl){
	    Left2.Set(ControlMode::Velocity, Left);
	    Right2.Set(ControlMode::Velocity, Right);
    }else{
        Left2.Set(ControlMode::PercentOutput, Left);
	    Right2.Set(ControlMode::PercentOutput, Right);
    }
	Left1.Set(ControlMode::Follower, lmasterID);
	Right1.Set(ControlMode::Follower, rmasterID);

}

int * Drivetrain::GetEncVel(){

	EncVel[0] = Left2.GetSelectedSensorVelocity(0);
	EncVel[1] = Right2.GetSelectedSensorVelocity(0);

	return EncVel;

}

int * Drivetrain::GetEncPos(){

	EncPos[0] = Left2.GetSelectedSensorPosition(0);
	EncPos[1] = Right2.GetSelectedSensorPosition(0);

	return EncPos;

}

void Drivetrain::SetEncPos(double left, double right){

	Left2.SetSelectedSensorPosition(left, 0, 10);
	Right2.SetSelectedSensorPosition(right, 0, 10);

	frc::Wait(0.1);

	Left2.SetSelectedSensorPosition(left, 0, 10);
	Right2.SetSelectedSensorPosition(right, 0, 10);

}

double Drivetrain::getGyroAngle(){
	return gyro.GetFusedHeading();
}

void Drivetrain::setGyroAngle(double angle){
	gyro.SetFusedHeading(angle, 10);
}

float * Drivetrain::GetCurr(){

	current[0] = Left1.GetOutputCurrent();
	current[1] = Left2.GetOutputCurrent();
	current[2] = Right1.GetOutputCurrent();
	current[3] = Right2.GetOutputCurrent();

	return current;
}

float * Drivetrain::GetSetpoint(){

	setpt[0] = leftSet;
	setpt[1] = rightSet;

}