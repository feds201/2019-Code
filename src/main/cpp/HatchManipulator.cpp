/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HatchManipulator.h"
HatchManipulator::HatchManipulator(int PCM, int intake, int eject) {

    rightSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
    leftSolenoid.Set(frc::DoubleSolenoid::Value::kForward);

}

void HatchManipulator::eject() {

    if(rightSolenoid.Get() == frc::DoubleSolenoid::Value::kReverse) {
        rightSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
        leftSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
    }

    else {
        rightSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
        leftSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
    }
}
