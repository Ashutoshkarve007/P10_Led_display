//  "message": "Hello from AWS IoT console","graphics":"1","animation":"2","positionx":"3","positiony":"4","fontm":"5"
// default_pins(pin_noe == 9 && pin_a == 6 && pin_b == 7 && pin_sck == 8), pin_clk(13), pin_r_data(11)
//not Working
#include <ArduinoJson.h>
#include <DMD2.h>

#define MODEM_RXD 19
#define MODEM_TXD 18

#define UART_BAUD 4800

#define DISPLAYS_ACROSS 4
#define DISPLAYS_DOWN 2

#include <fonts/Arial_Black_16.h>
#include <fonts/SystemFont5x7.h>

const char* message = "";
const char* animation = "";
const char* positionx = "";
const char* positiony = "";
const char* brightness = "";
const char* fontm = "";

const char *MESSAGE = "abcdefghijklmnopqrstuvwxyz";

SoftDMD dmd(4, 2);
// Number of P10 panels used X, Y
DMD_TextBox box(dmd, 0, 0, 32, 16);
// Set Box (dmd, x, y, Height, Width)


void setup() {
  Serial.begin(115200);
  Serial1.begin(UART_BAUD);
  dmd.setBrightness(100); // Set brightness 0 - 255
  dmd.selectFont(Arial_Black_16); // Font used
  dmd.begin();     // Start DMD


}

void loop() {
  LinkSerial();
  switchCommands();
  /*
    dmd.clearScreen();
    dmd.drawString(5, 5, ("setPixel"));
    delay(2000);
    dmd.clearScreen();
    dmd.setPixel(5, 5, GRAPHICS_ON);
    delay(2000);
    dmd.clearScreen();
    dmd.drawString(0, 0, ("DrawLine"));
    delay(2000);
    dmd.clearScreen();
    dmd.drawLine(0, 2, 0, 30, GRAPHICS_ON);
    delay(2000);
    dmd.clearScreen();
    dmd.drawString(0, 0, ("DrawCircle"));
    delay(2000);
    dmd.clearScreen();
    dmd.drawCircle(5, 5, 4, GRAPHICS_ON);
    delay(2000);
    dmd.clearScreen();
    dmd.drawString(0, 0, ("BOX"));
    delay(2000);
    dmd.clearScreen();
    dmd.drawBox(5, 5, 10, 10, GRAPHICS_ON);
    delay(2000);
    dmd.clearScreen();
    dmd.drawFilledBox(10, 10, 20, 20, GRAPHICS_ON);
    delay(2000);
    //dmd.drawTestPattern(PATTERN_ALT_1);
    delay(2000);
  */

  // dmd.drawString(5, 10, "Hello", GRAPHICS_ON);


}
void switchCommands()
{
  int ani = atoi(animation);
  int x = atoi(positionx);
  int y = atoi(positiony);
  int z = strlen(message) * 11;
  int bri = atoi(brightness);

  if (ani == 1)
  {
    dmd.clearScreen();
    dmd.setBrightness(bri);
    dmd.drawString(x, y, message, GRAPHICS_ON);
    delay(100);
    dmd.setBrightness(bri);
  }
  else if (ani == 2)
  {
    dmd.setBrightness(bri);
    for (int a = 0 - (strlen(message) * 6) ; a <= 128; a++) {
      dmd.drawString(a, y, message);
      if (a == 128)
      {
        dmd.clearScreen();
      }
    }
  }
  else if (ani == 3)
  {
    dmd.setBrightness(bri);
    dmd.clearScreen();
    for (int a = 128; a > 0 - z  ; a--) {
      dmd.drawString(a, y, message, GRAPHICS_ON);

    }
  }
}

void LinkSerial()
{

        

  if (Serial1.available())
  {

    StaticJsonDocument<1024> doc;

    DeserializationError err = deserializeJson(doc, Serial1);

    if (err == DeserializationError::Ok)
    {

      message = doc["message"];
      Serial.println(message);

      brightness = doc["brightness"];
      Serial.println(brightness);

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
