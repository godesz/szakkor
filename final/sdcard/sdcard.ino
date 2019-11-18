#include <SD.h>
#include <SPI.h>

File sdcard_file;

int CS_pin = 10; // Pin 10 on Arduino Uno


void setup() {
  Serial.begin(9600);

  pinMode(CS_pin, OUTPUT);

  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  sdcard_file = SD.open("data.txt", FILE_WRITE);
  if (sdcard_file) { 
    sdcard_file.print("alma2222");
    sdcard_file.print("     ");   
    sdcard_file.println("alma2222");
    sdcard_file.close(); // close the file
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
}

void loop() {

 
  sdcard_file = SD.open("data.txt", FILE_WRITE);
  if (sdcard_file) {    
    sdcard_file.print("alma");
    sdcard_file.print("     ");   
    sdcard_file.println("alma");
    sdcard_file.close(); // close the file
    Serial.println("Written");
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
  delay(3000);
}
