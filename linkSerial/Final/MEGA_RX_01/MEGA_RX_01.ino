//  "message": "Hello from AWS IoT console","graphics":"1","animation":"2","positionx":"3","positiony":"4","fontm":"5"
#include <ArduinoJson.h>
//#include <SoftwareSerial.h>

//SoftwareSerial linkSerial(10, 11); // RX, TX
#define MODEM_RXD 19
#define MODEM_TXD 18
#define UART_BAUD 4800

const char* message = "";
const char* animation = "";
const char* positionx = "";
const char* positiony = "";
const char* graphics = "";
const char* fontm = "";

void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  Serial1.begin(UART_BAUD);
}
 
void loop() {
LinkSerial();


}

void LinkSerial()
{
    if (Serial1.available()) 
  {

    StaticJsonDocument<300> doc;

    DeserializationError err = deserializeJson(doc, Serial1);

    if (err == DeserializationError::Ok) 
    {
      
      message = doc["message"];
      Serial.println(message);

      graphics = doc["graphics"];
      Serial.println(graphics);

      animation = doc["animation"];
      Serial.println(animation);

      positionx = doc["positionx"];
      Serial.println(positionx);

      positiony = doc["positiony"];
      Serial.println(positiony);

      fontm = doc["fontm"];
      Serial.println(fontm);
      
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial1.available() > 0)
        Serial1.read();
    }
  }
  
}
