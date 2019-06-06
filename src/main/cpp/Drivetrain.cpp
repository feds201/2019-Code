#include "Drivetrain.h"
#include"Math.h"

Drivetrain::Drivetrain(){

	gyro.SetFusedHeading(0, 10);

	if(ACCEL_LIMIT_ENABLED){

	//Low Gear Calculations

	metersPerCountLo = 1/(countsPerInLo*inchesPerMeter);

	metersPer100msLo = maxSpeedLo*metersPerCountLo;

	maxVLo = metersPer100msLo/10;

	accelTimeLo = maxVLo/MAXAccel;

	//High Gear Calculations

	metersPerCountHi = 1/(countsPerInHi*inchesPerMeter);

	metersPer100msHi = maxSpeedHi*metersPerCountHi;

	maxVHi = metersPer100msHi/10;

	accelTimeHi = maxVHi/MAXAccel;

	Left1.ConfigClosedloopRamp(accelTimeLo, 10);
	Right1.ConfigClosedloopRamp(accelTimeLo, 10);
	
	Left1.ConfigOpenloopRamp(accelTimeLo, 10);
	Right1.ConfigOpenloopRamp(accelTimeLo, 10);



    }else{
        std::cout << "WARNING: ACCEL LIMIT NOT ENABLED" << std::endl;
    }

	if(drivetrainVoltageControl){
		std::cout << "INFO: DRIVETRAIN IN VOLTAGE MODE" << std::endl;
	}else{
		std::cout << "INFO: DRIVETRAIN IN VEL MODE" << std::endl;
	}

	if(drivetrainAutoheading){
		std::cout << "INFO: DRIVETRAIN AUTOHEADING ENABLED" << std::endl;
	}else{
		std::cout << "INFO: DRIVETRAIN AUTOHEADING DISABLED" << std::endl;
	}

	Left1.Set(ControlMode::PercentOutput, 0);
	Left2.Set(ControlMode::Follower, Left1.GetDeviceID());
	Right1.Set(ControlMode::PercentOutput, 0);
	Right2.Set(ControlMode::Follower, Right1.GetDeviceID());

    lmasterID = Left1.GetDeviceID();
    rmasterID = Right1.GetDeviceID();

	Left1.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);
	Right1.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);

	Left1.Config_kP(0, Drivetrain_P);
	Left1.Config_kI(0, Drivetrain_I);
	Left1.Config_kD(0, Drivetrain_D);
	
    Right1.Config_kP(0, Drivetrain_P);
    Right1.Config_kI(0, Drivetrain_I);
    Right1.Config_kD(0, Drivetrain_D);

	Shifter.Set(frc::DoubleSolenoid::Value::kForward);

	std::cout << "INFO: DRIVETRAIN INIT COMPLETE" << std::endl;

}

void Drivetrain::Drive(float fwd, float trn, bool autoHeading, bool voltageControl){

	if(currentGear == 1){
		trn*=0.3;
		fwd*=0.5;
	}else{
		trn*=0.75;
	}

	if(gearHasChanged && ACCEL_LIMIT_ENABLED){

		if(currentGear == 1){
			Left1.ConfigClosedloopRamp(accelTimeLo, 10);
			Right1.ConfigClosedloopRamp(accelTimeLo, 10);
			gearHasChanged = false;
		}else if(currentGear == 2){
			Left1.ConfigClosedloopRamp(accelTimeHi, 10);
			Right1.ConfigClosedloopRamp(accelTimeHi, 10);
			gearHasChanged = false;
		}

	}

	if(trn != 0 && autoHeading){
		tempHeadingZero = gyro.GetFusedHeading();
    }
	if(autoHeading){
        angleErr = gyro.GetFusedHeading()-tempHeadingZero;

		leftSet = ((trn+(angleErr*0.05)) - fwd);
		rightSet = ((trn+(angleErr*0.05)) + fwd);
	}else{
		leftSet = (trn-fwd);
		rightSet = (trn+fwd);
	}

    if(!voltageControl && currentGear == 1){
        leftSet *= maxSpeedLo;
        rightSet *= maxSpeedLo;
    }else if(!voltageControl && currentGear == 2){
		leftSet *= maxSpeedHi;
        rightSet *= maxSpeedHi;
	}

	Set(leftSet, rightSet, voltageControl);



}

void Drivetrain::Set(float Left, float Right, bool voltageControl){

    if(!voltageControl){
	    Left1.Set(ControlMode::Velocity, Left);
	    Right1.Set(ControlMode::Velocity, Right);
    }else{
        Left1.Set(ControlMode::PercentOutput, Left);
	    Right1.Set(ControlMode::PercentOutput, Right);
    }
	Left2.Set(ControlMode::Follower, lmasterID);
	Right2.Set(ControlMode::Follower, rmasterID);

}

void Drivetrain::Shift(){

	if(Shifter.Get() == frc::DoubleSolenoid::Value::kForward){
		Shifter.Set(frc::DoubleSolenoid::Value::kReverse);
		currentGear = 2;
		gearHasChanged = true;
	}else{
		Shifter.Set(frc::DoubleSolenoid::Value::kForward);
		currentGear = 1;
		gearHasChanged = true;
	}



}


int * Drivetrain::GetEncVel(){

	EncVel[0] = Left1.GetSelectedSensorVelocity(0);
	EncVel[1] = Right1.GetSelectedSensorVelocity(0);

	return EncVel;

}

int * Drivetrain::GetEncPos(){

	EncPos[0] = Left1.GetSelectedSensorPosition(0);
	EncPos[1] = Right1.GetSelectedSensorPosition(0);

	return EncPos;

}

void Drivetrain::SetEncPos(double left, double right){

	Left1.SetSelectedSensorPosition(left, 0, 10);
	Right1.SetSelectedSensorPosition(right, 0, 10);

	frc::Wait(0.1);

	Left1.SetSelectedSensorPosition(left, 0, 10);
	Right1.SetSelectedSensorPosition(right, 0, 10);

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

	if(current[0] > 50 || current[1] > 50 || current[2] > 50 || current[3] > 50){
		
		double highCurr = 0;
		int motorNum = 0;

		for(int i = 0; i < 4; i++){
			if(current[i] > highCurr){
				highCurr = current[i];
				motorNum = i+1;
			}
		}
		
		std::cout << "WARNING: HIGH DRIVETRAIN CURRENT ON MOTOR " << motorNum << " DRAWING " << highCurr << " AMPS" << std::endl;
		
	}

	return current;
}

float * Drivetrain::GetSetpoint(){

	setpt[0] = leftSet;
	setpt[1] = rightSet;

}