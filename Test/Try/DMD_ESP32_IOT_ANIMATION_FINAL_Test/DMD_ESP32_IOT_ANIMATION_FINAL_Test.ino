#include <DMD32.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include <WebServer.h>
#include <EEPROM.h>
//#include <stdio.h>
//#include <string.h>

/*              Fonts                */
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

const char* Data = "";
const char* animation = "";
const char* positionx = "";
const char* positiony = "";
const char* graphics = "";
const char* fontm = "";

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

int p = 0;
int q = 0;

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
  //wifiEEPROM();
  //connect_gsm();
  connectAWS();
  hardwareTimer();
  dmd.clearScreen( true );
  Data = "123Welcome123";
  int x = 1;
  int y = 5;

  int p = 0;
  int q = 32;
  g = GRAPHICS_NORMAL;
  //  fn = SystemFont5x7;
  //sendData("AT+CGMM", 3000, DEBUG);
  delay(200);
 // String Dat = EEPROM.readString(101);
 // Dat.toCharArray(Data,sizeof(Data));
}

void loop(void)
{
  client.loop();
  fontmode();
  graphicsmode();
  switchcommands();
  client.loop();
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
      break;
    case 2:
      dmd.selectFont(DejaVuSans9);
      p = 0;
      q = 9;
      break;
    case 3:
      dmd.selectFont(DejaVuSansBold9);
      p = 0;
      q = 9;
      break;
    case 4:
      dmd.selectFont(Arial_Black_16_ISO_8859_1);
      p = 0;
      q = 12;

      break;
    case 5:
      dmd.selectFont(Arial_Black_16);
      p = 0;
      q = 14;

    case 6:
      dmd.selectFont(BodoniMTBlack24);
      p = 0;
      q = 17;
      break;
    case 7:
      dmd.selectFont(Droid_Sans_24);
      p = 0;
      q = 19;
      break;
    case 8:
      dmd.selectFont(Comic24);
      p = 0;
      q = 22;

      break;
    case 9:
      dmd.selectFont(BigNumber);
      p = 0;
      q = 14;
      break;
    case 10:
      dmd.selectFont(Arial_38b);
      p = 0;
      q = 28;
      break;
    case 11:
      dmd.selectFont(Arial_14);
      p = 0;
      q = 11;
      break;
    case 12:
      dmd.selectFont(Arial_Black21);
      p = 0;
      q = 20;
      break;
    case 13:
      dmd.selectFont(Tahoma_32);
      p = 0;
      q = 9;
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
  //aws payload >>
  int ani = atoi(animation);
  int x = atoi(positionx);
  int y = atoi(positiony);
  int spd = 35;

  if (ani == 1)
  {
    dmd.clearScreen( true );
    fontmode();
    //dmd.selectFont(fn);
    
    dmd.drawString(x, y, Data, strlen(Data), g );
  }
  else if (ani == 2)
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
  else if (ani == 3)
  {
    dmd.clearScreen( true );
    fontmode();
    //dmd.selectFont(fn);
    dmd.drawString(x, y, Data, strlen(Data), g );
    dmd.drawBox(  0,  0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, g );
    delay( 2000 );
  }
  else if (ani == 4)
  {
    dmd.clearScreen( true );
    for (int a = 0 - (strlen(Data) * 6); a < 128; a++) {
      fontmode();
      //  dmd.selectFont( fn );
      dmd.drawString(a, y, Data, strlen(Data), g);
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

  Data = doc["message"];
  Serial.println(Data);
  
  EEPROM.writeString(101, Data);
  EEPROM.commit();
  
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



  //char json[] = {"message"};

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

  //   sendData("AT+CGMM", 3000, DEBUG);
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
