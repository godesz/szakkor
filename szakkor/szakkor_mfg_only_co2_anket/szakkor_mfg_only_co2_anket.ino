#include <Wire.h>
#include <dht.h>
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
Adafruit_BMP085 pressure;    // BMP180 object
int LDR_1, temp[2], i, led = 8;  // LDR, data, looper, errorLED
File myFile;       // SD
const int chipSelect = 4;   // SD
double T, P, p0;   
char status;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(pwmPin1, INPUT);
  pressure.begin();
  //2 sec errorLED ON
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);
  
  Serial.begin(9600); 
  Serial.println("Initializing\nGetting file info..");
  if (!SD.begin(chipSelect)) {
          Serial.println("initialization failed!");
          while (1);
                              }
  
  Serial.println("Loop started.");
}

int get_CO2(int pin)  // CO2 from pwm length
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
  Serial.println("Sensor looping");
  
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

  // wait for SD enabled
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
    // errorLED on if not writing
  }
  Serial.println("\tEND\t");
  //
  
  delay(5000);   // 5 sec delay between measuring
}
