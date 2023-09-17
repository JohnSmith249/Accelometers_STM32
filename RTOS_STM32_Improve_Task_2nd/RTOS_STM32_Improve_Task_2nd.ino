#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_Accel.h>
#include <Arduino.h>
#include <Wire.h>
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

byte ACC_ADDRESS = 0x19;  //0011001
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

const int Buffer_Size = 10;

uint8_t Global_Data_xhi[Buffer_Size];
uint8_t Global_Data_xlo[Buffer_Size];
uint8_t Global_Data_yhi[Buffer_Size];
uint8_t Global_Data_ylo[Buffer_Size];
uint8_t Global_Data_zhi[Buffer_Size];
uint8_t Global_Data_zlo[Buffer_Size];
char test_subject[10];

String File_name = "TEST26.txt";
// Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// void displaySensorDetails(void) {
//   sensor_t sensor;
//   accel.getSensor(&sensor);
//   mySerial.println("------------------------------------");
//   mySerial.print("Sensor:       ");
//   mySerial.println(sensor.name);
//   mySerial.print("Driver Ver:   ");
//   mySerial.println(sensor.version);
//   mySerial.print("Unique ID:    ");
//   mySerial.println(sensor.sensor_id);
//   mySerial.print("Max Value:    ");
//   mySerial.print(sensor.max_value);
//   mySerial.println(" m/s^2");
//   mySerial.print("Min Value:    ");
//   mySerial.print(sensor.min_value);
//   mySerial.println(" m/s^2");
//   mySerial.print("Resolution:   ");
//   mySerial.print(sensor.resolution);
//   mySerial.println(" m/s^2");
//   mySerial.println("------------------------------------");
//   mySerial.println("");
//   delay(500);

//   accel.setRange(LSM303_RANGE_4G);
//   mySerial.print("Range set to: ");
//   lsm303_accel_range_t new_range = accel.getRange();
//   switch (new_range) {
//     case LSM303_RANGE_2G:
//       mySerial.println("+- 2G");
//       break;
//     case LSM303_RANGE_4G:
//       mySerial.println("+- 4G");
//       break;
//     case LSM303_RANGE_8G:
//       mySerial.println("+- 8G");
//       break;
//     case LSM303_RANGE_16G:
//       mySerial.println("+- 16G");
//       break;
//   }

//   accel.setMode(LSM303_MODE_NORMAL);
//   mySerial.print("Mode set to: ");
//   lsm303_accel_mode_t new_mode = accel.getMode();
//   switch (new_mode) {
//     case LSM303_MODE_NORMAL:
//       mySerial.println("Normal");
//       break;
//     case LSM303_MODE_LOW_POWER:
//       mySerial.println("Low Power");
//       break;
//     case LSM303_MODE_HIGH_RESOLUTION:
//       mySerial.println("High Resolution");
//       break;
//   }
// }

void Read_data(void) {
  // sensors_event_t event;
  // accel.SetDataRate();
  // for (int i = 0; i < 10; i++) {
  // unsigned long start = micros();
  // accel.getEvent(&event);
  // mySerial.println(micros() - start);
  // Global_Data_xhi[i] = accel.raw.xhig;
  // Global_Data_xlo[i] = (unsigned char) accel.raw.xlow;
  // Global_Data_yhi[i] = (unsigned char) accel.raw.yhig;
  // Global_Data_ylo[i] = (unsigned char) accel.raw.ylow;
  // Global_Data_zhi[i] = (unsigned char) accel.raw.zhig;
  // Global_Data_zlo[i] = (unsigned char) accel.raw.zlow;
  // mySerial.println(accel.raw.zlow);
  // mySerial.println((unsigned char) accel.raw.xhig);
  // mySerial.write(accel.raw.xhig);
  // mySerial.println("*****************");
  // mySerial.println(sizeof(accel.raw.xhig));
  // mySerial.println(sizeof(accel.raw.xlow));
  // mySerial.println(sizeof(accel.raw.yhig));
  // mySerial.println(sizeof(accel.raw.ylow));
  // mySerial.println(sizeof(accel.raw.zhig));
  // mySerial.println(sizeof(accel.raw.zlow));
  // }
}

