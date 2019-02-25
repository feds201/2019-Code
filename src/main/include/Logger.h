/*
 * Logger.h
 *
 *  Created on: Jan 21, 2016
 *      Author: feds
 */

#pragma once

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#include "MyTimer.h"

class Logger {
public:
	struct CSVVals {
		double voltage;
		double totalCurrent;

		double driveCurrents[4];

		double cargoClosedLoopTarget;
		double cargoIAccum;
		double cargoDirErr;

		double eleClosedLoopTarget;
		double eleIAccum;
		double eleDirErr;

		int driveEncPos[2];
		int driveEncVel[2];

		int eleEncVel;
		int eleEncPos;
		int eleErr;
		float eleOutputPercent;
		
		int wristEncVel;

		double elevatorCurrent;
		double cargoPickupWristPos;
		int cargoPickupWirstErr;
		float wristOutputPercent;
		double cargoPickupWristCurrent[2];
		double cargoPickupWheelsCurrent;

		bool hatchOn;
		bool cargoIn;

		bool frontLifterUp;
		bool backLifterUp;
		double liftStage;
	};

	virtual ~Logger();

	void logError(const char *msg, ... );
	void logInfo(const char *msg, ... );

	void logCSV(struct CSVVals *data);

    static Logger *instance()
    {
        if (!singlton)
        	singlton = new Logger();
        return singlton;
    }
private:

    MyTimer timer;

    struct timeval startTime;

    std::ofstream logFile;
    std::ofstream csvFile;

    void save();

	Logger();
	static Logger *singlton;
};