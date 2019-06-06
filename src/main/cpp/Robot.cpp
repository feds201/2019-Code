/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

void Robot::RobotInit() {

    //Camera = frc::CameraServer::GetInstance()->StartAutomaticCapture();

	//Camera.SetResolution(320, 240);
	//Camera.SetFPS(30);

	mjpegerver.SetSource(camera);  //Made MJPG server to host camera feed from roborio with more compression and a custom resolution
  	mjpegerver.SetFPS(15);
  	mjpegerver.SetResolution(480, 270);
  	mjpegerver.SetCompression(70);

	if(SINGLE_OPERATOR_MODE){
		std::cout << "WARNING: SINGLE OP MODE IS ENABLED" << std::endl;
	}

	if(CONFIG_MODE){
		std::cout << "WARNING: CONFIG MODE IS ENABLED" << std::endl;
	}

	std::cout << "INFO: ROBOT INIT COMPLETED" << std::endl;

	std::cout << std::endl << "FRED INITALIZED, READY TO TAKE OVER THE WORLD" << std::endl;
}


void Robot::RobotPeriodic() {}


void Robot::AutonomousInit() {


	//comp.SetClosedLoopControl(true);
 
}

void Robot::AutonomousPeriodic() {

	Teleop();
 
	//Make Sure To Put Teleop Code In Here For Sandstorm Period

}


float deadzone(float f){

			if (fabs(f) < .15)
				return 0.0f;
			else{
				if(f > 0)
					return(f-.15)/(1-.15);
				else
					return(f+.15)/(1-.15);
			}


		}


void Robot::TeleopInit() {

	//comp.SetClosedLoopControl(true);

}

void Robot::TeleopPeriodic() {

	Teleop();

}

