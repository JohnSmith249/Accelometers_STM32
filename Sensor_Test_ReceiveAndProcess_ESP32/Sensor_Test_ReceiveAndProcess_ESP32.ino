#include <SPI.h>
#include <LoRa.h>

#define SW2 22
#define SS 18
#define RST 14
#define DI0 26
#define BAND 433E6

void setup() {
  Serial.begin(115200);
  pinMode(DI0, OUTPUT);
  pinMode(RST, OUTPUT);
  LoRa.setPins(SS, RST, DI0);
  // pinMode(LORA_PCTR, OUTPUT);
  // digitalWrite(LORA_PCTR, HIGH);
  SPI.begin(5, 19, 27, SS);
  Serial.println("LoRa Receiver");
  Serial.println("Restart module");

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  } else {
    Serial.println("Initiate LoRa Success !!!");
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  // Serial.println("here");
  if (packetSize) {
    // received a packet
    // Serial.println("here 1");
    Serial.print("Received packet '");
    // char test[60];
    int i = 0;
    // read packet
    unsigned long current_time = micros();
    while (LoRa.available()) {
      Serial.println(LoRa.read());
    }
    Serial.println("***********************************************************************");
    Serial.println(micros() - current_time);
    // delay(500);
    // Serial.println("here");

    // print RSSI of packet
    // Serial.print(" with RSSI ");
    // Serial.println(LoRa.packetRssi());
  }
}
