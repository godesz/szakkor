#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

 
Adafruit_BMP085_Unified bmp;// = Adafruit_BMP085_Unified(10085);
 
void setup() 
{
  Serial.begin(9600);
  if (!bmp.begin()) 
  {
    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
    while (1) {}
  }
}
 
void loop() 
{
  float aaa,bbb;
  Serial.print("Temperature = ");
  bmp.getTemperature(&aaa);
  Serial.print(aaa);
  Serial.println(" Celsius");
 
  Serial.print("Pressure = ");
  bmp.getPressure(&bbb);
  Serial.print(bbb);
  Serial.println(" Pascal");
 
 
  Serial.println();
  delay(5000);
}