void Robot::Teleop(){

if(Op.GetPOV() == 270){
		hatchAndBallLockoutOverride = true;
	}else if(Op.GetPOV() == 90){
		hatchAndBallLockoutOverride = false;
	}

	isHatchPresent = Hatch.hatchOn();
	isCargoPresent = Cargo.isCargo();
	isCargoWristDown = Cargo.isDown();

	if(hatchAndBallLockoutOverride){
		isHatchPresent = false;
		isCargoPresent = false;
		isCargoWristDown = false;
	}



	//Logging Stuff

	logThisTime = false;
	logTicker++;
	if(logTicker == logInterval){
		logTicker = 0;
		logThisTime = true;
	}

	//Op Controls

	//Comment Out If No Limit Switch Is Added To Hatch Mech
/*
	if(Op.GetRawButtonPressed(hatchAbort)){
		Hatch.Abort();
	}

	Hatch.Run(Op.GetRawButton(hatchRun), (isCargoWristDown || isCargoPresent));
*/
	//
	//

	// Uncomment If No Limit Switch Is Added To Hatch Mech

	if(Op.GetRawButtonPressed(hatchRun)){
		Hatch.moveBackplate();
	}

	if(Op.GetRawButtonPressed(hatchAbort)){
		Ele.down2k();
	}

	//

	if(Climb.getStage() == 0){
		Ele.Override(-deadzone(Op.GetRawAxis(eleOverrideAxis)), Op.GetRawButton(eleOverride), Hatch.pinsOut());
	}

	if(Op.GetRawButtonPressed(eleLower)){
		Ele.Lower(isHatchPresent);
	}

	if(Op.GetRawButtonPressed(eleLift)){
		Ele.Lift();
	}
	
	if(Driver.GetRawButtonPressed(climbIncStage)){
		Climb.incStage();
	}

	if(Driver.GetRawButtonPressed(climbDecStage)){
		Climb.decStage();
	}

	Climb.setWheel(Driver.GetRawAxis(climberWheelsFwdAxis)-Driver.GetRawAxis(climberWheelsRevAxis));

	if(Op.GetRawButtonPressed(cargoToggle)){
		Cargo.ToggleArm();
	}

	Cargo.Intake(Op.GetRawAxis(cargoIntakeAxis), Op.GetRawAxis(cargoEjectAxis), isHatchPresent, Ele.getEncPos());
	
	//Driver Controls
	
	Drive.Drive(deadzone(Driver.GetRawAxis(fwdChl)), deadzone(Driver.GetRawAxis(trnChl)), drivetrainAutoheading, drivetrainVoltageControl);

	if(Driver.GetRawButtonPressed(shiftBtn)){
		Drive.Shift();
	}

	//Cargo.forceArmReset(Driver.GetRawButton(4));

	//Display Data To Dashboard


	press = (double)((((double)pressure.GetValue()-404.0)/3418.0)*120.0);

	frc::SmartDashboard::PutNumber("Pressure", press);
	frc::SmartDashboard::PutNumber("Total Current Draw", pdp.GetTotalCurrent());
	frc::SmartDashboard::PutBoolean("Has Cargo", Cargo.isCargo());
	frc::SmartDashboard::PutBoolean("Has Hatch", Hatch.hatchOn());
	frc::SmartDashboard::PutString("Elevator Target Pos", Ele.getTarget());
	frc::SmartDashboard::PutNumber("Wrist Pos", Cargo.getEncPos());
	frc::SmartDashboard::PutNumber("Wrist Err", Cargo.getErr());
	frc::SmartDashboard::PutNumber("Elevator Pos", Ele.getEncPos());
	frc::SmartDashboard::PutNumber("Elevator Err", Ele.getErr());
	frc::SmartDashboard::PutNumber("Wrist Pos", Cargo.getEncPos());
	frc::SmartDashboard::PutNumber("Left Drivetrain Vel", Drive.GetEncVel()[0]);
	frc::SmartDashboard::PutNumber("Right Drivetrain Vel", Drive.GetEncVel()[1]);
	frc::SmartDashboard::PutNumber("Left Drivetain Pos", Drive.GetEncPos()[0]);
	frc::SmartDashboard::PutNumber("Right Drivetrain Pos", Drive.GetEncPos()[1]);
	frc::SmartDashboard::PutNumber("DM1 Current Draw", Drive.GetCurr()[0]);
	frc::SmartDashboard::PutNumber("DM2 Current Draw", Drive.GetCurr()[1]);
	frc::SmartDashboard::PutNumber("DM3 Current Draw", Drive.GetCurr()[2]);
	frc::SmartDashboard::PutNumber("DM4 Current Draw", Drive.GetCurr()[3]);
	frc::SmartDashboard::PutNumber("Elevator Current Draw", Ele.getCurrent());
	frc::SmartDashboard::PutNumber("Cargo Pickup Wheel Current", Cargo.getWheelsCurrent());
	frc::SmartDashboard::PutNumber("Cargo Wrist Current Master", Cargo.getWristCurrent()[0]);
	frc::SmartDashboard::PutNumber("Cargo Wrist Current Slave", Cargo.getWristCurrent()[1]);
	



//Logging

/*
	if(logThisTime){

		logTicker = 0;

		struct Logger::CSVVals csvData;

		csvData.driveCurrents[0] = Drive.GetCurr()[0];
		csvData.driveCurrents[1] = Drive.GetCurr()[1];
		csvData.driveCurrents[2] = Drive.GetCurr()[2];
		csvData.driveCurrents[3] = Drive.GetCurr()[3];

		csvData.driveEncPos[0] = Drive.GetEncPos()[0];
		csvData.driveEncPos[1] = Drive.GetEncPos()[1];

		csvData.driveEncVel[0] = Drive.GetEncVel()[0];
		csvData.driveEncVel[1] = Drive.GetEncVel()[1];

		csvData.elevatorCurrent = Ele.getCurrent();
		csvData.eleEncPos = Ele.getEncPos();
		csvData.eleEncVel = Ele.getEncVel();
		csvData.eleErr = Ele.getErr();
		csvData.eleOutputPercent = Ele.getOutput();
		csvData.eleClosedLoopTarget = Ele.getClosedLoopTarget();
		csvData.eleIAccum = Ele.getIAccum();
		csvData.eleDirErr = Ele.getDirivErr();


		csvData.cargoClosedLoopTarget = Cargo.getClosedLoopTarget();
		csvData.cargoIAccum = Cargo.getIAccum();
		csvData.cargoDirErr = Cargo.getDirivErr();
		csvData.cargoPickupWheelsCurrent = Cargo.getWheelsCurrent();
		csvData.cargoPickupWristCurrent[0] = Cargo.getWristCurrent()[0];
		csvData.cargoPickupWristCurrent[1] = Cargo.getWristCurrent()[1];
		csvData.cargoPickupWristPos = Cargo.getEncPos();
		csvData.wristEncVel = Cargo.getEncVel();
		csvData.cargoPickupWirstErr = Cargo.getErr();
		csvData.wristOutputPercent = Cargo.getOutput();
		csvData.cargoIn = Cargo.isCargo();

		csvData.hatchOn = Hatch.hatchOn();

		csvData.frontLifterUp = Climb.getFrontState();
		csvData.backLifterUp = Climb.getBackState();
		csvData.liftStage = Climb.getStage();

		csvData.voltage = pdp.GetVoltage();
		csvData.totalCurrent = pdp.GetTotalCurrent();

		Logger::instance()->logCSV(&csvData);

	}
*/	
}


void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
