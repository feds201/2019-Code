#pragma once

#include"RobotMap.h"

class Drivetrain{

public:

	Drivetrain();
	void Drive(float fwd, float trn, bool autoHeading, bool voltageControl);
	void directSet(float left, float right);
	void Set(float Left, float Right, bool voltageControl);
	void Shift();
	int * GetEncVel();
	int * GetEncPos();
	float * GetCurr();
	float * GetSetpoint();
	void SetEncPos(double left, double right);
	double getGyroAngle();
	void setGyroAngle(double angle);

private:
		
		double countsPerInLo;
		double countsPerInHi;

		int maxVLo;
		int maxVHi;

		float inchesPerMeter = 39.37007874;

		double metersPerCountLo;
		double metersPerCountHi;

		double metersPer100msLo;
		double metersPer100msHi;

		float accelTimeLo;
		float accelTimeHi;

		float current[4];
		float setpt[2];
    	float tempHeadingZero;
    	float angleErr;
		int currentGear = 1;
		bool gearHasChanged = false;

		float lastTime;

		WPI_TalonSRX Left1{leftDriveMotor1};
		WPI_TalonSRX Left2{leftDirveMotor2};
		WPI_TalonSRX Right1{rightDriveMotor1};
		WPI_TalonSRX Right2{rightDriveMotor2};

		frc::DoubleSolenoid Shifter{PCMID, setShifterFirstGear, setShifterSecondGear};

    int lmasterID;
    int rmasterID;

		PigeonIMU gyro{Pigeon};

		float MAXAccel = 0; //To Be Calculated

		double leftSet;
		double rightSet;

		bool autoHeading = true;

		int EncPos[2];
		int EncVel[2];


};
