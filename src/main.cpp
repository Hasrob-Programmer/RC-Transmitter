#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RFMessage.h>

RF24 radio(8, 9); // CE, CSN

const int x_pin = A0;
const int y_pin = A1;

const byte address[6] = "00001";

long last_send_data_ms, last_read_analog_ms;
int send_data_ms, read_analog_ms;

RFMessage rf_msg;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  pinMode(x_pin, INPUT);
  pinMode(y_pin, INPUT);

  if (!radio.begin()) {
     Serial.println(F("radio hardware not responding!"));
     while (1) {} 
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  send_data_ms = 100;
  read_analog_ms = 5;

  last_send_data_ms = millis();
  last_read_analog_ms = last_send_data_ms;

  rf_msg.x_axis = 0;
  rf_msg.y_axis = 0;
}

void loop() {
  long now_ms = millis();
  
  if(now_ms - last_send_data_ms > send_data_ms)
  {
    const unsigned char* text = rf_msg.getBuffer();
    bool status = radio.write(text, RFMESSAGE_LEN);
  }
  if(now_ms - last_read_analog_ms > read_analog_ms)
  {
    rf_msg.x_axis = analogRead(x_pin);
    rf_msg.y_axis = analogRead(y_pin);
  }
  Serial.print("\n\n");
  Serial.print("X : ");
  Serial.println(rf_msg.x_axis);
  Serial.print("\n\n");
  Serial.print("Y : ");
  Serial.println(rf_msg.y_axis);
}