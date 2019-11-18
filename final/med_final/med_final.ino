#include <Adafruit_BMP085.h>
#include <dht.h>
#include <Wire.h>

#define ALTITUDE 210.19

#define DHT11_PIN1 7
#define DHT11_PIN2 8
#define DHT11_PIN3 9
#define DHT11_PIN4 10

#define pwmPin1 3 // co2 1
#define pwmPin2 5 // co2 2
#define pwmPin3 6 // co2 3
// 4,11,12,13 -> SD card


unsigned long th, tl, tpwm, ppm; // CO2
dht DHT;
int dht_pin;
int moisture_pin;
int LDR_1, LDR_2;
Adafruit_BMP085 pressure;


void setup() {
  Serial.println("Initializing.");
  Serial.begin(9600); 
  pressure.begin();
  pinMode(pwmPin1, INPUT);
  pinMode(pwmPin2, INPUT);
  pinMode(pwmPin3, INPUT);
  Serial.println("Loop started.");
}

int get_CO21()
{
  delay(500);
  th = pulseIn(pwmPin1, HIGH, 3000000); // use microseconds
  tl = pulseIn(pwmPin1, LOW, 3000000);
  tpwm = th + tl; // actual pulse width
  th = pulseIn(pwmPin1, HIGH, 3000000);
  ppm = 2000 * (th-2)/(tpwm-4);
  delay(1500);
  return ppm;
}

int get_CO22()
{
  delay(500);
  th = pulseIn(pwmPin2, HIGH, 3000000); // use microseconds
  tl = pulseIn(pwmPin2, LOW, 3000000);
  tpwm = th + tl; // actual pulse width
  th = pulseIn(pwmPin2, HIGH, 3000000);
  ppm = 2000 * (th-2)/(tpwm-4);
  delay(1500);
  return ppm;
}

int get_CO23()
{
  delay(500);
  th = pulseIn(pwmPin3, HIGH, 3000000); // use microseconds
  tl = pulseIn(pwmPin3, LOW, 3000000);
  tpwm = th + tl; // actual pulse width
  th = pulseIn(pwmPin3, HIGH, 3000000);
  ppm = 2000 * (th-2)/(tpwm-4);
  delay(1500);
  return ppm;
}

void loop() {

  //CO2 via pwm

  Serial.print("PPM: ");
  Serial.print(get_CO21());
  Serial.print(" ");
  Serial.print(get_CO22());
  Serial.print(" ");
  Serial.println(get_CO23());


  // DHT11
  dht_pin = DHT.read11(DHT11_PIN1);
  Serial.print(DHT.temperature);
  Serial.print("\t");
  Serial.print(DHT.humidity);
  Serial.print("\t");

  dht_pin = DHT.read11(DHT11_PIN2);
  Serial.print(DHT.temperature);
  Serial.print("\t");
  Serial.print(DHT.humidity);
  Serial.print("\t");

  dht_pin = DHT.read11(DHT11_PIN3);
  Serial.print(DHT.temperature);
  Serial.print("\t");
  Serial.print(DHT.humidity);
  Serial.print("\t");
  
  dht_pin = DHT.read11(DHT11_PIN4);
  Serial.print(DHT.temperature);
  Serial.print("\t");
  Serial.println(DHT.humidity);

  // Soil Moisture

  moisture_pin  = analogRead(A0);
  Serial.print("Moist: ");
  Serial.println(moisture_pin);

  // LDR

  LDR_1 = analogRead(A1);
  Serial.print("LDR: ");
  Serial.print(LDR_1);  

  LDR_2 = analogRead(A2);
  Serial.print("\t");
  Serial.println(LDR_2);  
  // bmp180
/*
  double T, P, p0;
  char status;

  T = pressure.readTemperature();
  P = pressure.readPressure();
*/
  
  delay(1000);
}


  /*Serial.print("PWM-time:\t");
  Serial.print(tpwm);
  Serial.print("\t");
  Serial.print(th);
  Serial.print("\t");
  Serial.print(tl);
  Serial.print("\t");*/
