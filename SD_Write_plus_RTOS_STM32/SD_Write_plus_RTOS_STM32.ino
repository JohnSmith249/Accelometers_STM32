#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_Accel.h>
#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
// #include <SD.h>
#include <stdlib.h>
#include "SdFat.h"
// #include "FreeStack.h"

// #include <STM32FreeRTOS.h>
// #include <semphr.h>

// SemaphoreHandle_t xSerialSemaphore;

#define SS PA15
#define RST PB7
#define DI0 PB6
#define BAND 433E6
#define LORA_PCTR PB14

#define FILE_BASE_NAME "Data"

SoftwareSerial mySerial(PA5, PA8);
SdFat SD;
SdFile file;
File myFile;

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// SdFat sd1;
// const uint8_t SD1_CS = PA7;

// const uint8_t BUF_DIM = 2048;
// uint8_t buf[BUF_DIM];

const uint32_t FILE_SIZE = 1000000;
// const uint16_t NWRITE = FILE_SIZE / BUF_DIM;

// #define errorExit(msg) errorHalt(F(msg))
// #define initError(msg) initErrorHalt(F(msg))

// void TaskSensorRead( void *pvParameters );
// void TaskSdWrite( void *pvParameters );

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

  // if (xSerialSemaphore == NULL)  // Check to confirm that the Serial Semaphore has not already been created.
  // {
  //   xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
  //   if ((xSerialSemaphore) != NULL)
  //     xSemaphoreGive((xSerialSemaphore));  // Make the Serial Port available for use, by "Giving" the Semaphore.
  // }

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
  // if (!sd1.begin(SD1_CS, SD_SCK_MHZ(18))) {
  //   sd1.initError("sd1:");
  // }
  // if (!sd1.exists("/Dir1")) {
  //   if (!sd1.mkdir("/Dir1")) {
  //     sd1.errorExit("sd1.mkdir");
  //   }
  // }

  // mySerial.println(F("------sd1 root-------"));
  // sd1.ls();

  // if (!sd1.chdir("/Dir1")) {
  //   sd1.errorExit("sd1.chdir");
  // }

  // mySerial.println(F("------sd1 Dir1-------"));
  // sd1.ls();
  // sd1.chvol();
  // SdFile file1;
  // if (!file1.open("test.bin", O_RDWR | O_CREAT | O_TRUNC)) {
  //   sd1.errorExit("file1");
  // }
  // char fileName[] = FILE_BASE_NAME "00.csv";
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

  // xTaskCreate(
  //   TaskSensorRead
  //   ,  "SensorRead"  // A name just for humans
  //   ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
  //   ,  NULL //Parameters for the task
  //   ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  //   ,  NULL ); //Task Handle

  // xTaskCreate(
  //   TaskSdWrite
  //   ,  "SdWrite" // A name just for humans
  //   ,  128  // Stack size
  //   ,  NULL //Parameters for the task
  //   ,  1  // Priority
  //   ,  NULL ); //Task Handle

  mySerial.println("Read 2048 sample !!!");
  unsigned char buf[2048];
  unsigned char data[2048];
  int counter = 0;
  sensors_event_t event;
  // ----------------------------------------------------- Sensor Test ----------------------------------------------------- //
  mySerial.println("Start read: ");
  // unsigned long Full_read_start = micros();
  
  while (counter < 7) {
    accel.getEvent(&event);
    // data[counter] == (unsigned char)accel.raw.xhigh;
    // data[counter + 1] == (unsigned char)accel.raw.yhigh;
    // data[counter + 2] == (unsigned char)accel.raw.zhigh;
    // data[counter + 3] == (unsigned char)accel.raw.xlow;
    // data[counter + 4] == (unsigned char)accel.raw.ylow;
    // data[counter + 5] == (unsigned char)accel.raw.zlow;
    unsigned long Full_read_start = micros();
    data[counter] == (char)accel.raw.x;
    data[counter + 1] == (char)accel.raw.y;
    data[counter + 2] == (char)accel.raw.z;
    mySerial.println(micros() - Full_read_start);
    counter += 3;
  }
  mySerial.println("Done read");

  // mySerial.println(F("Writing test.bin to sd1"));

  // for (uint16_t i = 0; i < NWRITE; i++) {
  //   if (file1.write(buf, sizeof(buf)) != sizeof(buf)) {
  //     sd1.errorExit("sd1.write");
  //   }
  // }

  // file1.close();

  myFile = SD.open("test22.txt", FILE_WRITE);
  if (myFile) {
    mySerial.println("begin write");
    // myFile.write(&buf[0], 2048);
    for (int i = 0; i < 2048; i++) {
      myFile.print(data[i]);
    }
    myFile.close();
    mySerial.println("done write.");
  } else {
    mySerial.println("error opening txt");
  }
}


void loop(void) {
  // mySerial.println("here");
}

// void TaskSensorRead( void *pvParameters __attribute__((unused)) ){
//   uint8_t buffer[2048];
//   int counter = 0;
//   sensors_event_t event;
//   // mySerial.println("Start read: ");
//   while (counter < 2048) {
//     if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ){
//       accel.getEvent(&event);
//       buffer[counter] == accel.raw.xhigh;
//       buffer[counter + 1] == accel.raw.yhigh;
//       buffer[counter + 2] == accel.raw.zhigh;
//       buffer[counter + 3] == accel.raw.xlow;
//       buffer[counter + 4] == accel.raw.ylow;
//       buffer[counter + 5] == accel.raw.zlow;
//       counter += 6;
//       xSemaphoreGive( xSerialSemaphore );
//     }
//     vTaskDelay(1);
//   }
//   // mySerial.println("Done read");
// }
// void TaskSdWrite( void *pvParameters __attribute__((unused)) ){
//   if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ){
//     myFile = SD.open("test.txt", FILE_WRITE);
//     if (myFile) {
//     myFile.write(&buf[0], 2048);
//     myFile.close();
//     mySerial.println("done write.");
//     } else {
//       mySerial.println("error opening test.txt");
//     }
//     xSemaphoreGive( xSerialSemaphore );
//   }
//   vTaskDelay(1);
// }
