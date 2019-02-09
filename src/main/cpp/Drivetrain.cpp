#include "Drivetrain.h"

Drivetrain::Drivetrain(int FL, int BL, int FR, int BR, int gyro) {
    motorFL = new WPI_TalonSRX(FL);
    motorBL = new WPI_TalonSRX(BL);
    motorFR = new WPI_TalonSRX(FR);
    motorBR = new WPI_TalonSRX(BR);
    this.gyro = new PigeonIMU(gyro);
       gyro->SetFusedHeading(0, 10);
}

Drivetrain::~Drivetrain() {
    delete motorFL;
    delete motorBL;
    delete motorFR;
    delete motorBR;
}

void Drivetrain::setVelocity(float linear, float twist, bool useGyro) {
    // Needs meters/sec at max speed
    float accelCalculated = 0;
    if (accel != 0)
        accelCalculated = abs(linear - speed) * max / accel;
    motorFL->SetMaxAccel(accelCalculated);
    motorBL->SetMaxAccel(accelCalculated);
    motorFR->SetMaxAccel(accelCalculated);
    motorBR->SetMaxAccel(accelCalculated);

    float left = linear - twist;
    float right = -linear - twist;
    if (twist == 0) {
        if (setFusedHeading) {
            gyro->setFusedHeading(0, 10);
            setFusedHeading = false;
        }
        left -= gyro->getFusedHeading() * 0.07;
        right -= gyro->getFusedHeading() * 0.07;
    } else
        setFusedHeading = true;
    motorFL->Set(left);
    motorBL->Set(left);
    motorFR->Set(right);
    motorBR->Set(right);
    speed = linear;
}

void Drivetrain::setMaxAccel(float accel) {
    this.accel = accel;
}

float Drivetrain::abs(float x) {
    if (x < 0)
        return -x;
    return x;
}