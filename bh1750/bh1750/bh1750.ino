
#include <BH1750FVI.h> // Sensor Library
#include <Wire.h> // I2C Library

uint16_t Light_Intensity=0;
// Call the function
#define LedPin 9 // led connecting to pin D9
BH1750FVI LightSensor;

int SensorValue =0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  call begin Function so turn the sensor On .
  LightSensor.begin();


  LightSensor.SetAddress(Device_Address_H); //Address 0x5C

  LightSensor.SetMode(Continuous_H_resolution_Mode);


}

void loop() {

  Light_Intensity=LightSensor.GetLightIntensity();
  delay(500);

  SensorValue=map(Light_Intensity,0,2000,255,0);
  SensorValue=constrain(SensorValue,255,0);

  Serial.println(Light_Intensity);

}
