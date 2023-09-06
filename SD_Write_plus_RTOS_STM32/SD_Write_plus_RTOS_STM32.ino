#include <Wire.h>
#include <Adafruit_Sensor.h>
// #include <Adafruit_LSM303_U.h>
#include <Adafruit_LSM303_Accel.h>
#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <stdlib.h>

#define SS PA15
#define RST PB7
#define DI0 PB6
#define BAND 433E6
#define LORA_PCTR PB14
SoftwareSerial mySerial(PA5, PA8);
File myFile;

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);
  mySerial.println("------------------------------------");
  mySerial.print("Sensor:       ");
  mySerial.println(sensor.name);
  mySerial.print("Driver Ver:   ");
  mySerial.println(sensor.version);
  mySerial.print("Unique ID:    ");
  mySerial.println(sensor.sensor_id);
  mySerial.print("Max Value:    ");
  mySerial.print(sensor.max_value);
  mySerial.println(" m/s^2");
  mySerial.print("Min Value:    ");
  mySerial.print(sensor.min_value);
  mySerial.println(" m/s^2");
  mySerial.print("Resolution:   ");
  mySerial.print(sensor.resolution);
  mySerial.println(" m/s^2");
  mySerial.println("------------------------------------");
  mySerial.println("");
  delay(500);
}

void setup(void) {
#ifndef ESP8266
  while (!Serial)
    ;  // will pause Zero, Leonardo, etc until serial console opens
#endif
  pinMode(DI0, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(LORA_PCTR, OUTPUT);
  digitalWrite(LORA_PCTR, LOW);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  mySerial.begin(4800);
  mySerial.println("Accelerometer Test");

  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    mySerial.println("No LSM303 detected ... Check your wiring!");
    while (1)
      ;
  }

  mySerial.println("Initializing SD card...");

  if (SD.begin(PA7)) {
    delay(10);
    mySerial.println("SD detected !!!");
    mySerial.println("initialization done.");
  } else {
    mySerial.println("SD initialize failed !!!");
  }
  
  displaySensorDetails();

// ------------------------------------------------ Display sensor config ------------------------------------------------ //

  accel.setRange(LSM303_RANGE_4G);
  mySerial.print("Range set to: ");
  lsm303_accel_range_t new_range = accel.getRange();
  switch (new_range) {
    case LSM303_RANGE_2G:
      mySerial.println("+- 2G");
      break;
    case LSM303_RANGE_4G:
      mySerial.println("+- 4G");
      break;
    case LSM303_RANGE_8G:
      mySerial.println("+- 8G");
      break;
    case LSM303_RANGE_16G:
      mySerial.println("+- 16G");
      break;
  }

  accel.setMode(LSM303_MODE_NORMAL);
  mySerial.print("Mode set to: ");
  lsm303_accel_mode_t new_mode = accel.getMode();
  switch (new_mode) {
    case LSM303_MODE_NORMAL:
      mySerial.println("Normal");
      break;
    case LSM303_MODE_LOW_POWER:
      mySerial.println("Low Power");
      break;
    case LSM303_MODE_HIGH_RESOLUTION:
      mySerial.println("High Resolution");
      break;
// ------------------------------------------------ Display sensor config ------------------------------------------------ //
  }
  mySerial.println("Read 16384 sample !!!");

  String Test_Sensor_Data = "";
  int counter = 0;
  /* Get a new sensor event */
  sensors_event_t event;
  // ----------------------------------------------------- Sensor Test ----------------------------------------------------- //
  mySerial.println("Whole read and save process cost: ");
  unsigned long start_read = micros();
  while (counter < 16384) {
    accel.getEvent(&event);
    unsigned long start = micros();
    Test_Sensor_Data += (unsigned char)accel.raw.xhigh;
    Test_Sensor_Data += (unsigned char)accel.raw.yhigh;
    Test_Sensor_Data += (unsigned char)accel.raw.zhigh;
    Test_Sensor_Data += (unsigned char)accel.raw.xlow;
    Test_Sensor_Data += (unsigned char)accel.raw.ylow;
    Test_Sensor_Data += (unsigned char)accel.raw.zlow;
    mySerial.println(micros()-start);
    counter += 6;
  }
  mySerial.println(micros() - start_read);
  mySerial.println("Done read");

  mySerial.println("Total Record process cost: ");
  unsigned long start_record = micros();
  myFile = SD.open("test18.txt", FILE_WRITE);
  if (myFile) {
    myFile.write(&Test_Sensor_Data[0], Test_Sensor_Data.length());
    myFile.close();
  }
  mySerial.println(micros() - start_record);
  mySerial.println("End Record");
  mySerial.println("Done recording");
}

void loop(void) {
// Nothing here

}
