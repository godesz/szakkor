#include <dht.h>
#include <Wire.h>
#include <SFE_BMP180.h>

#define ALTITUDE 210.19

#define DHT11_PIN1 4
#define DHT11_PIN2 7
#define DHT11_PIN3 8

#define pwmPin1 3 // co2 1
#define pwmPin2 5 // co2 2
#define pwmPin3 6 // co2 3
// 10,11,12,13 -> SD card


unsigned long th, tl, tpwm, ppm; // CO2
dht DHT;
int dht_pin;
int moistire_pin;
SFE_BMP180 pressure;


void setup() {
  Serial.println("Initializing.");
  Serial.begin(9600); 
  pressure.begin();
  pinMode(pwmPin, INPUT);
  Serial.println("Loop started.");
}

int get_CO2(int pin)
{
  th = pulseIn(pin, HIGH, 3000000); // use microseconds
  tl = pulseIn(pin, LOW, 3000000);
  tpwm = th + tl; // actual pulse width

  th = pulseIn(pin, HIGH, 3000000);
  ppm = 2000 * (th-2)/(tpwm-4);
  
	return ppm;
}

void loop() {

  //CO2 via pwm

  Serial.print("PPM1: ");
  Serial.println(get_CO2(pwmPin1));
  Serial.print("PPM2: ");
  Serial.println(get_CO2(pwmPin2));
  Serial.print("PPM3: ");
  Serial.println(get_CO2(pwmPin3));


  // DHT11
	dht_pin = DHT.read11(DHT11_PIN1);
	Serial.print(DHT.temperature);
	Serial.print("\t");
	Serial.println(DHT.humidity);

	dht_pin = DHT.read11(DHT11_PIN2);
	Serial.print(DHT.temperature);
	Serial.print("\t");
	Serial.println(DHT.humidity);

	dht_pin = DHT.read11(DHT11_PIN3);
	Serial.print(DHT.temperature);
	Serial.print("\t");
	Serial.println(DHT.humidity);

	// Soil Moisture

	moisture_pin  = analogRead(A0);
	Serial.println(moisture_pin);

	// bmp180

	double T, P, p0;
	char status;

	status = pressure.getTemperature(T);
	status = pressure.getPressure(P, T);

  
  delay(2000);
}


  /*Serial.print("PWM-time:\t");
  Serial.print(tpwm);
  Serial.print("\t");
  Serial.print(th);
  Serial.print("\t");
  Serial.print(tl);
  Serial.print("\t");*/
