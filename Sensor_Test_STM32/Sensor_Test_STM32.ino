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
  // mySerial.println("");

  /* Initialise the sensor */
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
  /* Display some basic information on this sensor */
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
}

void loop(void) {
  char Sensor_Data[96] = {};
  int counter = 0;
  /* Get a new sensor event */
  sensors_event_t event;
  
  // ----------------------------------------------------- Sensor Test ----------------------------------------------------- //

  /* Display the results (acceleration is measured in m/s^2) */
  // mySerial.print("X: ");
  // mySerial.print(event.acceleration.x); mySerial.print("  ");
  // mySerial.print("Y: ");
  // mySerial.print(event.acceleration.y); mySerial.print("  ");
  // mySerial.print("Z: ");
  // mySerial.println(event.acceleration.z); mySerial.print("  ");
  // mySerial.println("m/s^2 ");

  // ----------------------------------------------------- Sensor Test ----------------------------------------------------- //

  // --------------------------------------------- Sensor sample accquired time --------------------------------------------- //

  // unsigned long start = micros();
  // event.acceleration.x;
  // event.acceleration.y;
  // event.acceleration.z;
  // unsigned long end = micros() - start;
  // mySerial.println(end);

  // --------------------------------------------- Sensor sample accquired time --------------------------------------------- //

  // ------------------------------------------------- Sensor Raw data test ------------------------------------------------- //

  // mySerial.println(accel.raw.x);
  // mySerial.println();
  // mySerial.print(accel.raw.y);mySerial.println();
  // mySerial.print(accel.raw.z);mySerial.println();
  // mySerial.println("=============================================================");

  // ------------------------------------------------- Sensor Raw data test ------------------------------------------------- //

  // ------------------------------------------------- Sensor Data Recorder ------------------------------------------------- //
  // mySerial.println("here");
  // unsigned long start = micros();


  while (counter < 96) {
    accel.getEvent(&event);
    // mySerial.println(counter);
    Sensor_Data[counter] = (char)accel.raw.xhigh;  //0
    counter++;
    Sensor_Data[counter] = (char)accel.raw.xlow;  //0
    counter++;                           //1
    // mySerial.println(counter);
    Sensor_Data[counter] = (char)accel.raw.yhigh;  //1
    counter++;
    Sensor_Data[counter] = (char)accel.raw.ylow;  //1
    counter++;                           //2
    // mySerial.println(counter);
    Sensor_Data[counter] = (char)accel.raw.zhigh;  //2
    counter++;
    Sensor_Data[counter] = (char)accel.raw.zlow;  //2
    counter++;                           //3
    // mySerial.println(counter);
    // delay(500);
  }


  mySerial.println("Start package");
  for (int i =0; i<96; i++){
    mySerial.println((uint8_t)Sensor_Data[i]);
  }
  // mySerial.println((char*)Sensor_Data);
  mySerial.println("End package");
  // mySerial.println(micros() - start);
  // delay(500);
  // mySerial.println("End here");
  // mySerial.println(sizeof(Sensor_Data));  //252bytes
  // delay(1000);

  // for (int i=0; i<126; i+=3){
  //   mySerial.print(Sensor_Data[i]);mySerial.print(' ');
  //   mySerial.print(Sensor_Data[i+1]);mySerial.print(' ');
  //   mySerial.println(Sensor_Data[i+2]);
  // }
  // delay(10000);
  // const uint8_t temp[1024]={10};
  // unsigned long start = micros();
  myFile = SD.open("test11.txt", FILE_WRITE);
  if (myFile) {
    // Serial.print("Recording data...");
    // for (int i = 0; i < 4092; i += 6) {
    //   myFile.print(Sensor_Data[i]);
    //   myFile.print(' ');
    //   myFile.print(Sensor_Data[i + 1]);
    //   myFile.print(' ');
    //   myFile.println(Sensor_Data[i + 2]);
      // myFile.print("test");myFile.print(' ');
      // myFile.print("test1");myFile.print(' ');
      // myFile.println("test2");
      // myFile.print("")
    myFile.print(Sensor_Data);
    myFile.close(); 
  }
  // mySerial.println(micros() - start);
  // mySerial.println(accel.raw.xhigh);
  mySerial.println("Done recording");
// mySerial.println("Sensor_Data");
//   mySerial.println((uint32_t)Sensor_Data);
//   mySerial.println(*Sensor_Data);
//   mySerial.println((uint32_t)&Sensor_Data);
//   mySerial.println((uint32_t)&Sensor_Data[0]);
//   mySerial.println(Sensor_Data[0]);

  // ------------------------------------------------- Sensor Data Recorder ------------------------------------------------- //

  // delay(500);
  while(1);
}
