/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

void Robot::RobotInit() {

    Camera = frc::CameraServer::GetInstance()->StartAutomaticCapture();

	if(SINGLE_OPERATOR_MODE){
		std::cout << "WARNING: SINGLE OP MODE IS ENABLED" << std::endl;
	}

	if(CONFIG_MODE){
		std::cout << "WARNING: CONFIG MODE IS ENABLED" << std::endl;
	}

	std::cout << "INFO: ROBOT INIT COMPLETED" << std::endl;

}


void Robot::RobotPeriodic() {}


void Robot::AutonomousInit() {
 
}

void Robot::AutonomousPeriodic() {
 
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



void Robot::singleOpMode() {

	if(Driver.GetPOV() == 90){
		singleOPDriverMode = false;
	}else if(Driver.GetPOV() == 270){
		singleOPDriverMode = true;
	}

	//Op Controler Vibration For Ele Modes

	if(vibrationTicker == 1 && eleMode == 0){
		vibrationTicker++;
		Driver.SetRumble(frc::GenericHID::RumbleType::kLeftRumble, 1);
	}else if(vibrationTicker == 1 && eleMode == 1){
		vibrationTicker++;
		Driver.SetRumble(frc::GenericHID::RumbleType::kRightRumble, 1);
	}else if(eleMode == 2){
		vibrationTicker = 0;
	}else if(vibrationTicker != 0 && vibrationTicker < 50){
		vibrationTicker++;
	}else{
		vibrationTicker = 0;
		Driver.SetRumble(frc::GenericHID::RumbleType::kLeftRumble, 0);
		Driver.SetRumble(frc::GenericHID::RumbleType::kRightRumble, 0);
	}

	if(!singleOPDriverMode){

	//Op Controls

	//Comment Out If No Limit Switch Is Added To Hatch Mech

	if(Driver.GetRawButtonPressed(hatchAbort)){
		Hatch.Abort();
	}

	Hatch.Run(Driver.GetRawButton(hatchRun), (isCargoWristDown || isCargoPresent));

	//
	//

	/* Uncomment If No Limit Switch Is Added To Hatch Mech

	if(Driver.GetRawButtonPressed(hatchRun)){
		Hatch.moveBackplate();
	}

	Hatch.Eject(Driver.GetRawButton(hatchAbort));

	*/

	if(Climb.getStage() == 0){
		Ele.Override(-deadzone(Driver.GetRawAxis(eleOverrideAxis)), Driver.GetRawButton(eleOverride));
	}

	if(Driver.GetRawButtonPressed(eleSwitch)){
		eleMode = Ele.Switch();
		vibrationTicker = 1;
	}

	if(Driver.GetRawButtonPressed(eleLift)){
		Ele.Lift();
	}
	
	if(Driver.GetRawButtonPressed(climbIncStage)){
		Climb.incStage();
	}

	if(Driver.GetRawButtonPressed(climbDecStage)){
		Climb.decStage();
	}

	Climb.setWheel(Driver.GetRawAxis(climbWheelAxis));

	if(Driver.GetRawButtonPressed(cargoToggle)){
		Cargo.ToggleArm();
	}

	Cargo.Intake(Driver.GetRawAxis(cargoIntakeAxis), Driver.GetRawAxis(cargoEjectAxis), isHatchPresent);
	
	Ele.Refresh(Hatch.hatchOn(), Cargo.isCargo());
	
	}else{
	//Driver Controls
	
	Drive.Drive(deadzone(Driver.GetRawAxis(fwdChl)), deadzone(Driver.GetRawAxis(trnChl)), drivetrainAutoheading, drivetrainVoltageControl);

	if(Driver.GetRawButtonPressed(shiftBtn)){
		Drive.Shift();
	}

	}
}

void Robot::TeleopInit() {

}

void Robot::TeleopPeriodic() {

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

	if(!SINGLE_OPERATOR_MODE){

	//Op Controler Vibration For Ele Modes

	if(vibrationTicker == 1 && eleMode == 0){
		vibrationTicker++;
		Op.SetRumble(frc::GenericHID::RumbleType::kLeftRumble, 1);
	}else if(vibrationTicker == 1 && eleMode == 1){
		vibrationTicker++;
		Op.SetRumble(frc::GenericHID::RumbleType::kRightRumble, 1);
	}else if(vibrationTicker == 1 && eleMode == 2){
		vibrationTicker = 0;
	}else if(vibrationTicker != 0 && vibrationTicker < 50){
		vibrationTicker++;
	}else{
		vibrationTicker = 0;
		Op.SetRumble(frc::GenericHID::RumbleType::kLeftRumble, 0);
		Op.SetRumble(frc::GenericHID::RumbleType::kRightRumble, 0);
	}

	//Op Controls

	//Comment Out If No Limit Switch Is Added To Hatch Mech

	if(Op.GetRawButtonPressed(hatchAbort)){
		Hatch.Abort();
	}

	Hatch.Run(Op.GetRawButton(hatchRun), (isCargoWristDown || isCargoPresent));

	//
	//

	/* Uncomment If No Limit Switch Is Added To Hatch Mech

	if(Op.GetRawButtonPressed(hatchRun)){
		Hatch.moveBackplate();
	}

	Hatch.Eject(Op.GetRawButton(hatchAbort));

	*/

	if(Climb.getStage() == 0){
		Ele.Override(-deadzone(Op.GetRawAxis(eleOverrideAxis)), Op.GetRawButton(eleOverride));
	}

	if(Op.GetRawButtonPressed(eleSwitch)){
		eleMode = Ele.Switch();
		vibrationTicker = 1;
	}

	if(Op.GetRawButtonPressed(eleLift)){
		Ele.Lift();
	}
	
	if(Op.GetRawButtonPressed(climbIncStage)){
		Climb.incStage();
	}

	if(Op.GetRawButtonPressed(climbDecStage)){
		Climb.decStage();
	}

	Climb.setWheel(Op.GetRawAxis(climbWheelAxis));

	if(Op.GetRawButtonPressed(cargoToggle)){
		Cargo.ToggleArm();
	}

	Cargo.Intake(Op.GetRawAxis(cargoIntakeAxis), Op.GetRawAxis(cargoEjectAxis), isHatchPresent);
	
	Ele.Refresh(Hatch.hatchOn(), Cargo.isCargo());
	
	
	//Driver Controls
	
	Drive.Drive(deadzone(Driver.GetRawAxis(fwdChl)), deadzone(Driver.GetRawAxis(trnChl)), drivetrainAutoheading, drivetrainVoltageControl);

	if(Driver.GetRawButtonPressed(shiftBtn)){
		Drive.Shift();
	}

	}else{

		singleOpMode();
	
	}

	//Display Data To Dashboard


	press = (double)((((double)pressure.GetValue()-404.0)/3418.0)*120.0);

	frc::SmartDashboard::PutNumber("Pressure", press);
	frc::SmartDashboard::PutNumber("Total Current Draw", pdp.GetTotalCurrent());
	frc::SmartDashboard::PutBoolean("Has Cargo", Cargo.isCargo());
	frc::SmartDashboard::PutBoolean("Has Hatch", Hatch.hatchOn());
	frc::SmartDashboard::PutString("Elevator Target Pos", Ele.getTarget());
	frc::SmartDashboard::PutNumber("Wrist Pos", Cargo.getEncPos());
	frc::SmartDashboard::PutNumber("Err", Cargo.getErr());
	
	if(CONFIG_MODE){
		frc::SmartDashboard::PutNumber("Elevator Pos", Ele.getEncPos());
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
	}



//Logging


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

		csvData.cargoPickupWheelsCurrent = Cargo.getWheelsCurrent();
		csvData.cargoPickupWristCurrent[0] = Cargo.getWristCurrent()[0];
		csvData.cargoPickupWristCurrent[1] = Cargo.getWristCurrent()[1];
		csvData.cargoPickupWristPos = Cargo.getEncPos();
		csvData.wristEncVel = Cargo.getEncVel();
		csvData.cargoPickupWirstErr = Cargo.getErr();
		csvData.cargoIn = Cargo.isCargo();

		csvData.hatchOn = Hatch.hatchOn();

		csvData.frontLifterUp = Climb.getFrontState();
		csvData.backLifterUp = Climb.getBackState();
		csvData.liftStage = Climb.getStage();

		csvData.voltage = pdp.GetVoltage();
		csvData.totalCurrent = pdp.GetTotalCurrent();

		Logger::instance()->logCSV(&csvData);

	}

}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
