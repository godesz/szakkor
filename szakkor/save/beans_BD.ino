#include <BH1750.h>
#include <HardwareSerial.h>
//#include "Timer.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

//#define DEBUG

#define hdc2080addr 0x40
#define hdc2080addr_2 65
#define temp_low_addr 0 
#define temp_high_addr 1
#define hum_low_addr 2
#define hum_high_addr 3

#define meas_conf_addr 0x0F
#define meas_conf_data 0x01

#define dev_config_addr 0x0E
#define dev_config_data 0x50

#define ONE_WIRE_BUS 23 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

#define TFT_MOSI 13  // Data out
#define TFT_SCLK 14  // Clock out
  #define TFT_CS        27 // Hallowing display control pins: chip select
  #define TFT_RST       32 // Display reset
  #define TFT_DC        33 // Display data/command select

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

bool isluxlow = false;
int luxvalue = 0;

String stringToSend = "";
String stringToDisplay = "";

void StartHDCMeasurement()
{
  Wire.beginTransmission(hdc2080addr);
  Wire.write(meas_conf_addr);
  Wire.write(meas_conf_data);
  //DEBUG_PRINTLN(Wire.endTransmission());
  Wire.endTransmission();

  Wire.beginTransmission(hdc2080addr_2);
  Wire.write(meas_conf_addr);
  Wire.write(meas_conf_data);
  //DEBUG_PRINTLN(Wire.endTransmission());
  Wire.endTransmission();
}

float ReadTemp(int num)
{
  int temp_low = 0;
  int temp_high = 0;
  float temp = 0;
  if(num == 0)
  {
    Wire.beginTransmission(hdc2080addr);
    Wire.write(temp_low_addr);
    Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      temp_low = c;
      DEBUG_PRINTLN(String(c,HEX));         // print the character
    }

    Wire.beginTransmission(hdc2080addr);
    Wire.write(temp_high_addr);
    Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as cha racter
      temp_high = c;
      DEBUG_PRINTLN(String(c,HEX));         // print the character
    }
  }
  else if(num == 1)
  {
    Wire.beginTransmission(hdc2080addr_2);
    Wire.write(temp_low_addr);
    Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr_2, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      temp_low = c;
      DEBUG_PRINTLN(String(c,HEX));         // print the character
    }
  
    Wire.beginTransmission(hdc2080addr_2);
    Wire.write(temp_high_addr);
    Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr_2, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      temp_high = c;
      DEBUG_PRINTLN(String(c,HEX));         // print the character
    }
  }
  if(num > 1)
  {
    return -1;
  }
  temp = (((temp_high << 8) | temp_low) / pow(2, 16)) * 165 - 40;
  //DEBUG_PRINTLN("Temperature (Sensor, value:");
  //DEBUG_PRINT(num);
  //DEBUG_PRINT(", ");
  //DEBUG_PRINT(temp);
  return temp;
}

float ReadHumidity(int num)
{
  int hum_low = 0;
  int hum_high = 0;
  float hum = 0;

  String err = "";
  
  if(num == 0)
  {
    int error = 0;
    Wire.beginTransmission(hdc2080addr);
    Wire.write(hum_low_addr);
    error = Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      hum_low = c;
      DEBUG_PRINT(String(c,DEC));         // print the character
    }
    error = Wire.endTransmission();
  
    Wire.beginTransmission(hdc2080addr);
    Wire.write(hum_high_addr);
    Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      hum_high = c;
      DEBUG_PRINT(String(c,DEC));         // print the character
    }
    error = Wire.endTransmission();
  }
  else if(num == 1)
  {
    Wire.beginTransmission(hdc2080addr_2);
    Wire.write(hum_low_addr);
    Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr_2, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      hum_low = (int)c;
      DEBUG_PRINT(String(c,DEC));         // print the character
    }
  
    Wire.beginTransmission(hdc2080addr_2);
    Wire.write(hum_high_addr);
    Wire.endTransmission();
    
    Wire.requestFrom(hdc2080addr_2, 1); //read one byte
    while (Wire.available()) 
    { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      hum_high = c;
      DEBUG_PRINT(String(c,DEC));         // print the character
    }
  }
  if(num > 1)
  {
    return -1;
  }
  hum = (((hum_high << 8) | hum_low) / pow(2, 16)) * 100;
  //DEBUG_PRINTLN("Humidity (Sensor, value:");
  //DEBUG_PRINT(num);
  //DEBUG_PRINT(", ");
  //DEBUG_PRINT(hum);
  return hum;
}

