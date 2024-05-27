#ifndef L04xMTW_h
#define L04xMTW_h

#include "Arduino.h"

class L04xMTW {
  public:
    L04xMTW(int rxPin, int txPin);
    void begin();
    int readDistance();
  private:
    int _rxPin;
    int _txPin;
};
#endif