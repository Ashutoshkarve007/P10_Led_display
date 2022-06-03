#include <stdio.h>
#include <string.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <SoftwareSerial.h>

#define ESP_TX 12
#define ESP_RX 13

#define MODEM_RXD 16
#define MODEM_TXD 17

#define UART_BAUD 115200

#define DEBUG true
#define MODE_1A

String from_usb = "";

const char* message = "";
const char* animation = "";
const char* positionx = "";
const char* positiony = "";
const char* graphics = "";
const char* fontm = "";

SoftwareSerial espPort;
void setup()
{
  Serial.begin(115200);
  Serial1.begin(UART_BAUD, SERIAL_8N1, MODEM_RXD, MODEM_TXD);
  espPort.begin(4800, SWSERIAL_8N1, ESP_RX, ESP_TX);

  delay(20000);             // Connecting GSM
  connect_mqtt();
}

void loop()
{
  gsm_response();
  //usb_serial();
}

void usb_serial()
{
  while (Serial1.available() > 0)
  {
    Serial.write(Serial1.read());

    yield();
  }
  while (Serial.available() > 0)
  {
#ifdef MODE_1A
    int c = -1;
    c = Serial.read();
    if (c != '\n' && c != '\r')
    {
      from_usb += (char)c;
    }
    else
    {
      if (!from_usb.equals(""))
      {
        sendData(from_usb, 0, DEBUG);
        from_usb = "";
      }
    }
#else
    Serial1.write(Serial.read());
    //  Serial.print(Serial1.read());
    yield();
#endif
  }
}

void gsm_response()
{
  while (Serial1.available() > 0)
  {
    delay(10);
    String a = Serial1.readString();

    if (a.indexOf("PAYLOAD") != -1)
    {
      int new1 = a.indexOf("PAYLOAD");
      String neww = a.substring(new1);
      int new2 = neww.indexOf('\n');
      String new3 = neww.substring(new2 + 1);

      // Serial.print(new3);
      StaticJsonDocument<200> doc;

      deserializeJson(doc, new3);


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

      serializeJson(doc, espPort);

    }
  }
}

void connect_mqtt()
{
  sendData("AT+CSSLCFG=\"sslversion\",0,4", 1000, DEBUG);
  // delay(1000);
  sendData("AT+CSSLCFG=\"authmode\",0,2", 1000, DEBUG);
  // delay(1000);
  sendData("AT+CSSLCFG=\"cacert\",0,\"cacert.pem\"", 1000, DEBUG);
  //  delay(1000);
  sendData("AT+CSSLCFG=\"clientcert\",0,\"clientcert.pem\"", 1000, DEBUG);
  //  delay(1000);
  sendData("AT+CSSLCFG=\"clientkey\",0,\"clientkey.pem\"", 1000, DEBUG);
  //  delay(1000);
  sendData("AT+CPIN?", 1000, DEBUG); //
  delay(1000);
  sendData("AT+CIPMODE=0", 1000, DEBUG);
  delay(1000);
  sendData("AT+CIPMODE", 1000, DEBUG);
  delay(1000);
  sendData("AT+NETOPEN", 1000, DEBUG); //
  delay(1000);
  sendData("AT+CGACT=1", 1000, DEBUG); //
  delay(1000);
  sendData("AT+CGACT?", 1000, DEBUG); //
  delay(1000);
  sendData("AT+IPADDR", 1000, DEBUG); //
  delay(1000);

  sendData("AT+CMQTTSTART", 1000, DEBUG); //
  delay(1000);
  sendData("AT+CMQTTACCQ=0,\"SIMCom_client01\",1", 1500, DEBUG); //
  delay(1000);
  sendData("AT+CMQTTSSLCFG=0,0", 1000, DEBUG);
  delay(1000);
  Serial1.println("AT+CMQTTCONNECT=0,\"tcp://a1z3mi6l817ak1-ats.iot.ap-south-1.amazonaws.com:8883\",60,1");

  delay(5000);

  sendData("AT+CMQTTSUBTOPIC=0,3,1", 2000, DEBUG);
  delay(1000);
  sendData("aws", 1000, DEBUG);
  delay(1000);
  sendData("AT+CMQTTSUB=0", 1000, DEBUG);
  delay(1000);
  sendData("AT+CMQTTTOPIC=0,3", 1000, DEBUG);
  delay(1000);
  sendData("aws", 1000, DEBUG);
  delay(1000);
  sendData("AT+CMQTTPAYLOAD=0,38", 1000, DEBUG);
  delay(1000);
  sendData("{\"message\":\"Hello from SIMCom Module\"}", 1000, DEBUG);
  delay(1000);
  sendData("AT+CMQTTPUB=0,1,60", 1000, DEBUG);

}

String sendData(String command, const int timeout, boolean debug)
{

  String response = "";
  if (command.equals("1A") || command.equals("1a"))
  {
    Serial.println();
    Serial.println("Get a 1A, input a 0x1A");

    //Serial1.write(0x1A);
    Serial1.write(26);
    Serial1.println();
    return "";
  }
  else
  {
    Serial1.println(command);
  }

  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
