#ifndef GL04xMT_h
#define GL04xMT_h

#include "Arduino.h"

class GL04xMT {
  public:
    GL04xMT(int rxPin, int txPin);
    void begin();
    int readDistance();
  private:
    int _rxPin;
    int _txPin;
};
#endif