void I2C_Read(void) {
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(ACC_OUT_X_L_A | 0x80);
  // Wire.write(ACC_OUT_X_H_A | 0x80);
  // Wire.write(ACC_OUT_Y_L_A | 0x80);
  // Wire.write(ACC_OUT_Y_H_A | 0x80);
  // Wire.write(ACC_OUT_Z_L_A | 0x80);
  // Wire.write(ACC_OUT_Z_H_A | 0x80);
  Wire.endTransmission();
  Wire.requestFrom((byte)ACC_ADDRESS, (byte)6);
  while (Wire.available() < 6)
    ;
  uint8_t xla = Wire.read();
  uint8_t xha = Wire.read();
  uint8_t yla = Wire.read();
  uint8_t yha = Wire.read();
  uint8_t zla = Wire.read();
  uint8_t zha = Wire.read();
  mySerial.println(xla);
  mySerial.println(xha);
  mySerial.println(yla);
  mySerial.println(yha);
  mySerial.println(zla);
  mySerial.println(zha);
  mySerial.println("***********************");
}

void Log_data(void) {
  myFile = SD.open(File_name, FILE_WRITE);
  if (myFile) {
    mySerial.println("begin write");
    // myFile.println("************************************");
    // for (int i = 0; i < 10; i++) {
    // myFile.print(Global_Data_xhi[i]);
    //   myFile.print(Global_Data_y[i]);
    //   myFile.println(Global_Data_z[i]);
    // }
    myFile.write(&Global_Data_xhi[0], 10);
    // myFile.write(&Global_Data_yhi[0], 200);
    // myFile.write(&Global_Data_zhi[0], 200);
    // myFile.write(&Global_Data_xlo[0], 200);
    // myFile.write(&Global_Data_ylo[0], 200);
    // myFile.write(&Global_Data_zlo[0], 200);
    myFile.close();
    mySerial.println("done write.");
  } else {
    mySerial.println("error opening txt");
  }
}

void Check_data(void) {
  mySerial.println("Data check, method write :");
  // mySerial.write(&Global_Data_xhi[0], 10);
  // mySerial.write(&Global_Data_yhi[0], 200);
  // mySerial.write(&Global_Data_zhi[0], 200);
  // mySerial.write(&Global_Data_xlo[0], 200);
  // mySerial.write(&Global_Data_ylo[0], 200);
  // mySerial.write(&Global_Data_zlo[0], 200);
  for (int i = 0; i < 10; i++) {
    mySerial.println(Global_Data_xhi[i]);
    //   mySerial.println(Global_Data_yhi[i]);
    //   mySerial.print(Global_Data_zhi[i]);
    //   mySerial.println(Global_Data_xlo[i]);
    //   mySerial.print(Global_Data_ylo[i]);
    //   mySerial.println(Global_Data_zlo[i]);
  }
}

// void Sensor_Test(void) {
//   mySerial.println("Accelerometer Test !!!");
//   if (!accel.begin()) {
//     mySerial.println("No LSM303 detected ... Check your wiring!");
//     while (1)
//       ;
//   } else {
//     mySerial.println("Initializing Sensor Success... ");
//   }
// }

void SD_Test(void) {
  delay(100);
  if (SD.begin(PA7)) {
    delay(10);
    mySerial.println("SD detected and ready to use !!!");
  } else {
    mySerial.println("SD initialize failed !!!");
  }
}

void Test_array(void) {
  test_subject[0] = 'c';
}

void setup() {
  pinMode(DI0, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(LORA_PCTR, OUTPUT);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  mySerial.begin(4800);
  delay(1000);
  Wire.begin();
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(ACC_CTRL_REG1_A);
  Wire.write(01110111);
  Wire.endTransmission();

  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(ACC_CTRL_REG4_A);
  Wire.write(00000000);
  Wire.endTransmission();

  // Sensor_Test();
  // displaySensorDetails();
  SD_Test();

  mySerial.println("-----------------------------------------");
  I2C_Read();
  // Read_data();
  // Check_data();
  // delay(100);
  // Log_data();
  // Test_array();
  // mySerial.print(test_subject[0]);
}

void loop() {
  // Empty
}
