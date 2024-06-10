#include <SoftwareSerial.h>
#include<String.h>

#define BAUD_RATE 115200
#define CHECKSUM_OPERATOR '^'
#define COM 0x55

// Define variables
uint8_t received_data_buffer[4];
uint8_t CS;
int Distance = 0; // инициализируем переменную Distance в 0
SoftwareSerial mySerial(10, 11);

// Define functions
int calculateDistance(uint8_t* buffer_RTT) {
  return (buffer_RTT[1] << 8) + buffer_RTT[2];
}

// Define function to process distance data and format NMEA string
void processDistanceData() {
  char nmea_string[40];
  double dist_double = (double)Distance / 1000.0; // Convert mm to meters
  double feet_double = dist_double * 3.2808; // Convert meters to feet
  int feet = (int)feet_double;
  int fraction_feet = (int)((feet_double - feet) * 100);
  int meters = (int)dist_double;
  int fraction_meters = (int)((dist_double - meters) * 100);
  int fathoms = (int)dist_double * 0.3048;

  sprintf(nmea_string, "$SDDBT,%d.%02d,f,%d.%02d,M,%d.%02d,F", feet, fraction_feet, meters, fraction_meters, fathoms, fraction_meters);
  int checksum = 0;
  for (int i = 1; i < strlen(nmea_string); i++) {
    checksum = checksum ^ nmea_string[i];
  }
  char checksum_char = checksum;
  sprintf(nmea_string + strlen(nmea_string), "*%02X", checksum_char);
  Serial.print(nmea_string);
  Serial.println();
}


void setup() {
  Serial.begin(BAUD_RATE);
  mySerial.begin(BAUD_RATE);
}

void loop() {
  mySerial.write(COM);
  delay(100);
  if (mySerial.available() > 0) {
    delay(4);
    if (mySerial.read() == 0xFF) {
      for (int i = 0; i < 4; i++) {
        received_data_buffer[i] = mySerial.read();
      }
      CS = received_data_buffer[0] + received_data_buffer[1] + received_data_buffer[2];
      if (received_data_buffer[3] == CS) {
        Distance = calculateDistance(received_data_buffer);
      }
    }
  }

  processDistanceData();
}
