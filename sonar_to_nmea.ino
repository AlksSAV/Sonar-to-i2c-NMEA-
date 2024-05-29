#include <SoftwareSerial.h>
#include <string.h>

#define COM 0x55

// Define variables
uint8_t buffer_RTT[4];
uint8_t CS;
int Distance = 0;
SoftwareSerial mySerial(10, 11);

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
  mySerial.write(COM);
  delay(100);
  if (mySerial.available() > 0) {
    delay(4);
    if (mySerial.read() == 0xFF) {
      buffer_RTT[0] = 0xFF;
      for (int i = 1; i < 4; i++) {
        buffer_RTT[i] = mySerial.read();
      }
      CS = buffer_RTT[0] + buffer_RTT[1] + buffer_RTT[2];
      if (buffer_RTT[3] == CS) {
        Distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
        char nmea_string[15];
        sprintf(nmea_string, "$DPT,%1d.%02d,M,*,*", Distance / 10, Distance % 100);

        int checksum = 0;
        for (int i = 1; i < strlen(nmea_string); i++) {
          checksum ^= nmea_string[i];
        }

        char checksum_char = checksum;
        char nmea_string_with_checksum[16];
        strcpy(nmea_string_with_checksum, nmea_string);
        sprintf(&nmea_string_with_checksum[strlen(nmea_string)], "*%02X", checksum_char);

        Serial.print(nmea_string_with_checksum);
        Serial.println();
      }
    }
  }
}
