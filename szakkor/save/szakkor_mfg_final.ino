#include <dht.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <SD.h>

#define ALTITUDE 210.19

#define DHT11_PIN1 7
#define DHT11_PIN2 8
#define DHT11_PIN3 9
#define DHT11_PIN4 10

#define pwmPin1 3 // co2 1
#define pwmPin2 5 // co2 2
#define pwmPin3 6 // co2 3
// 4,11,12,13 -> SD card
// SCL A5, SDA A4



unsigned long th, tl, tpwm, ppm; // CO2
dht DHT;
int dht_pin;
int moisture_pin;
Adafruit_BMP085 pressure;
int LDR_1, LDR_2;
File myFile;
int sorszam;
String filename2 = "";
String ssz;
int temp[14], i;
const int chipSelect = 4;
  double T, P, p0;
  char status;


void setup() {
  randomSeed(analogRead(A1));
  Serial.begin(9600); 
  Serial.println("Initializing.");
  Serial.println("Getting file info");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  filename2 += String(random(1000)) + ".txt";


  pressure.begin();
  pinMode(pwmPin1, INPUT);
  pinMode(pwmPin2, INPUT);
  pinMode(pwmPin3, INPUT);
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
  temp[1] = get_CO2(pwmPin2);
  temp[2] = get_CO2(pwmPin3);
  
  // DHT11
  dht_pin = DHT.read11(DHT11_PIN1);
  temp[3] = DHT.temperature;
  temp[4] = DHT.humidity;
 
  dht_pin = DHT.read11(DHT11_PIN2);
  temp[5] = DHT.temperature;
  temp[6] = DHT.humidity;
  
  dht_pin = DHT.read11(DHT11_PIN3);
  temp[7] = DHT.temperature;
  temp[8] = DHT.humidity;

  
  dht_pin = DHT.read11(DHT11_PIN4);
  temp[9] = DHT.temperature;
  temp[10] = DHT.humidity;


  // Soil Moisture

  moisture_pin  = analogRead(A0);
  temp[11] = moisture_pin;

  // LDR

  LDR_1  = analogRead(A1);
  temp[12] = LDR_1;

  LDR_2  = analogRead(A2);
  temp[13] = LDR_2;
  // bmp180

  T = pressure.readTemperature();
  P = pressure.readPressure();

  // Filewrite
  String dataString = "";
for(i=0;i<14;i++) { dataString += String(temp[i]);
                    dataString += "\t"; }
  dataString += String(P);
  dataString += "\t";
  dataString += String(T);
  
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
  }
  Serial.println("\tEND\t");
  //
  
  delay(5000);
}


  /*Serial.print("PWM-time:\t");
  Serial.print(tpwm);
  Serial.print("\t");
  Serial.print(th);
  Serial.print("\t");
  Serial.print(tl);
  Serial.print("\t");*/
