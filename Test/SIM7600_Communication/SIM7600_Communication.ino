#include <stdio.h>
#include <string.h>

#define DEBUG true
#define MODE_1A
 
//#define DTR_PIN 9
//#define RI_PIN 8
 
#define LTE_PWRKEY_PIN 26
#define LTE_RESET_PIN 33
#define LTE_FLIGHT_PIN 27

#define MODEM_RXD 16
#define MODEM_TXD 17

#define UART_BAUD 115200
 
String from_usb = "";
 
void setup()
{
    Serial.begin(115200);
    //while (!SerialUSB)
    {
        ; // wait for Arduino serial Monitor port to connect
    }
    delay(100);
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
    digitalWrite(LTE_FLIGHT_PIN, LOW); //Normal Mode
    // digitalWrite(LTE_FLIGHT_PIN, HIGH);//Flight Mode
 
    Serial.println("Maduino Zero 4G Test Start!");
 
    sendData("AT+CGMM", 3000, DEBUG);
}
 
void loop()
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
            return moduleState;
        }
        delay(1000);
    }
    return moduleState;
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
