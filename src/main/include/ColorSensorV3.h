/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
 
 #include <frc/I2C.h>
 #include <frc/ErrorBase.h>
 #include <frc/util/Color.h>
 #include <hal/SimDevice.h>
 #include "CIEColor.h"
 
 namespace rev {
 
 class ColorSensorV3 : public frc::ErrorBase {
 public:
     enum class GainFactor {
         k1x = 0,
         k3x = 1,
         k6x = 2,
         k9x = 3,
         k18x = 4
     };
 
     enum class LEDPulseFrequency {
         k60kHz = 0x18,
         k70kHz = 0x40,
         k80kHz = 0x28,
         k90kHz = 0x30,
         k100kHz = 0x38,
     };
 
     enum class LEDCurrent {
         kPulse2mA = 0,
         kPulse5mA = 1,
         kPulse10mA = 2,
         kPulse25mA = 3,
         kPulse50mA = 4,
         kPulse75mA = 5,
         kPulse100mA = 6,
         kPulse125mA = 7,
     };
 
     enum class ProximityResolution {
         k8bit = 0x00,
         k9bit = 0x08,
         k10bit = 0x10,
         k11bit = 0x18,
     };
 
     enum class ProximityMeasurementRate {
         k6ms = 1,
         k12ms = 2,
         k25ms = 3,
         k50ms = 4,
         k100ms = 5,
         k200ms = 6,
         k400ms = 7,
     };
 
     enum class ColorResolution {
         k20bit = 0x00,
         k19bit = 0x08,
         k18bit = 0x10,
         k17bit = 0x18,
         k16bit = 0x20,
         k13bit = 0x28,
     };
 
     enum class ColorMeasurementRate {
         k25ms = 0,
         k50ms = 1,
         k100ms = 2,
         k200ms = 3,
         k500ms = 4,
         k1000ms = 5,
         k2000ms = 7,
     };
 
     struct RawColor {
         uint32_t red;
         uint32_t green;
         uint32_t blue;
         uint32_t ir;
         RawColor(uint32_t r, uint32_t g, uint32_t b, uint32_t _ir) : red(r), green(g), blue(b), ir(_ir) {}
     };
 
     explicit ColorSensorV3(frc::I2C::Port port);
 
     ColorSensorV3(ColorSensorV3&&) = default;
     ColorSensorV3& operator=(ColorSensorV3&&) = default;
 
     frc::Color GetColor();
 
     RawColor GetRawColor();
 
     rev::CIEColor GetCIEColor();
 
     double GetIR();
 
     uint32_t GetProximity();
 
     void SetGain(GainFactor gain);
 
     void ConfigureProximitySensorLED(LEDPulseFrequency freq, LEDCurrent current, uint8_t pulses);
     
     void ConfigureProximitySensor(ProximityResolution res, ProximityMeasurementRate rate);
     
     void ConfigureColorSensor(ColorResolution res, ColorMeasurementRate rate);
 
     bool HasReset();
 
 private:
     enum class Register {
         kMainCtrl = 0x00,
         kProximitySensorLED = 0x01,
         kProximitySensorPulses = 0x02,
         kProximitySensorRate = 0x03,
         kLightSensorMeasurementRate = 0x04,
         kLightSensorGain = 0x05,
         kPartID = 0x06,
         kMainStatus = 0x07,
         kProximityData = 0x08,
         kDataInfrared = 0x0A,
         kDataGreen = 0x0D,
         kDataBlue = 0x10,
         kDataRed = 0x13
     };
 
     enum class MainCtrlFields {
         kProximitySensorEnable = 0x01,
         kLightSensorEnable = 0x02,
         kRGBMode = 0x04
     };
 
     struct MainStatus {
         uint8_t PSDataStatus:1;
         uint8_t PSInterruptStatus:1;
         uint8_t PSLogicStatus:1;
         uint8_t LSDataStatus:1;
         uint8_t LSInterruptStatus:1;
         uint8_t PowerOnStatus:1;
         uint8_t :2;
     };
 
     bool Write(Register reg, uint8_t data) {
         return m_i2c.Write(static_cast<uint8_t>(reg), data);
     }
 
     bool Read(Register reg, int count, uint8_t* data) {
         return m_i2c.Read(static_cast<uint8_t>(reg), count, data);
     }
 
     uint32_t To20Bit(uint8_t *val) {
         uint32_t result = 0;
         std::memcpy(&result, val, 3);
         return result & 0x03FFFF;
     }
     
     uint16_t To11Bit(uint8_t *val) {
         uint16_t result = 0;
         std::memcpy(&result, val, 2);
         return result & 0x7FF;
     }
 
     uint32_t Read20BitRegister(Register reg);
     uint16_t Read11BitRegister(Register reg);
 
     bool CheckDeviceID();
     void InitializeDevice();
     MainStatus GetStatus();
 
     static const double Cmatrix[9];
 
     frc::I2C m_i2c;
 
     hal::SimDevice m_simDevice;
     hal::SimDouble m_simR, m_simG, m_simB, m_simIR, m_simProx;
 };
 
 } // namespace rev
