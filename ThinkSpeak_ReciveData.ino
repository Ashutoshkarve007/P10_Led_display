#include <stdio.h>
#include <string.h>
//#include <DHT.h>
 
//Change the API key to yours
String Apikey = "YX7HQBE3UWSFWRTT";
 
#define DEBUG true
#define LTE_RESET_PIN 33
#define LTE_PWRKEY_PIN 26
#define LTE_FLIGHT_PIN 27
//#define Sensor_PIN 3  //D3-DHT11
 
//DHT  dht(Sensor_PIN,DHT11);
#define MODEM_RXD 16
#define MODEM_TXD 17

#define UART_BAUD 115200
void setup()
{
    Serial.begin(115200);
    //while (!SerialUSB)
    {
      ; // wait for Arduino serial Monitor port to connect
    }
    //Serial1.begin(115200);
  
    Serial1.begin(UART_BAUD, SERIAL_8N1, MODEM_RXD, MODEM_TXD);
  
    pinMode(LTE_RESET_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
  
    pinMode(LTE_PWRKEY_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LTE_PWRKEY_PIN, HIGH);
    delay(2000);
    digitalWrite(LTE_PWRKEY_PIN, LOW);
    
    pinMode(LTE_FLIGHT_PIN, OUTPUT);
    digitalWrite(LTE_FLIGHT_PIN, LOW);//Normal Mode
    
    delay(5000);
 
    
    /*ModuleState = moduleStateCheck();
    if (ModuleState == false) //if it's off, turn on it.
    {
        digitalWrite(PWR_KEY, LOW);
        delay(3000);
        digitalWrite(PWR_KEY, HIGH);
        delay(10000);
        SerialUSB.println("Now turnning the SIM7600 on.");
    }*/
    
 
    sendData("AT+CCID", 3000, DEBUG);
    sendData("AT+CREG?", 3000, DEBUG);
    sendData("AT+CGATT=1", 1000, DEBUG);
    sendData("AT+CGACT=1,1", 1000, DEBUG);
    sendData("AT+CGDCONT=1,\"IP\",\"jionet\"", 1000, DEBUG);
 
    //sendData("AT+CIPSTART=\"TCP\",\"www.mirocast.com\",80", 2000, DEBUG);
    Serial.println("4G HTTP Test Begin!");
 
//    dht.begin();
    delay(1000);
}
 
void loop()
{
     float t= random(10,30);
     float h= random(10, 20);
     Serial.print("Humidity: ");
     Serial.print(h);
     Serial.println("%");
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.println("*C");
     delay(1000);
 
    //-----------HTTP---------------------
    String http_str = "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=" + Apikey + "&field1=" + (String)t + "&field2=" + (String)h + "\"\r\n";
    Serial.println(http_str);
 
    sendData("AT+HTTPINIT\r\n", 2000, DEBUG);
    sendData(http_str, 2000, DEBUG);
    sendData("AT+HTTPACTION=0\r\n", 3000, DEBUG);
    sendData("AT+HTTPTERM\r\n", 3000, DEBUG);
 
    delay(5000);   
}
 
bool moduleStateCheck()
{
    int i = 0;
    bool moduleState = false;
    for (i = 0; i < 5; i++)
    {
        String msg = String("");
        msg = sendData("AT", 1000, DEBUG);
        if (msg.indexOf("OK") >= 0)
        {
            Serial.println("SIM7600 Module had turned on.");
            moduleState = true;
        }
        delay(1000);
    }
    return moduleState;
}
 
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  Serial1.println(command);
  
  long int time = millis();
  while ( (time + timeout) > millis())
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
