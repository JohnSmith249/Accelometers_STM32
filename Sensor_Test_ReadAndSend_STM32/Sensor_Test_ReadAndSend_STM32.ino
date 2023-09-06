#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_Accel.h>
#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include <Arduino_FreeRTOS.h>


#define SS PA15
#define RST PB7
#define DI0 PB6
#define BAND 433E6
#define LORA_PCTR PB14
SoftwareSerial mySerial(PA5, PA8);  //RX, TX


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

void ReadSensor(void *pvParameters);
void SendPackage(void *pvParameters);

void setup(void) {
#ifndef ESP8266
  while (!Serial)
    ;  // will pause Zero, Leonardo, etc until serial console opens
#endif
  pinMode(DI0, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(LORA_PCTR, OUTPUT);
  digitalWrite(LORA_PCTR, HIGH);
  delay(1000);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    mySerial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  mySerial.begin(4800);
  mySerial.println("LoRa Sender");
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
  /* Display some basic information on this sensor */
  displaySensorDetails();

  // ------------------------------------------------ Display sensor config ------------------------------------------------ //

  accel.setRange(LSM303_RANGE_4G);
  Serial.print("Range set to: ");
  lsm303_accel_range_t new_range = accel.getRange();
  switch (new_range) {
    case LSM303_RANGE_2G:
      Serial.println("+- 2G");
      break;
    case LSM303_RANGE_4G:
      Serial.println("+- 4G");
      break;
    case LSM303_RANGE_8G:
      Serial.println("+- 8G");
      break;
    case LSM303_RANGE_16G:
      Serial.println("+- 16G");
      break;
  }

  accel.setMode(LSM303_MODE_NORMAL);
  Serial.print("Mode set to: ");
  lsm303_accel_mode_t new_mode = accel.getMode();
  switch (new_mode) {
    case LSM303_MODE_NORMAL:
      Serial.println("Normal");
      break;
    case LSM303_MODE_LOW_POWER:
      Serial.println("Low Power");
      break;
    case LSM303_MODE_HIGH_RESOLUTION:
      Serial.println("High Resolution");
      break;
      // ------------------------------------------------ Display sensor config ------------------------------------------------ //
  }

  xTaskCreate(
    ReadSensor
    , "Read Sensor"
    , 256
    , NULL
    , 1
    , NULL);

  xTaskCreate(
    SendPackage
    , "Send Package"
    , 256
    , NULL
    , 2
    , NULL);

}

void loop(void) {
  // int16_t Sensor_Data[126] = { 0 };
  // int counter = 0;
  /* Get a new sensor event */
  // sensors_event_t event;
  // accel.getEvent(&event);
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

  while (counter < 126) {
    // mySerial.println(counter);
    Sensor_Data[counter] = accel.raw.x;  //0
    counter++;                           //1
    // mySerial.println(counter);
    Sensor_Data[counter] = accel.raw.y;  //1
    counter++;                           //2
    // mySerial.println(counter);
    Sensor_Data[counter] = accel.raw.z;  //2
    counter++;                           //3
    // mySerial.println(counter);
    // delay(500);
  }
  //mySerial.println(sizeof(Sensor_Data));  //252bytes
  unsigned long start = micros();
  LoRa.beginPacket();
  for (int i = 0; i < 126; i++) {
    LoRa.print(Sensor_Data[i]);
  }
  LoRa.endPacket();
  mySerial.println(micros() - start);

  // ------------------------------------------------- Sensor Data Recorder ------------------------------------------------- //
}

void ReadSensor(void *pvParameters) {
  (void)pvParameters;
  for (;;) {
    int16_t Sensor_Data[126] = { 0 };
    int counter = 0;
    sensors_event_t event;
    accel.getEvent(&event);
    while (counter < 126) {
      Sensor_Data[counter] = accel.raw.x;  //0
      counter++;                           //1
      Sensor_Data[counter] = accel.raw.y;  //1
      counter++;                           //2
      Sensor_Data[counter] = accel.raw.z;  //2
      counter++;                           //3
    }
  }
}

void SendPackage(void *pvParameters) {
  (void) *pvParameters;
  LoRa.beginPacket();
  for (int i = 0; i < 126; i++) {
    LoRa.print(Sensor_Data[i]);
  }
  LoRa.endPacket();
}
