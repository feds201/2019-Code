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



void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {

	if(Op.GetRawButtonPressed(hatchAbort)){
		Hatch.Abort();
	}

	Hatch.Run(Op.GetRawButton(hatchRun));

	Ele.Override(deadzone(Op.GetRawAxis(eleOverrideAxis)), Op.GetRawButtonPressed(eleOverride));
	
	if(Op.GetRawButtonPressed(eleSwitch)){
		Ele.Switch();
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

    Climb.deployArm(Op.GetRawAxis(climbArmAxis));
	Climb.setWheel(Op.GetRawAxis(climbWheelAxis));

	if(Op.GetRawButtonPressed(cargoToggle)){
		Cargo.ToggleArm();
	}

	Cargo.Intake(Op.GetRawAxis(cargoIntakeAxis));
	
	Ele.Refresh();
	
	
	
	
	Drive.Drive(deadzone(Driver.GetRawAxis(1)), deadzone(Driver.GetRawAxis(4)), false, true);

}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
