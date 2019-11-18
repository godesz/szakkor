#include <dht.h>
#include <Wire.h>

#define DHT11_PIN1 4
#define DHT11_PIN2 7
#define DHT11_PIN3 8

int LDR_1 = A0; 
int LDR_2 = A1;

int LDR_1_value = 0;
int LDR_2_value = 0;


unsigned long th, tl, tpwm, ppm; // CO2
dht DHT;
int dht_pin;
int moisture_pin;




void setup() {
Serial.begin(9600);
Serial.println("Loop started.");
}
void loop() {

  // LDR
LDR_1_value = analogRead(LDR_1);
LDR_2_value = analogRead(LDR_2);
Serial.print(LDR_1_value);
Serial.print("\t");
Serial.print(LDR_2_value);
Serial.print("\t");
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

    // Soil Moisture

  moisture_pin  = analogRead(A2);
  Serial.println(moisture_pin);

delay(2000);

}
