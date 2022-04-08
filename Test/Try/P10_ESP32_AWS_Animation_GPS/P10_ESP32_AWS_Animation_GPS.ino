#include <DMD32.h>        
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#include <stdio.h>
#include <string.h>

#include "fonts/SystemFont5x7.h"
#include "fonts/Arial_black_16.h"
#include "fonts/Arial_38b.h"
#include "fonts/Arial_Black_16_ISO_8859_1.h"
#include "fonts/Arial_Black21.h"
#include "fonts/Arial14.h"
#include "fonts/BodoniMTBlack24.h"
#include "fonts/Comic24.h"
#include "fonts/Droid_Sans_24.h"
#include "fonts/Tahoma_32.h"
//new fonts added................//
#include "fonts/BigNumber.h"
#include "fonts/DejaVuSans9.h"
#include "fonts/DejaVuSansBold9.h"

#include "Secrets.h"
#include "declarations.h"

String from_usb = "";

const char* Data = "";
const char* Border = "";

const char* command ="";
const char* f = "";
const char* animation ="";

DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
   //Timer setup
  //create a hardware timer  of ESP32
  hw_timer_t * timer = NULL;

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void IRAM_ATTR triggerScan()
{
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
void setup(void)
{

// return the clock speed of the CPU
Serial.begin(UART_BAUD);
delay(100);
Serial1.begin(UART_BAUD, SERIAL_8N1, MODEM_RXD, MODEM_TXD);
connect_gsm();
connectAWS();
  uint8_t cpuClock = ESP.getCpuFreqMHz();
  
  // Use 1st timer of 4 
  // devide cpu clock speed on its speed value by MHz to get 1us for each signal  of the timer
  timer = timerBegin(0, cpuClock, true);
  // Attach triggerScan function to our timer 
  timerAttachInterrupt(timer, &triggerScan, true);
  // Set alarm to call triggerScan function  
  // Repeat the alarm (third parameter) 
  timerAlarmWrite(timer, 300, true);

  // Start an alarm 
  timerAlarmEnable(timer);

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
   Data = "QuroLabs";
   Border = "```````````````````````````````````````";
   command = "f1";
   animation = "a1";
sendData("AT", 3000, DEBUG);
delay(200);
/*
 sendData("AT+CVAUXV=3050",3000, DEBUG);
delay(200);
sendData("AT+CVAUXS=1", 3000, DEBUG);
delay(200);
sendData("AT+CGPS=1", 3000, DEBUG);
delay(200);
sendData("AT+CVAUXS=0", 3000, DEBUG);
delay(200);

sendData("AT+CGPS=1,1", 3000, DEBUG);
delay(200);
//sendData("AT+CGPS=0", 3000, DEBUG);
//delay(200);
//sendData("AT+CGPSDEL", 3000, DEBUG);
//delay(200);
sendData("AT+CGPSHOR=50", 3000, DEBUG);
delay(200);
sendData("AT+CGPSAUTO=1", 3000, DEBUG);
delay(200);
sendData("AT+CGPSINFO", 3000, DEBUG);
delay(200);
*/
//sendData("AT+CGPS=1", 5000, DEBUG);
//delay(2000);
}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
void loop(void)
{

    usb_serial();            //Function created 
    //sendData("AT+CGPSINFO", 1000, DEBUG);
//    dmd.selectFont(SystemFont5x7);
//    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NORMAL );
//    delay(200);

/*
  client.loop();
   if(command == "f1"){
    dmd.clearScreen( true );
    //f = "SystemFont5x7";
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NORMAL );
    delay(2000);
 }
 else if(command == "f2"){
   // dmd.clearScreen( true );
   // f = "Arial_black_16";
    dmd.selectFont(Arial_Black_16);
    dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
    delay(2000);
 }
 else if(command == "f3"){
   // dmd.clearScreen( true );
   // f = "Arial_38b";
    dmd.selectFont(Arial_38b);
    //dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
    delay(2000);
 }
 else if(command == "f4"){
   // dmd.clearScreen( true );
   // f = "Arial_Black_16_ISO_8859_1";
    dmd.selectFont(Arial_Black_16_ISO_8859_1);
   // dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
   // delay(2000);  
 }
 else if(command == "f5"){
  //  dmd.clearScreen( true );
  //  f = "Arial_Black21";
    dmd.selectFont(Arial_Black21);
  //  dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
   // delay(2000);  
 }
 else if(command == "f6"){
  //  dmd.clearScreen( true );
  //  f = "Arial14";
    dmd.selectFont(Arial_14);
   // dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
  //  delay(2000); 
 }
 else if(command == "f7"){
 //   dmd.clearScreen( true );
  //  f = "BodoniMTBlack24";
    dmd.selectFont(BodoniMTBlack24);
  //  dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
  //  delay(2000); 
 }
 else if(command == "f8"){
  //  dmd.clearScreen( true );
 //   f = "Comic24";
    dmd.selectFont(Comic24);
   // dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
  //  delay(2000); 
 }
 else if(command == "f10"){
  //  dmd.clearScreen( true );
  //  f = "Droid_Sans_24";
    dmd.selectFont(Droid_Sans_24);
  //  dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
  //  delay(2000);  
 }
 else if(command == "f11"){
  //  dmd.clearScreen( true );
  //  f = "Tahoma_32";
    dmd.selectFont(Tahoma_32);
  //  dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
   // delay(2000);  
 }
 else if(command == "f12"){
   // dmd.clearScreen( true );
  //  f = "1234567aA a";
    dmd.selectFont(BigNumber);
 //   dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
  //  delay(2000);
 }
 else if(command == "f13"){
   // dmd.clearScreen( true );
  //  f = "DejaVuSans9";
    dmd.selectFont(DejaVuSans9);
  //  dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
   // delay(2000); 
 }
 else if(command == "f14"){
  //  dmd.clearScreen( true );
  //  f = "DejaVuSansBold9";
    dmd.selectFont(DejaVuSansBold9);
 //   dmd.drawString(1, 5, f, strlen(f), GRAPHICS_NORMAL );
  //  delay(2000); 
 }
 else if(command == "f15"){
    dmd.clearScreen( true );
 }
 else if(animation == "a1"){
    dmd.clearScreen( true );
   // commands();
    dmd.drawBox( 0, 0, (32*DISPLAYS_ACROSS)-1, (16*DISPLAYS_DOWN)-1, GRAPHICS_NORMAL );
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NORMAL );
  }
  else if(animation == "a2"){
   dmd.clearScreen( true );
  // commands(); 
   dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NORMAL );   
  }
//  else if(animation == "a3"){
   dmd.clearScreen( true );
  // commands();
   dmd.drawMarquee(Data,strlen(Data),(32*DISPLAYS_ACROSS)-1,0);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret){
     if ((timer+30) < millis()) {
       ret = dmd.stepSplitMarquee(0,7); // parameters are the top & bottom rows to be scrolled
       timer=millis();
        client.loop();
     }
   }

  if(animation == "a1"){
    dmd.clearScreen( true );
   // commands();
    dmd.drawBox( 0, 0, (32*DISPLAYS_ACROSS)-1, (16*DISPLAYS_DOWN)-1, GRAPHICS_NORMAL );
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NORMAL );
  }
  else if(animation == "a2"){
   dmd.clearScreen( true );
  // commands(); 
   dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NORMAL );   
  }
  else if(animation == "a3"){
   dmd.clearScreen( true );
  // commands();
   dmd.drawMarquee(Data,strlen(Data),(32*DISPLAYS_ACROSS)-1,0);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret){
     if ((timer+30) < millis()) {
       ret = dmd.stepSplitMarquee(0,7); // parameters are the top & bottom rows to be scrolled
       timer=millis();
        client.loop();
     }
   }
    
  }
*/
//commands();
client.loop();

 
    dmd.selectFont(Arial_Black_16);
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NORMAL );
    delay(2000);
    dmd.clearScreen( true );
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_INVERSE);
    delay(2000);
    dmd.clearScreen( true );
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_TOGGLE);
    delay(2000);
    dmd.clearScreen( true );
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_OR);
    delay(2000);
    dmd.clearScreen( true );
    dmd.drawString(1, 5, Data, strlen(Data), GRAPHICS_NOR);
    delay(2000);
    
    dmd.clearScreen( true );
    f = "drawMarquee";
    dmd.selectFont(SystemFont5x7);
    dmd.drawMarquee(f,strlen(f),(32*DISPLAYS_ACROSS)-1,0);
