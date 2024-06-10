#include <SoftwareSerial.h>
#include "mavlink/common/mavlink.h"  // Mavlink interface
#include "mavlink/common/mavlink_msg_distance_sensor.h"
unsigned char buffer_RTT[4] = { 0 };
uint8_t CS;
#define COM 0x55
int Distance = 0;
SoftwareSerial mySerial(10, 11);

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
  read_sonar();
  printvalue();
  send_mavlink();
}

void read_sonar() {
  mySerial.write(COM);
  delay(100);
  if (mySerial.available() > 0) {
    delay(4);
    if (mySerial.read() == 0xff) {
      buffer_RTT[0] = 0xff;
      for (int i = 1; i < 4; i++) {
        buffer_RTT[i] = mySerial.read();
      }
      CS = buffer_RTT[0] + buffer_RTT[1] + buffer_RTT[2];
      if (buffer_RTT[3] == CS) {
        Distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
      }
    }
  }
}


void printvalue() {
  Serial.print("Distance:");
  Serial.print(Distance);
  Serial.println("mm");
}

void send_mavlink() {

  //MAVLINK DISTANCE MESSAGE
  int sysid = 1;
  //< The component sending the message.
  int compid = 196;
  uint32_t time_boot_ms = 0;                   /*< Time since system boot*/
  uint16_t min_distance = 5;                   /*< Minimum distance the sensor can measure in centimeters*/
  uint16_t max_distance = 600;                 /*< Maximum distance the sensor can measure in centimeters*/
  uint16_t current_distance = (Distance / 10); /*< Current distance reading*/
  uint8_t type = 0;                            /*< Type from MAV_DISTANCE_SENSOR enum.*/
  uint8_t id = 1;                              /*< Onboard ID of the sensor*/
  uint8_t orientation = 25;                    /*(0=forward, each increment is 45degrees more in clockwise direction), 24 (upwards) or 25 (downwards)*/
  uint8_t covariance = 0;                      /*< Measurement covariance in centimeters, 0 for unknown / invalid readings*/
  float horizontal_fov = 0;
  float vertical_fov = 0;
  uint8_t signal_quality = 0;
  const float quaternion = 0;

  // Initialize the required buffers
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Pack the message
  mavlink_msg_distance_sensor_pack(sysid, compid, &msg, time_boot_ms, min_distance, max_distance, current_distance, type, id, orientation, covariance, horizontal_fov, vertical_fov, signal_quality, quaternion);
  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  // Send the message (.write sends as bytes)
  //delay(1);
  Serial.write(buf, len);
}
