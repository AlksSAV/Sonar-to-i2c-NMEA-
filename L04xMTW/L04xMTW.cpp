#include "Arduino.h"
#include "L04xMTW.h"
#include "SoftwareSerial.h"

SoftwareSerial mySerial(10, 11);

L04xMTW::L04xMTW(int rxPin, int txPin) {
  _rxPin = rxPin;
  _txPin = txPin;
}

void L04xMTW::begin() {
  mySerial.begin(115200);
}

int L04xMTW::readDistance() {
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