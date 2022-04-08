#include <DMD32.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include <WebServer.h>
#include <EEPROM.h>

/****************HeaderFiles******************/
#include "Secrets.h"
#include "declarations.h"

/******************Fonts*********************/
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

/***********New Fonts Added******************/
#include "fonts/BigNumber.h"
#include "fonts/DejaVuSans9.h"
#include "fonts/DejaVuSansBold9.h"


/**************Data From Aws******************
          Check Message Handler
*********************************************/
const char* Data = "";
//String f = String(Data);
const char* animation = "";
const char* positionx = "";
const char* positiony = "";
const char* graphics = "";
const char* fontm = "";

/********************************************/
char simData, temp;
String message;
String from_usb = "";

int i = 0;
int statusCode;
const char* ssid = "Default SSID";
const char* passphrase = "Default passord";
String st;
String content;
String esid;
String epass = "";

byte g;

/**************DMD Default*********************/

int b;
int x;
int y;
int p;
int q;

int fm;
int grap;
int ani;

/*****************GPS-Data*********************/
String gpsHour = "";
String gpsMinutes = "";
String gpsSeconds = "";
String gpsStatus = "";
String gpsLat = "";
String gpsLong = "";
String gpsSpeed = "";
String gpsMagVariation = "";
String latDirection = "";
String longDirection = "";


DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
WebServer server(80);
hw_timer_t * timer = NULL;

void IRAM_ATTR triggerScan()
{
  dmd.scanDisplayBySPI();
}

void setup(void)
{
  Serial.begin(UART_BAUD);
  delay(100);
  Serial1.begin(UART_BAUD, SERIAL_8N1, MODEM_RXD, MODEM_TXD);
  EEPROM.begin(500);
  //wifiEEPROM();
  connect_gsm();
  connectAWS();
  hardwareTimer();
  dmd.clearScreen( true );
  // Serial.print(EEPROM.readString(101));

  g = GRAPHICS_NORMAL;


  int b = 0;
  int x = 1;
  int y = 5;
  int p = 0;
  int q = 32;

  int fm = 1;
  int grap = 1;
  int ani = 1;
  Data = "123hello123";

  delay(200);
  // String Dat = EEPROM.readString(101);
  // Dat.toCharArray(Data,sizeof(Data));
  //simgps();         // added
  delay(2000);

  // xTaskCreate(taskgps, " taskShedule", 5000, NULL, 1, NULL);
}

void loop(void)
{

  client.loop();
  usb_serial();
  //gpsParser();
  //simgps();
  fontmode();
  graphicsmode();
  switchcommands();
  // client.loop();
}

void gpsParser()
{
  String s = Serial1.readStringUntil('\n');
  if (s.startsWith("$GPRMC")) {
    int posHeader = s.indexOf("$GPRMC");
    if (posHeader == 0) {
      //Serial.println(s);
      int posHour = s.indexOf(',');       // Get first occurance of comma ","
      //Serial.print(" First Comma: ");
      // Serial.println(posHour);

      gpsHour = s.substring(posHour + 1, posHour + 3); // Get GPS Hours from String
      //  Serial.print("GPS Hour: ");
      //Serial.println(gpsHour.toInt());

      gpsMinutes = s.substring(9, 11);
      //Serial.print("GPS Minutes: ");
      //Serial.println(gpsMinutes);

      gpsSeconds = s.substring(11, 13);
      //Serial.print("GPS Seconds: ");
      //Serial.println(gpsSeconds);

      gpsStatus = s.substring(17, 18);
      //Serial.print("GPS Status: ");
      //Serial.println(gpsStatus);

      gpsLat = s.substring(19, 29);
      //     Serial.print("GPS Latitute: ");
      //   Serial.println(gpsLat);

      latDirection = s.substring(30, 31);
      //        Serial.print("Latitute Direction: ");
      //       Serial.println(latDirection);

      gpsLong = s.substring(33, 43);
      // Serial.print("GPS Longitude: ");
      //Serial.println(gpsLong);

      longDirection = s.substring(44, 45);
      // Serial.print("Longitude Direction: ");
      // Serial.println(longDirection);

      gpsSpeed = s.substring(46, 51);
      //   Serial.print("GPS Speed: ");
      //   Serial.println(gpsSpeed/*.toInt()*1.852*/);   // knot to kmh

      /*     gpsMagVariation = s.substring(60, 63);
           Serial.print("GPS gpsMagVariation: ");
           Serial.println(gpsMagVariation);   // degrees
      */

    }
  }
}

