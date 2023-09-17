#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_Accel.h>
#include <Arduino.h>
#include <Wire.h>
#include <LoRa.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <SdFat.h>

#define SS PA15
#define RST PB7
#define DI0 PB6
#define BAND 433E6
#define LORA_PCTR PB14
#define DRDY PA5

byte ACC_ADDRESS = 0x19;
byte ACC_CTRL_REG1_A = 0x20;
byte ACC_CTRL_REG4_A = 0x23;
byte ACC_OUT_X_L_A = 0x28;
byte ACC_OUT_X_H_A = 0x29;
byte ACC_OUT_Y_L_A = 0x2A;
byte ACC_OUT_Y_H_A = 0x2B;
byte ACC_OUT_Z_L_A = 0x2C;
byte ACC_OUT_Z_H_A = 0x2D;

SoftwareSerial mySerial(PA6, PA8);
File myFile;
SdFile file;
SdFat SD;

const int Buffer_Size = 100;

uint8_t Global_Data_xhi[Buffer_Size];
uint8_t Global_Data_xlo[Buffer_Size];
uint8_t Global_Data_yhi[Buffer_Size];
uint8_t Global_Data_ylo[Buffer_Size];
uint8_t Global_Data_zhi[Buffer_Size];
uint8_t Global_Data_zlo[Buffer_Size];

String File_name = "Test_Accel_Data_2_uint8_t_type.txt";
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
  
  accel.SetDataRate();
  for (int i = 0; i < Buffer_Size; i++) {
    unsigned long start_read = micros();
    accel.getEvent(&event);
    mySerial.println(micros() - start_read);
    Global_Data_xhi[i] = accel.raw.xhig;
    Global_Data_xlo[i] = accel.raw.xlow;
    Global_Data_yhi[i] = accel.raw.yhig;
    Global_Data_ylo[i] = accel.raw.ylow;
    Global_Data_zhi[i] = accel.raw.zhig;
    Global_Data_zlo[i] = accel.raw.zlow;
    
  }
}

void Log_data(void) {
  myFile = SD.open(File_name, FILE_WRITE);
  if (myFile) {
    for (int i=0; i< Buffer_Size; i++){
      // unsigned long start = micros();
      myFile.print(Global_Data_xhi[i]);myFile.print(",");
      myFile.print(Global_Data_xlo[i]);myFile.print(",");
      myFile.print(Global_Data_yhi[i]);myFile.print(",");
      myFile.print(Global_Data_ylo[i]);myFile.print(",");
      myFile.print(Global_Data_zhi[i]);myFile.print(",");
      myFile.print(Global_Data_zlo[i]);myFile.println(",");
      // mySerial.println(micros() - start);
    }
    myFile.close();
  } else {
    mySerial.println("error opening txt");
  }
}

void SD_Test(void) {
  delay(100);
  mySerial.println("SD Test !!!");
  if (SD.begin(PA7)) {
    delay(10);
    mySerial.println("SD detected and ready to use !!!");
  } else {
    mySerial.println("SD initialize failed !!!");
  }
}

void Sensor_Test(void) {
  mySerial.println("Accelerometer Test !!!");
  if (!accel.begin()) {
    mySerial.println("No LSM303 detected ... Check your wiring!");
    while (1)
      ;
  } else {
    mySerial.println("Initializing Sensor Success... ");
    displaySensorDetails();
  }
}

void setup() {
  pinMode(DI0, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(LORA_PCTR, OUTPUT);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  mySerial.begin(4800);
  
  Sensor_Test();
  SD_Test();
  // unsigned long start = micros();
  // Read_data();
  // mySerial.println(micros() - start);
  // Log_data();
  
}

void loop() {
  
}
