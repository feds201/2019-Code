#pragma once

#include"frc/WPILib.h"
#include"ctre/Phoenix.h"
#include"RobotMap.h"

class Drivetrain{

public:

	Drivetrain();
	void Drive(float fwd, float trn, bool autoHeading, bool voltageControl);
	void directSet(float left, float right);
	void Set(float Left, float Right, bool voltageControl);
	int * GetEncVel();
	int * GetEncPos();
	float * GetCurr();
	float * GetSetpoint();
	void SetEncPos(double left, double right);
	double getGyroAngle();
	void setGyroAngle(double angle);

private:

    bool ACCEL_LIMIT_ENABLED = false;

		double P = Drivetrain_P;
		double I = Drivetrain_I;
		double D = Drivetrain_D;

		int maxSpeed = maxRobotVelocity; 

		double gearRatio = gearRatioToEncodersFromWheels; 
		double wheelR = wheelRadius; 
		double encCountsPerRev = 4096;
		double pi = 3.14159;
		double countsPerIn;
		double maxV = 0; //Don't Know Yet
		float inchesPerMeter = 39.37007874;
		double metersPerCount;
		float accelTime;
		float current[4];
		float setpt[2];
    float tempHeadingZero;
    float angleErr;

		float lastTime;

		WPI_TalonSRX Left1{driveMotor1};
		WPI_TalonSRX Left2{dirveMotor2};
		WPI_TalonSRX Right1{driveMotor3};
		WPI_TalonSRX Right2{driveMotor4};

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
