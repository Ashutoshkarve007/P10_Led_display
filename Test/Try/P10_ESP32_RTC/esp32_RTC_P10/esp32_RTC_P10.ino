#include <DMD32.h>
#include <stdio.h>
#include <string.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "RTClib.h"
#include "Secrets.h"
#include "declarations.h"

/*_______________DMD FONTS________________*/
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
/*_______________NEW FONTS________________*/
#include "fonts/BigNumber.h"
#include "fonts/DejaVuSans9.h"
#include "fonts/DejaVuSansBold9.h"

RTC_DS3231 rtc;

/*___________________STORAGES_______________*/
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String from_usb = "";

const char* Data = "";
const char* Border = "";

const char* command = "";
const char* f = "";
const char* animation = "";

const char* rt = "";
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

//Timer setup
hw_timer_t * timer = NULL;

void IRAM_ATTR triggerScan()
{
  dmd.scanDisplayBySPI();
}

void setup(void)
{
  Serial.begin(115200);
  delay(100);
  Serial1.begin(UART_BAUD, SERIAL_8N1, MODEM_RXD, MODEM_TXD);
  connect_gsm();
  //connectAWS();
  
  /*_____________DMD_TIMER_______________*/
  uint8_t cpuClock = ESP.getCpuFreqMHz();
  timer = timerBegin(0, cpuClock, true);
  timerAttachInterrupt(timer, &triggerScan, true);
  timerAlarmWrite(timer, 300, true);
  timerAlarmEnable(timer);

  /*______________RTC____________________*/
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  /*____________DEFAULT DISPLAY ON P10______________*/
  dmd.clearScreen( true );
  Data = "QuroLabs";
  Border = "```````````````````````````````````````";
  command = "f1";
  animation = "a1";
  
  /*______________SETTING_UP GPS________________*/
  sendData("AT", 3000, DEBUG);
  delay(200);
  sendData("AT+CGPS=1", 5000, DEBUG);
  delay(2000);
}

void loop(void)
{

  usb_serial();            //Function created
  //commands();
  rtc_esp();
  client.loop();

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
  dmd.drawChar(  1,  3, '0' + ((uiTime % 10000) / 1000), GRAPHICS_NORMAL ); // thousands
  dmd.drawChar(  8,  3, '0' + ((uiTime % 1000) / 100),  GRAPHICS_NORMAL ); // hundreds
  dmd.drawChar( 17,  3, '0' + ((uiTime % 100)  / 10),   GRAPHICS_NORMAL ); // tens
  dmd.drawChar( 25,  3, '0' + (uiTime % 10),          GRAPHICS_NORMAL ); // units
  if ( bColonOn )
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
  switch ('SelectFont') {
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

void rtc_esp()
{
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();

    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.println(" C");

    Serial.println();
    delay(3000);
}

void rtc_dmd()
{
  dmd.clearScreen( true );
  DateTime now = rtc.now();
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(  0,  3, rt, strlen(rt) , GRAPHICS_NORMAL );
 // rt = Serial.print(now.second(), DEC);
}
