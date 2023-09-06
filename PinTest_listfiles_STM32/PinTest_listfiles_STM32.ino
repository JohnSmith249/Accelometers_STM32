/*
  Listfiles

  This example shows how print out the files in a
  directory on a SD card

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
  modified 2 Feb 2014
  by Scott Fitzgerald

  This example code is in the public domain.

*/
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#define LORA_PCTR PB14
SoftwareSerial mySerial(PA5, PA8);
File root;
#define SS PA15
void setup() {
  // Open serial communications and wait for port to open:
  mySerial.begin(4800);
  pinMode(LORA_PCTR, OUTPUT);
  digitalWrite(LORA_PCTR, LOW);
  pinMode(PB11, OUTPUT);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  while (!mySerial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  mySerial.print("Initializing SD card...");
  // SPI.begin(5, 19, 27, PB11);
  if (!SD.begin(PA7)) {
    mySerial.println("initialization failed!");
    while (1);
  }
  mySerial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  mySerial.println("done!");
}

void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      mySerial.print('\t');
    }
    mySerial.print(entry.name());
    if (entry.isDirectory()) {
      mySerial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      mySerial.print("\t\t");
      mySerial.println(entry.size(), DEC);
    }
    entry.close();
  }
}



