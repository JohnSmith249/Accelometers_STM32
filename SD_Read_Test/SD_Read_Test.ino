/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
// #include <Adafruit_LSM303_U.h>
#include <Adafruit_LSM303_Accel.h>
#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include <SD.h>

#define SS PA15
#define RST PB7
#define DI0 PB6
#define BAND 433E6
#define LORA_PCTR PB14
SoftwareSerial mySerial(PA5, PA8);
File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  pinMode(DI0, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(LORA_PCTR, OUTPUT);
  digitalWrite(LORA_PCTR, LOW);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  mySerial.begin(4800);
  mySerial.println("Accelerometer Test");
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }


  mySerial.print("Initializing SD card...");
  char Data[96];
  int counter = 0;
  if (!SD.begin(PA7)) {
    mySerial.println("initialization failed!");
    while (1)
      ;
  }
  mySerial.println("SD detected !!!");
  mySerial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // re-open the file for reading:
  myFile = SD.open("test13.txt");
  if (myFile) {
    mySerial.println("test13.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Data[counter] = myFile.read();
      counter++;
      // delay(10);
    }
    // close the file:
    myFile.close();
    for (int i = 0; i < 96; i++) {
      mySerial.println(Data[i]);
    }
  } else {
    // if the file didn't open, print an error:
    mySerial.println("error opening test13.txt");
  }
}

void loop() {
  // nothing happens after setup
}
