#include "Arduino.h"
#include "GL04xMT.h"
#include "SoftwareSerial.h"

SoftwareSerial mySerial(10, 11);

GL04xMT::GL04xMT(int rxPin, int txPin) {
  _rxPin = rxPin;
  _txPin = txPin;
}

void GL04xMT::begin() {
  mySerial.begin(115200);
}

int GL04xMT::readDistance() {
  if (mySerial.available() >= 4) {
    byte frameHeader = mySerial.read();
    byte dataH = mySerial.read();
    byte dataL = mySerial.read();
    byte checksum = mySerial.read();

    if ((frameHeader + dataH + dataL) & 0xFF == checksum) {
      return dataH * 256 + dataL;
    } else {
      return -1;  // return -1 in case of checksum error
    }
  } else {
    return -2;  // return -2 if not enough data is available
  }
}