#include <dht.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <SD.h>

#define ALTITUDE 210.19

#define pwmPin1 3 // co2 1
// 4,11,12,13 -> SD card
// MOSI - pin 11
// MISO - pin 12
// CLK - pin 13
// CS - pin 4
// SCL A5, SDA A4

unsigned long th, tl, tpwm, ppm; // CO2
Adafruit_BMP085 pressure;
int LDR_1;
File myFile;
int temp[5], i;
const int chipSelect = 4;
double T, P, p0;
char status;
int led = 8;

void setup() {
  pinMode(led, OUTPUT);
digitalWrite(led, HIGH);
    delay(2000);
    digitalWrite(led, LOW);
  
 
  Serial.begin(9600); 
  Serial.println("Initializing.");
  Serial.println("Getting file info");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }

  pressure.begin();
  pinMode(pwmPin1, INPUT);

  Serial.println("Loop started.");
}

int get_CO2(int pin)
{
   delay(100);
  th = pulseIn(pin, HIGH, 3000000); // use microseconds
  tl = pulseIn(pin, LOW, 3000000);
  tpwm = th + tl; // actual pulse width

  th = pulseIn(pin, HIGH, 3000000);
  ppm = 2000 * (th-2)/(tpwm-4);
  delay(100);
  return ppm;
}

void loop() {
  Serial.println("looping");
  
  //CO2 via pwm
 
  temp[0] = get_CO2(pwmPin1);
 
  // LDR

  LDR_1  = analogRead(A1);
  temp[1] = LDR_1;
  
  // bmp180

  T = pressure.readTemperature();
  P = pressure.readPressure();

  // Filewrite
  String dataString = "";

  dataString = String(temp[0]) + "\t" + String(temp[1]) + "\t" + String(P) + "\t" + String(T);
  
  Serial.println(dataString);
  if (!SD.begin(chipSelect)){}
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }

  else {
    Serial.println("error opening datalog.txt");
    digitalWrite(led, HIGH);
    delay(2000);
    digitalWrite(led, LOW);
  }
  Serial.println("\tEND\t");
  //
  
  delay(5000);
}