void simgps()
{
  sendData("AT+CGPS=?");
  delay(1000);
  sendData("AT+CGPS?");
  delay(1000);
  sendData("AT+CGPS=1");
  delay(1000);
  sendData("AT+CGPSINFO=?");
  delay(1000);
  sendData("AT+CGPSINFO?");
  delay(1000);
  sendData("AT+CGPSINFOCFG=?");
  delay(1000);
  sendData("AT+CGPSINFOCFG=5,31");
  delay(1000);
}

void taskgps(void * pvParameter) {
  while (1)
  {
    usb_serial();
    sendData("AT+CGPSINFO");
    delay(1000);
    //scheduleTask();
  }
}

void fontmode()
{
  int fm = atoi(fontm);
  switch (fm)
  {
    case 1:
      dmd.selectFont(SystemFont5x7);
      p = 0;
      q = 8;
      b = -q;
      break;

    case 2:
      dmd.selectFont(DejaVuSans9);
      p = 0;
      q = 9;
      b = -q;
      break;

    case 3:
      dmd.selectFont(DejaVuSansBold9);
      p = 0;
      q = 9;
      b = -q;
      break;

    case 4:
      dmd.selectFont(Arial_Black_16_ISO_8859_1);
      p = 0;
      q = 12;
      b = -q;
      break;

    case 5:
      dmd.selectFont(Arial_Black_16);
      p = 0;
      q = 14;
      b = -q;
      break;

    case 6:
      dmd.selectFont(BodoniMTBlack24);
      p = 0;
      q = 17;
      b = -q;
      break;

    case 7:
      dmd.selectFont(Droid_Sans_24);
      p = 0;
      q = 19;
      b = -q;
      break;

    case 8:
      dmd.selectFont(Comic24);
      p = 0;
      q = 22;
      b = -q;
      break;

    case 9:
      dmd.selectFont(BigNumber);
      p = 0;
      q = 14;
      b = -q;
      break;

    case 10:
      dmd.selectFont(Arial_38b);
      p = 0;
      q = 28;
      b = -q;
      break;

    case 11:
      dmd.selectFont(Arial_14);
      p = 0;
      q = 11;
      b = -q;
      break;

    case 12:
      dmd.selectFont(Arial_Black21);
      p = 0;
      q = 20;
      b = -q;
      break;

    case 13:
      dmd.selectFont(Tahoma_32);
      p = 0;
      q = 9;
      b = -q;
      break;

  }
}

void graphicsmode()
{
  int grap = atoi(graphics);
  switch (grap)
  {
    case 1:
      g = GRAPHICS_NORMAL;
      break;
    case 2:
      g = GRAPHICS_INVERSE;
      break;
    case 3:
      g = GRAPHICS_TOGGLE;
      break;
    case 4:
      g = GRAPHICS_OR;
      break;
    case 5:
      g = GRAPHICS_NOR;
      break;
  }
}

