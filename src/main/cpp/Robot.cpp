/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include"frc/WPILib.h"



void Robot::RobotInit() {

    Camera = frc::CameraServer::GetInstance()->StartAutomaticCapture();


}


void Robot::RobotPeriodic() {}


void Robot::AutonomousInit() {
 
}

void Robot::AutonomousPeriodic() {
 
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

}

void Robot::TeleopPeriodic() {

	//Logging Stuff

	logThisTime = false;
	logTicker++;
	if(logTicker == logInterval){
		logTicker = 0;
		logThisTime = true;
	}

	//Op Controler Vibration For Ele Modes

	if(vibrationTicker == 1 && eleMode){
		vibrationTicker++;
		Op.SetRumble(frc::GenericHID::RumbleType::kLeftRumble, 1);
	}else if(vibrationTicker == 1 && !eleMode){
		vibrationTicker++;
		Op.SetRumble(frc::GenericHID::RumbleType::kRightRumble, 1);
	}else if(vibrationTicker != 0 && vibrationTicker < 50){
		vibrationTicker++;
	}else{
		vibrationTicker = 0;
		Op.SetRumble(frc::GenericHID::RumbleType::kLeftRumble, 0);
		Op.SetRumble(frc::GenericHID::RumbleType::kRightRumble, 0);
	}

	//Op Controls

	if(Op.GetRawButtonPressed(hatchAbort)){
		Hatch->Abort();
	}

	Hatch->Run(Op.GetRawButton(hatchRun));

	Ele->Override(deadzone(Op.GetRawAxis(eleOverrideAxis)), Op.GetRawButtonPressed(eleOverride));
	
	if(Op.GetRawButtonPressed(eleSwitch)){
		eleMode = Ele->Switch();
		vibrationTicker = 1;
	}

	if(Op.GetRawButtonPressed(eleLift)){
		Ele->Lift();
	}
	
	if(Op.GetRawButtonPressed(climbIncStage)){
		Climb->incStage();
	}

	if(Op.GetRawButtonPressed(climbDecStage)){
		Climb->decStage();
	}

    Climb->deployArm(Op.GetRawAxis(climbArmAxis));
	Climb->setWheel(Op.GetRawAxis(climbWheelAxis));

	if(Op.GetRawButtonPressed(cargoToggle)){
		Cargo->ToggleArm();
	}

	Cargo->Intake(Op.GetRawAxis(cargoIntakeAxis));
	
	Ele->Refresh();
	
	
	//Driver Controls
	
	Drive->Drive(deadzone(Driver.GetRawAxis(1)), deadzone(Driver.GetRawAxis(4)), false, true);


	//Display Data To Dashboard

	frc::SmartDashboard::PutNumber("Total Current Draw", pdp.GetTotalCurrent());
	frc::SmartDashboard::PutBoolean("Has Cargo", Cargo->isCargo());
	frc::SmartDashboard::PutBoolean("Has Hatch", Hatch->hatchOn());
	frc::SmartDashboard::PutString("Elevator Target Pos", Ele->getTarget());
	
	if(CONFIG_MODE){
		frc::SmartDashboard::PutNumber("Elevator Pos", Ele->getEncPos());
		frc::SmartDashboard::PutNumber("Wrist Pos", Cargo->getEncPos());
		frc::SmartDashboard::PutNumber("Left Drivetrain Vel", Drive->GetEncVel()[0]);
		frc::SmartDashboard::PutNumber("Right Drivetrain Vel", Drive->GetEncVel()[1]);
		frc::SmartDashboard::PutNumber("Left Drivetain Pos", Drive->GetEncPos()[0]);
		frc::SmartDashboard::PutNumber("Right Drivetrain Pos", Drive->GetEncPos()[1]);
		frc::SmartDashboard::PutNumber("DM1 Current Draw", Drive->GetCurr()[0]);
		frc::SmartDashboard::PutNumber("DM2 Current Draw", Drive->GetCurr()[1]);
		frc::SmartDashboard::PutNumber("DM3 Current Draw", Drive->GetCurr()[2]);
		frc::SmartDashboard::PutNumber("DM4 Current Draw", Drive->GetCurr()[3]);
		frc::SmartDashboard::PutNumber("Elevator Current Draw", Ele->getCurrent());
		frc::SmartDashboard::PutNumber("Cargo Pickup Wheel Current", Cargo->getWheelsCurrent());
		frc::SmartDashboard::PutNumber("Cargo Wrist Current Master", Cargo->getWristCurrent()[0]);
		frc::SmartDashboard::PutNumber("Cargo Wrist Current Slave", Cargo->getWristCurrent()[1]);
	}



//Logging


	if(logThisTime){

		logTicker = 0;

		struct Logger::CSVVals csvData;

		csvData.driveSetpoints[0] = Drive->GetSetpoint()[0];
		csvData.driveSetpoints[1] = Drive->GetSetpoint()[1];

		csvData.driveCurrents[0] = Drive->GetCurr()[0];
		csvData.driveCurrents[1] = Drive->GetCurr()[1];
		csvData.driveCurrents[2] = Drive->GetCurr()[2];
		csvData.driveCurrents[3] = Drive->GetCurr()[3];

		csvData.elevatorCurrent = Ele->getCurrent();

		csvData.cargoPickupWheelsCurrent = Cargo->getWheelsCurrent();
		csvData.cargoPickupWristCurrent[0] = Cargo->getWristCurrent()[0];
		csvData.cargoPickupWristCurrent[1] = Cargo->getWristCurrent()[1];
		csvData.cargoPickupWristPos = Cargo->getEncPos();
		csvData.cargoIn = Cargo->isCargo();

		csvData.hatchOn = Hatch->hatchOn();

		csvData.frontLifterUp = Climb->getFrontState();
		csvData.backLifterUp = Climb->getBackState();
		csvData.liftStage = Climb->getStage();

		csvData.voltage = pdp.GetVoltage();
		csvData.totalCurrent = pdp.GetTotalCurrent();

		Logger::instance()->logCSV(&csvData);

	}

}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
