#include "frc/smartdashboard/Smartdashboard.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "Drivetrain.h"

#ifndef SRC_VISION_H_
#define SRC_VISION_H_

#include "WPILib.h"
#include"CANTalon.h"

class Vision{
std::shared_ptr<NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
double targetOffsetAngle_Horizontal = table->GetNumber("tx",0.0);
double targetOffsetAngle_Vertical = table->GetNumber("ty",0.0);
double targetArea = table->GetNumber("ta",0.0);
double targetSkew = table->GetNumber("ts",0.0);

 
};

#endif SRC_VISION_H_;