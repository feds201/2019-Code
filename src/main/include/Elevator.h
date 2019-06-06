/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

/*
-one button 
[cargo mode 
hatch mode]
-cargo mode has 3 hights
-lowest hatch hight is the home position
-one button will switch from how high it is
-limit swatchs are on the top and buttom of robot
   -limit switchs control the direction the lift is going from up to down or down to up

Lift: Switch position to low, middle, and high
Switch: Switch from cargo to hatch & go back to home
Home: Send lift to home position
Move: Carry out direction from lift
*/


#pragma once

#include"RobotMap.h"


class Elevator {
 public:
  Elevator();
  void Lift();
  void Lower(bool hasHatch);
  void Override(double speed, bool isOverride, bool arePinsOut);
  int getEncPos();
  int getEncVel();
  double getCurrent();
  std::string getTarget();
  int getErr();
  float getOutput();
  int getAnalogRaw();
  double getClosedLoopTarget();
  double getIAccum();
  double getDirivErr();
  void down2k();

  private:
   
   int posList[6] = {0, 2500, 5000, 13500, 17500, 26500}; //Home/Cargo Low, Hatch Pickup, Hatch Low, Cargo Mid, Hatch Mid, Cargo High 

   int currentPos = 0;
   int overridePos = 0;
   bool isOverridden = false;
   bool wasOverridden = false;
   bool pinsOut = false;

   WPI_TalonSRX motor{elevatorMotor};

   bool topLimit = false;
   frc::DigitalInput bottomLimit{bottomLimitSwitch};

};