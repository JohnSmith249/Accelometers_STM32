#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_Accel.h>
#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <SdFat.h>
// #include <STM32FreeRTOS.h>

#define SS PA15
#define RST PB7
#define DI0 PB6
#define BAND 433E6
#define LORA_PCTR PB14
#define DRDY PA5

SoftwareSerial mySerial(PA6, PA8);
File myFile;
SdFile file;
SdFat SD;

int16_t Global_Data_x[200];
int16_t Global_Data_y[200];
int16_t Global_Data_z[200];
char test_subject[10];

String File_name = "TEST25.txt";
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
  }
}

void Read_data(void) {
  sensors_event_t event;
  // accel.begin();
  accel.SetDataRate();
  for (int i = 0; i < 200; i++) {
    unsigned long start = micros();
    accel.readRawData();
    mySerial.println(micros() - start);
    // Global_Data_x[i] == accel.raw.x;
    // Global_Data_y[i] == accel.raw.y;
    // Global_Data_z[i] == accel.raw.z;
    // mySerial.print(accel.raw.x);
    // mySerial.print(accel.raw.y);
    // mySerial.println(accel.raw.z);
  }
}

void Log_data(void) {
  myFile = SD.open(File_name, FILE_WRITE);
  if (myFile) {
    mySerial.println("begin write");
    myFile.println("************************************");
    for (int i = 0; i < 200; i++) {
      myFile.print(Global_Data_x[i]);
      myFile.print(Global_Data_y[i]);
      myFile.println(Global_Data_z[i]);
    }
    myFile.close();
    mySerial.println("done write.");
  } else {
    mySerial.println("error opening txt");
  }
}

void Check_data(void) {
  for (int i = 0; i < 200; i++) {
    mySerial.print(Global_Data_x[i]);
    mySerial.print(Global_Data_y[i]);
    mySerial.println(Global_Data_z[i]);
  }
}
// TaskHandle_t Task_Handle1;
// TaskHandle_t Task_Handle2;


void Sensor_Test(void) {
  mySerial.println("Accelerometer Test !!!");
  if (!accel.begin()) {
    mySerial.println("No LSM303 detected ... Check your wiring!");
    while (1)
      ;
  } else {
    mySerial.println("Initializing Sensor Success... ");
  }
}

void SD_Test(void) {
  delay(10);
  if (SD.begin(PA7)) {
    delay(10);
    mySerial.println("SD detected and ready to use !!!");
  } else {
    mySerial.println("SD initialize failed !!!");
  }
}

void Test_array(void){
  test_subject[0] = 'c';
}

void setup() {
  pinMode(DI0, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(LORA_PCTR, OUTPUT);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  pinMode(DRDY, INPUT);
  mySerial.begin(4800);
  delay(1000);

  // xTaskCreate(Read_data,
  //             "Task1",
  //             100,
  //             NULL,
  //             1,
  //             &Task_Handle1);

  // xTaskCreate(Log_data,
  //             "Task2",
  //             100,
  //             NULL,
  //             2,
  //             &Task_Handle2);

  Sensor_Test();
  // displaySensorDetails();
  // SD_Test();

  mySerial.println("-----------------------------------------");
  Read_data();
  // delay(100);
  // Log_data();
  // Test_array();
  // mySerial.print(test_subject[0]);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// void Read_data(void *parameter) {
//   (void)parameter;
//   while (1) {
//     sensors_event_t event;
//     for (int i = 0; i < 200; i++) {
//       accel.getEvent(&event);
//       Global_Data_x[i] == (char)accel.raw.x;
//       Global_Data_y[i] == (char)accel.raw.y;
//       Global_Data_z[i] == (char)accel.raw.z;
//     }
//   }
// }

// void Log_data(void *parameter) {
//   (void)parameter;
//   while (1) {
//     myFile = SD.open(File_name, FILE_WRITE);
//     if (myFile) {
//       mySerial.println("begin write");
//       for (int i = 0; i < 200; i++) {
//         myFile.print(Global_Data_x[i]);
//         myFile.print(Global_Data_y[i]);
//         myFile.print(Global_Data_z[i]);
//       }
//       myFile.close();
//       mySerial.println("done write.");
//     } else {
//       mySerial.println("error opening txt");
//     }
//   }
// }