float ReadDS18B20()
{
  
}

union 
{
  float fval;
  byte bval[4];
} floatAsBytes;

union 
{
  int ival;
  byte bval[4];
} intAsBytes;

BH1750 lightMeter;
int moisturePin = 35;

int ledChannel = 0;
int ledPin = 2;

byte co2_read_command[] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };

int co2_pins[] = { /*16, 22, 23*/ 12, 25, 26 };

//Timer t, t2;
int timelapse_counter = 0;

float LightIntensity() 
{
  float lux = lightMeter.readLightLevel();
  if(lux < 255)
  {
    DEBUG_PRINTLN("Lux is lower than 255");
    isluxlow = true;
    //ledcWrite(ledChannel, 255 - lux);
    //turn off the led after x seconds
    //int afterEvent = t2.after(3000, turnOffLed);
    //DEBUG_PRINT("Led timer started! ID: ");
    //DEBUG_PRINTLN(afterEvent);
  }
  else
  {
    isluxlow = false;
  }
  luxvalue = lux;
  return lux;
}

int ReadMoisture() //return in percentage
{
  int readValue = analogRead(moisturePin); //send raw data
  //readValue = map(readValue, 550, 0, 0, 100);
  return readValue;
}

int receiveCO2(int co2no) //0-2
{
  int byteCounter = 0;
  int receivedbytes[9];
  if(co2no > 2 || co2no < 0)
  {
    return -1;
  }
  Serial1.begin(9600, SERIAL_8N1, co2_pins[co2no], 15); //RX = 16 | TX = 17
  //Serial.printf("Second serial on pin %d initialized", co2_pins[co2no]);
  Serial1.write(co2_read_command, sizeof(co2_read_command));
  delay(200);
  //DEBUG_PRINTLN("Second serial write command");
  if(Serial1.available() == 9)
  {
    while(Serial1.available() > 0)
    {
      receivedbytes[byteCounter] = Serial1.read();
      DEBUG_PRINT("Received value: ");
      DEBUG_PRINTLN(receivedbytes[byteCounter]);
      byteCounter++;
    }
    if(receivedbytes[0] != 255)
    {
      return -1;
    }
    if(receivedbytes[1] != 134)
    {
      return -1;
    }
    return (receivedbytes[2] * 256) + receivedbytes[3];
  }
  else
  {
    return -1;
  }
  Serial1.end();
}

