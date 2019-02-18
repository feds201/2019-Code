/*
 * Logger.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: feds
 */

#include <Logger.h>
#include <frc/WPILib.h>
#include <MyTimer.h>

Logger *Logger::singlton = 0;

Logger::Logger() {
	gettimeofday(&startTime, 0);

	char timeString[64];
	time_t rawTime;
	struct tm * timeInfo;

	time(&rawTime);
	timeInfo = localtime (&rawTime);
	strftime(timeString, sizeof(timeString), "%g%m%e_%H%M", timeInfo);

	logFile.open("/home/lvuser/logFile" + (std::string)timeString + ".txt");
	csvFile.open("/home/lvuser/logFile" + (std::string)timeString + ".csv");

	std::cout << "INFO: THE LOGFILE FOR THIS SESSION IS NAMED " << (std::string)timeString << std::endl; 

	csvFile
			<< "time" << ','
			<< "voltage" << ','
			<< "totalCurrent" << ','
			<< "driveCurrents0" << ','
			<< "driveCurrents1" << ','
			<< "driveCurrents2" << ','
			<< "driveCurrents3" << ','
			<< "leftDriveEncPos" << ','
			<< "rightDriveEncPos" << ','
			<< "leftDriveEncVel" << ','
			<< "rightDriveEncVel" << ','
			<< "elevatorCurrent" << ','
			<< "elevatorEncPos" << ','
			<< "elevatorEncVel" << ','
			<< "elevatorErr" << ','
			<< "cargoPickupWristPos" << ','
			<< "cargoPickupWristEncVel" << ','
			<< "cargoPickupWristErr" << ','
			<< "cargoPickupEncVel" << ','
			<< "cargoPickupWristCurrent1" << ','
			<< "cargoPickupWristCurrent2" << ','
			<< "cargoPickupWheelsCurrent" << ','
			<< "cargoIn" << ','
			<< "hatchOn" << ','
			<< "frontLifterUp" << ','
			<< "backLifterUp" << ','
			<< "liftStage" << std::endl;
}

void Logger::logInfo(const char *msg, ... )
{
	va_list args;
	va_start(args,msg);

	char buffer[256];
	vsnprintf(buffer, sizeof(buffer), msg, args);

	logFile << timer.getTotalTime() << " - [INFO] " << buffer << std::endl;
	std::cout << timer.getTotalTime() << " - [INFO] " << buffer << std::endl;
	save();
}

void Logger::logError(const char *msg, ... )
{
	va_list args;
	va_start(args,msg);

	char buffer[256];
	vsnprintf(buffer, sizeof(buffer), msg, args);

	logFile << "[ERROR] " << buffer << std::endl;
	std::cerr << "[ERROR] " << buffer << std::endl;
	save();
}

void Logger::logCSV(struct CSVVals *data)
{
	double time = timer.getTotalTime();

	csvFile
			<< time << ','
			<< data->voltage << ','
			<< data->totalCurrent << ','
			<< data->driveCurrents[0] << ','
			<< data->driveCurrents[1] << ','
			<< data->driveCurrents[2] << ','
			<< data->driveCurrents[3] << ','
			<< data->driveEncPos[0] << ','
			<< data->driveEncPos[1] << ','
			<< data->driveEncVel[0] << ','
			<< data->driveEncVel[1] << '.'
			<< data->elevatorCurrent << ','
			<< data->eleEncPos << ','
			<< data->eleEncVel << ','
			<< data->eleErr << ','
			<< data->cargoPickupWristPos << ','
			<< data->wristEncVel << ','
			<< data->cargoPickupWirstErr << ','
			<< data->cargoPickupWristCurrent[0] << ','
			<< data->cargoPickupWristCurrent[1] << ','
			<< data->cargoPickupWheelsCurrent << ','
			<< (data->cargoIn ? 1 : 0) << ','
			<< (data->hatchOn ? 1 : 0) << ','
			<< (data->frontLifterUp ? 1 : 0) << ','
			<< (data->backLifterUp ? 1 : 0) << ','
			<< data->liftStage << std::endl;
	save();
}

void Logger::save()
{
	logFile.flush();
	csvFile.flush();
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}
