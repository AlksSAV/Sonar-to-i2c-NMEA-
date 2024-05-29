#include <Wire.h>

#define I2C_SLAVE_ADDR 0x09
byte received;


int distances = 0;
int distance = 0;

unsigned char buffer_RTT[4] = {0};
uint8_t CS;
#define COM 0x55
int Distance = 0;


//look for i2c read read request
void receiveEvent(int howMany) {
  while (Wire.available()) {  // loop through all but the last
    {
      received = Wire.read();
      if (received == 0x51) {
        
        }
      }
    }
  }

//send results over i2c
void requestEvent() {
  Wire.write(highByte(distances));
  Wire.write(lowByte(distances));
}


void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(receiveEvent);  // register event
  Wire.onRequest(requestEvent);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

 
    Serial.write(COM);  
    delay(100);
    digitalWrite(LED_BUILTIN, LOW); 
    if(Serial.available() > 0){
    delay(4);
    if(Serial.read() == 0xff){    
      buffer_RTT[0] = 0xff;
      for (int i=1; i<4; i++){
        buffer_RTT[i] = Serial.read();   
      }
      CS = buffer_RTT[0] + buffer_RTT[1]+ buffer_RTT[2];  
      if(buffer_RTT[3] == CS) {
        distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
        distances = (distance/10);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);

      }
    }
    }
}