void MeasurementCallback()
{
  stringToSend = "";
  stringToDisplay = "";
  timelapse_counter++;
  if(timelapse_counter == 60)
  {
    stringToDisplay += "Picture taken";
    stringToSend += "true ";
    timelapse_counter = 0;
    
  }
  else
  {
    stringToDisplay += String(60-timelapse_counter, DEC) + "left before pic.\n";
    stringToSend += "false ";
  }
  int counter = 2;
  //byte bytesToSend[32];
  //bytesToSend[0] = 0xFF;
  //bytesToSend[1] = 0x1E;

  int co2[3];
  //read 3 co2 sensor
  stringToDisplay += "CO2 values: ";
  for(int i = 0; i < 3; i++)
  {
    co2[i] = receiveCO2(i);
    //intAsBytes.ival = co2[i];
    /*for(int j = 0; j < 4; j++)
    {
      bytesToSend[counter] = intAsBytes.bval[j];
      counter++;
    }
    */
    stringToSend += String(co2[i], DEC) + " ";
    stringToDisplay += String(co2[i]) + ", ";
    DEBUG_PRINT("CO2 Concentration: ");
    DEBUG_PRINTLN(co2[i]);
  }
  stringToDisplay += "\n";
  
  float temp[2];
  float hum[2];
  StartHDCMeasurement();
  for(int i = 0; i < 2; i++)
  {
    temp[i] = ReadTemp(i);
    hum[i] = ReadHumidity(i);
    /*floatAsBytes.fval = temp[i];
    for(int j = 0; j < 4; j++)
    {
      bytesToSend[counter] = floatAsBytes.bval[j];
      counter++;
    }
    */
    stringToSend += String(temp[i], DEC) + " ";
    stringToSend += String(hum[i], DEC) + " ";
    DEBUG_PRINT("Temperature: ");
    DEBUG_PRINTLN(temp[i]);
  }

  stringToDisplay += "HDC temp 1 = " + String(temp[0], DEC) + ", ";
  stringToDisplay += "HDC temp 2 = " + String(temp[1], DEC) + ", ";
  stringToDisplay += "HDC hum 1 = " + String(hum[0], DEC) + ", ";
  stringToDisplay += "HDC hum 2 = " + String(hum[1], DEC) + ", ";
 
  float dstemp[2];
  sensors.requestTemperatures();
  for(int i = 0; i < 2; i++)
  {
    dstemp[i] = sensors.getTempCByIndex(i);
    stringToSend += String(dstemp[i], DEC) + " ";
  }
  stringToDisplay += "DS temps = " + String(dstemp[0], DEC) + ", " + String(dstemp[0], DEC) + "\n";
  
  float lux = LightIntensity();

  /*floatAsBytes.fval = lux;
  for(int i = 0; i < 4; i++)
  {
    bytesToSend[counter] = floatAsBytes.bval[i];
    counter++;
  }
  */
  stringToSend += String(lux, DEC) + " ";
  stringToDisplay += "Lux = " + String(lux, DEC) + ", ";

  //Moisture snippet
  /*bytesToSend[counter] = 50;
  counter++;
  */
  stringToSend += String(ReadMoisture(), DEC) + " ";
  stringToDisplay += "Moisture = " + String(ReadMoisture(), DEC) + ", ";
  
  DEBUG_PRINT("Lux: ");
  DEBUG_PRINTLN(lux);

  //bytesToSend[31] = 0xFF;

  //Serial.write(bytesToSend, sizeof(bytesToSend));

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Hello World!");
  //tft.println(stringToDisplay);
  Serial.println(stringToSend);

  //create an array to send
  /*
   * Header: 0xFF (1 byte)
   * Bytes to send (1 byte)
   * Light intensity (4 byte)
   * Moisture in percentage (1 byte)
   * 3 x co2 (4 byte)
   * 3 x temperature (4 byte)
   * End of packet 0xFF (1 byte)
   */
   
}

void turnOffLed()
{
  //DEBUG_PRINT("Led timer event raised!");
  ledcWrite(ledChannel, 0);
}

int led_counter = 0;

void setup() 
{
  // put your setup code here, to run once:

  //Setup usb serial
  Serial.begin(115200);
  DEBUG_PRINTLN("Serial test");

  ledcSetup(ledChannel, 5000, 8); //channel, freq, resolution
  ledcAttachPin(ledPin, ledChannel); //attach led pin to channel 0

  Wire.begin(21, 22);
  //Wire.begin(4, 15);

  Wire.beginTransmission(hdc2080addr);
  Wire.write(meas_conf_addr);
  Wire.write(meas_conf_data);
  //DEBUG_PRINTLN(Wire.endTransmission());
  Wire.endTransmission();

  Wire.beginTransmission(hdc2080addr);
  Wire.write(dev_config_addr);
  Wire.write(dev_config_data);
  //DEBUG_PRINTLN(Wire.endTransmission());
  Wire.endTransmission();
  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  
  /*
  Serial.print("Int size: ");
  Serial.println(sizeof(int));
  Serial.print("Float size: ");
  Serial.println(sizeof(float));
  */

  sensors.begin();

  //int tickEvent = t.every(2000, MeasurementCallback); //20000
  DEBUG_PRINT("Tick started! ID: ");
  DEBUG_PRINTLN(tickEvent);

  tft.init(240, 240);
  tft.fillScreen(ST77XX_BLACK); 
  tft.setCursor(0,0);
  tft.println("Setup\n");
  tft.setTextSize(2);
}

void loop() 
{
  tft.setCursor(0,0);
  tft.println("Loop\n");
  // put your main code here, to run repeatedly:
  //t.update();
  //t2.update();
  for(int i = 0; i < 20000; i++)
  {
    if(i == 3000)
    {
      turnOffLed();
    }
    delay(1);
  }
  MeasurementCallback();
  if(isluxlow == true)
  {
    ledcWrite(ledChannel, 255 - luxvalue);
  }
}