void switchcommands()
{
  //aws payload >> "Data":"Hello123","graphics":"1","animation":"9","positionx":"1","positiony":"3","fontm":"1"
  int ani = atoi(animation);
  int x = atoi(positionx);
  int y = atoi(positiony);
  int spd = 35;

  //String f = String(Data);
  // Serial.println("Data : " + f);
  // EEPROM.writeString(101, f);
  //  EEPROM.commit();

  if (ani == 1)                                                       //Normal Print String
  {
    dmd.clearScreen( true );
    fontmode();
    //dmd.selectFont(fn);

    dmd.drawString(x, y, Data, strlen(Data), g );
  }
  else if (ani == 2)                                                      //Scroll From right to left
  {
    fontmode();
    //dmd.selectFont( fn );
    delay(200);
    dmd.drawMarquee(Data, strlen(Data), (32 * DISPLAYS_ACROSS) - 1, y);
    long start = millis();
    long timer = start;
    boolean ret = false;
    while (!ret) {
      if ((timer + 30) < millis()) {
        ret = dmd.stepSplitMarquee(y + p, y + q); // parameters are the top & bottom rows to be scrolled
        timer = millis();
        //client.loop();
      }
    }
  }
  else if (ani == 3)                                                      //Scroll from left to right
  {
    dmd.clearScreen( true );
    // fontmode();
    for (int a = 0 - (strlen(Data) * 6); a < 129; a++) {
      fontmode();
      //  dmd.selectFont( fn );
      dmd.drawString(a, y, Data, strlen(Data), g);
      delay(spd);
    }
  }

  else if (ani == 4)                                                     //Scroll from top to bottom
  {
    dmd.clearScreen( true );
    for ( int a = b - 1 ; a < 33; a++)
    {
      dmd.clearScreen( true );
      fontmode();
      dmd.drawString(x, a, Data, strlen(Data), g);
      delay(40);
    }
  }
  else if (ani == 5)                                                    //Scroll from bottom to top
  {
    dmd.clearScreen( true );
    for (int a = 33 ; a > b - 1; a--)
    {
      dmd.clearScreen( true );
      dmd.drawString(x, a, Data, strlen(Data), g);
      delay(40);
    }
  }
  else if (ani == 6)                                                  // Box + string
  {
    dmd.clearScreen( true );
    fontmode();
    //dmd.selectFont(fn);
    dmd.drawBox(0,  0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, g );
    //delay(200);
    dmd.drawString(x, y, Data, strlen(Data), g );

    delay( 2000 );
  }
  else if (ani == 7)                                                  //  Box + scroll right to left
  {
    fontmode();
    //dmd.selectFont( fn );
    delay(200);
    dmd.drawBox(0,  0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, g );
    dmd.drawMarquee(Data, strlen(Data), (32 * DISPLAYS_ACROSS) - 1, y);
    long start = millis();
    long timer = start;
    boolean ret = false;
    while (!ret) {
      if ((timer + 30) < millis()) {
        ret = dmd.stepSplitMarquee(y + p, y + q); // parameters are the top & bottom rows to be scrolled
        timer = millis();
        //client.loop();
      }
    }
  }
  else if (ani == 8)                                                // Box + scroll left to Right
  {
    dmd.clearScreen( true );
    fontmode();
    dmd.drawBox(0,  0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, g );
    delay(200);
    for (int a = 0 - (strlen(Data) * 6); a < 128; a++)
    {
      fontmode();
      //  dmd.selectFont( fn );
      dmd.drawBox(0,  0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, g );
      dmd.drawString(a, y, Data, strlen(Data), g);
      delay(spd);
    }
  }
  else if (ani == 9)                                                // Box + Scroll Top to Bottom
  {
    dmd.clearScreen( true );
    dmd.drawBox(0, 0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, g );
    delay(200);
    for ( int a = b  ; b < 32; b++)
    {
      fontmode();
      dmd.drawString(x, a, Data, strlen(Data), g);
      delay(spd);
    }
  }
  else if (ani == 10)                                             // Box + Scroll Bottom to top
  {
    dmd.clearScreen( true );
    dmd.drawBox(0, 0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, g );
    delay(200);
    for (int a = 32 ; a > b - 1; a++)
    {
      dmd.drawString(x, a, Data, strlen(Data), g);
      delay(spd);
    }
  }
  delay(2000);
}

void wifiEEPROM()
{
  Serial.println("Disconnecting current wifi connection");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  Serial.println("Reading EEPROM ssid");
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  WiFi.begin(esid.c_str(), epass.c_str());
  int retry = 0;
  while ((WiFi.status() != WL_CONNECTED)  && retry < 15)
  {
    Serial.print("Connecting to : ");
    Serial.println(esid.c_str());
    delay(200);
    retry++;
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Localmode Started");
    setupAP();
    while (1)
    {
      server.handleClient();
    }
  }
  else
  {
    Serial.println("WIFI MODE");
    Serial.println();
    Serial.print("Connected to : ");
    Serial.println(WiFi.SSID());
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    // Configuring MQTT Client

    //connectAWS();
    delay(1000);
  }
}
void messageHandler(char* topic, byte * payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  Data = doc["Data"];
  Serial.println(Data);



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
  //  int retry = 0;
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
}
void hardwareTimer()
{
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
}
void usb_serial()
{
  while (Serial1.available() > 0)
  {
    String response = Serial1.readStringUntil('\n');
    if (response.startsWith("$GPRMC")) {
      int posHeader = response.indexOf("$GPRMC");
      if (posHeader == 0) {
        Serial.println("Received :" + response);
        int posHour = response.indexOf(',');       // Get first occurance of comma ",
        gpsLat = response.substring(19, 29);
        Serial.print("GPS Latitute: ");
        Serial.println(gpsLat);
        latDirection = response.substring(30, 31);
        Serial.print("Latitute Direction: ");
        Serial.println(latDirection);

        gpsLong = response.substring(33, 43);
        Serial.print("GPS Longitude: ");
        Serial.println(gpsLong);
      }
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
          sendData(from_usb);
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
}


String sendData(String command)
{
  String response = "";
  if (command.equals("1A") || command.equals("1a"))
  {
    Serial.println();
    Serial.println("Get a 1A, input a 0x1A");
    Serial1.write(26);
    Serial1.println();
    // return "";
  }
  else
  {
    Serial1.println(command);

  }
  while (Serial1.available())
  {
    response = Serial1.readString();
    Serial.println("Response : " + response);
  }
  return response;
}

void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      //Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    //st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("QuroLabs", "");
  Serial.println("Initializing_softap_for_wifi credentials_modification");
  launchWeb();
  Serial.println("over");
}

void createWebServer()
{
  {
    server.on("/", []() {

      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>Welcome to Wifi Credentials Update page";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.restart();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
  }
}
