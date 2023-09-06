#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
// #include <STM32SD.h>

#define SS PA15
#define RST PB7
#define DI0 PB6
#define BAND 433E6
#define LORA_PCTR PB14
SoftwareSerial mySerial(PA5, PA8);

/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached

 This example code is in the public domain.

 */

#include <STM32SD.h>

// If SD card slot has no detect pin then define it as PB11
// to ignore it. One other option is to call 'SD.begin()' without parameter.
// #ifndef SD_DETECT_PIN
// #define SD_DETECT_PIN PB11
// #endif

File myFile;

void setup()
{
  // Open serial communications and wait for port to open:
  mySerial.begin(4800);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  mySerial.print("Initializing SD card...");
  while (!SD.begin(PB11))
  {
    delay(10);
    mySerial.println("detected !!!");
  }
  mySerial.println("initialization done.");

// ----------------------------------------------------- SD Card Test ----------------------------------------------------- //
// open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // myFile = SD.open("test.txt", FILE_WRITE);

  // // if the file opened okay, write to it:
  // if (myFile) {
  //   Serial.print("Writing to test.txt...");
  //   myFile.println("testing 1, 2, 3.");
  //   // close the file:
  //   myFile.close();
  //   Serial.println("done.");
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening test.txt");
  // }

  // // re-open the file for reading:
  // myFile = SD.open("test.txt");
  // if (myFile) {
  //   Serial.println("test.txt:");

  //   // read from the file until there's nothing else in it:
  //   while (myFile.available()) {
  //     Serial.write(myFile.read());
  //   }
  //   // close the file:
  //   myFile.close();
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening test.txt");
  // }
  // Serial.println("###### End of the SD tests ######");

// ----------------------------------------------------- SD Card Test ----------------------------------------------------- //
}

void loop()
{
  // nothing happens after setup
}