/*
   dmd.clearScreen( true );
   dmd.drawBox(  0,  0, (32*DISPLAYS_ACROSS)-1, (16*DISPLAYS_DOWN)-1, GRAPHICS_NORMAL );
   delay( 1000 );
   dmd.selectFont(Arial_Black_16);
   dmd.drawString(2, 8, "Welcome to QURO LABS", 20, GRAPHICS_NORMAL );
   delay(5000);
   dmd.clearScreen( true );
   dmd.selectFont(System5x7);

   for(byte x=0;x<DISPLAYS_ACROSS;x++){
    byte y = 1;
    dmd.drawString(  2+(32*x),  1+(16*y), "Quro", 4, GRAPHICS_NORMAL );   
   }

   //dmd.drawString(  2+(32*4), 1+(32*4), "Labs", 4, GRAPHICS_NORMAL );
   delay(1000);
   
   dmd.drawMarquee(Data,strlen(Data),(32*DISPLAYS_ACROSS)-1,0);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret){
     if ((timer+30) < millis()) {
       ret = dmd.stepSplitMarquee(0,7); // parameters are the top & bottom rows to be scrolled
       timer=millis();
        client.loop();
     }
   }
   delay(200);
   dmd.clearScreen( true );

  unsigned long time;
  int n;
//  boolean ret = false;

  // Show some stationary text on the bottom half, and scrolling text on the top half
  // scrolls 3 times
  dmd.drawString(0,8,"Beauto Systems",11,GRAPHICS_NORMAL); // the stationary string
  dmd.drawMarquee(Data,strlen(Data),(32*DISPLAYS_ACROSS)-0,16);
  time = millis();
  n=0;
  while(n<3) {
    while (!ret) {
      if ((time+30) < millis()) {
        ret = dmd.stepSplitMarquee(16,32); // parameters are the top & bottom rows to be scrolled
        time = millis();
      }
    }
    ret = false;
    n++;
  }
  delay(1000);        //added
  
 dmd.clearScreen( true );


  // Now some stationary text on the top half, and scrolling text on the bottom half
  // scrolls for 10 seconds
  dmd.drawString(0,0,"QURO_LABS",11,GRAPHICS_NORMAL); // stationary text
  dmd.drawMarquee("BEAUTO_SYSTEMS",14,0,8); // set up the marquee
  time = millis();
  while ((millis() - time)<10000){ // loop for 10 seconds
    dmd.stepSplitMarquee(8,15); // only scroll rows 8 to 15
    delay(30);
  } // a slightly different way to loop for stepping the marquee
    // note that this does not test for completion of the scroll, but continues until
    // the 10 second time has expired
  dmd.clearScreen( true );
  
  // Now a bit of fun
  dmd.drawString(0,1,Border,strlen(Border),GRAPHICS_NORMAL); // note the position is above a single DMD so
                                                         // only part of the text will be visible
  dmd.drawString(0,20,Border,strlen(Border),GRAPHICS_NORMAL); // and this is too far down a single DMD so
                                                         // only part will be visible
  // these 2 lines above use partial characters displayed on the screen by placing the text at non-standard positions
  // to give a graphical highlight effect.
  dmd.drawMarquee("Scrolling text",14,0,5);
  time = millis();
  while ((millis() - time)<10000){ // again we will scroll for 10 seconds
    dmd.stepSplitMarquee(5,11); 
    delay(30);
  }
  dmd.clearScreen( true );
*/  

}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  
   Data = doc["message"];
  Serial.println(Data);

  command = doc["command"];
  Serial.println(command);

  animation = doc["animation"];
  Serial.println(animation);
  commands();
}

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME) && retry < 20)
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

