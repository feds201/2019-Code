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
  int Switch();
  void Home();
  void Refresh(bool isHatch, bool isCargo);
  void Override(double speed, bool isOverride);
  int getEncPos();
  double getCurrent();
  std::string getTarget();

  private:
   
   bool configMode = CONFIG_MODE;

   void Move(int position);
   
   int pos = 0; // 0 = Home, 1 = Hatch Mid, 2 = Hatch High, 3 = Cargo Low, 4 = Cargo Mid, 5 = Cargo High

   enum mode {Hatch, Cargo, Auto};

   int posList[5] = {0, 0, 0, 0, 0}; //Hatch Mid, Hatch High, Cargo Low, Cargo Mid, Cargo High

   mode currentMode = Hatch;
   int currentPos = 0;
   bool goingUp = true;
   bool isMoving = false;
   bool isOverridden = false;
   int bufferDist = 0; //TBD

   bool isAuto = false;
   
   double goingUpCruseSpeed = upCruseSpeed;
   double goingUpApproachSpeed = upApproachSpeed;
   double goingDownCruseSpeed = downCruseSpeed;
   double goingDownApproachSpeed = downApproachSpeed;

   WPI_TalonSRX motor{elevatorMotor};

   frc::DigitalInput topLimit{topLimitSwitch};
   frc::DigitalInput bottomLimit{bottomLimitSwitch};

};