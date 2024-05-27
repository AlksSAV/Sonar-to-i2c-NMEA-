#include <Wire.h>
#include "L04xMTW.h"

#define I2C_SLAVE_ADDR 0x09
byte received;

const int numReadings = 1;  //number of readings to average.
int MODE = 1;

int distances = 0;
int distance = 0;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;


L04xMTW sensor(10, 11);  // RX, TX


//look for i2c read read request
void receiveEvent(int howMany) {
  while (Wire.available()) {  // loop through all but the last
    {
      received = Wire.read();
      if (received == 0x51) {
        //Pings the sonar in mode 2
        if (MODE == 2) {
          void readsonar();
        }
      }
    }
  }
}
//send results over i2c
void requestEvent() {
  Wire.write(highByte(distance));
  Wire.write(lowByte(distance));
}


void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(receiveEvent);  // register event
  Wire.onRequest(requestEvent);
  sensor.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("STARTUP");
}

void loop() {

  void read_sonar();
  void average_sonar();
}


void read_sonar() {
  int distances = sensor.readDistance();
  if (distances >= 0) {
    Serial.println("Distance: " + String(distance) + " mm");
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (distances == -1) {
    Serial.println("Checksum error");
    digitalWrite(LED_BUILTIN, LOW); 
  } else if (distances == -2) {
    Serial.println("Not enough data");
    digitalWrite(LED_BUILTIN, LOW); 
  }
  delay(14);
}


void average_sonar() {

  {

    total = total - readings[readIndex];
    readings[readIndex] = distances;
    total = total + readings[readIndex];
    readIndex = readIndex + 1;
    if (readIndex >= numReadings) {
      readIndex = 0;
    }
    average = total / numReadings;
  }
  distance = (average / 10);
}