/*--------------------------------------------------------------------------------------
  Show clock numerals on the screen from a 4 digit time value, and select whether the
  flashing colon is on or off
--------------------------------------------------------------------------------------*/

void ShowClockNumbers( unsigned int uiTime, byte bColonOn )
{
   dmd.clearScreen(true);
   dmd.drawChar(  1,  3,'0'+((uiTime%10000)/1000), GRAPHICS_NORMAL );   // thousands
   dmd.drawChar(  8,  3, '0'+((uiTime%1000) /100),  GRAPHICS_NORMAL );   // hundreds
   dmd.drawChar( 17,  3, '0'+((uiTime%100)  /10),   GRAPHICS_NORMAL );   // tens
   dmd.drawChar( 25,  3, '0'+ (uiTime%10),          GRAPHICS_NORMAL );   // units
   if( bColonOn )
      dmd.drawChar( 15,  3, ':', GRAPHICS_OR     );   // clock colon overlay on
   else
      dmd.drawChar( 15,  3, ':', GRAPHICS_NOR    );   // clock colon overlay off
}

/*     custom functions       */

void commands()
{

}

void fontSelection()
{
  String SelectFont;
  switch('SelectFont'){
  case 'FONT_ONE':
    dmd.clearScreen( true );
    delay(100);
    dmd.selectFont(System5x7);
    break;
  case 'FONT_TWO':
    dmd.clearScreen( true );
    delay(100);
    dmd.selectFont(System5x7);
  case 'FONT_THREE':
    dmd.clearScreen( true );
    delay(100);
    dmd.selectFont(System5x7);
  case 'FONT_FOUR':
     dmd.clearScreen( true );
    delay(100);
    dmd.selectFont(System5x7);
  case 'FONT_FIVE':
    dmd.clearScreen( true );
    delay(100);
    dmd.selectFont(System5x7);
  default:
    dmd.clearScreen( true );
    delay(100);
    dmd.selectFont(System5x7);
  } 
}

void connect_gsm()
{
  
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
 
    Serial.println("P10 4G Test Start!");
 
 //   sendData("AT+CGMM", 3000, DEBUG);
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
