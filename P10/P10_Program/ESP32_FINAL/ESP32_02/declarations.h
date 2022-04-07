#ifndef _DECLARATIONS_H
#define _DECLARATIONS_H
//#include <DMD32.h>
//#include <stdio.h>
//#include <string.h>

#define DEBUG true
#define MODE_1A

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 4
#define DISPLAYS_DOWN 2

#define UART_BAUD 115200

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

/*    SIM7600 Pins        */
#define LTE_PWRKEY_PIN 26
#define LTE_RESET_PIN 33
#define LTE_FLIGHT_PIN 27

#define MODEM_RXD 16
#define MODEM_TXD 17

/*   P10 pins            */
#define PIN_DMD_nOE  9          //22       //4   // D22 active low Output Enable, setting this low lights all the LEDs in the selected rows. Can pwm it at very high frequency for brightness control.
#define PIN_DMD_A 13            //19       //13   // D19  default is 15   
#define PIN_DMD_B 12            //21       //12     // D21
#define PIN_DMD_CLK 18          //18       //18  // D18_SCK  is SPI Clock if SPI is used   R39
#define PIN_DMD_SCLK  14        //2        //14    // D02
#define PIN_DMD_R_DATA    23    //23       //23  // D23_MOSI is SPI Master Out if SPI is used


//#define EEPROM_SIZE 1



/*              Function Prototypes               */
void connectAWS();
void hardwareTimer();
//void graphicsmode();
//void switchcommands();
//void setupAP(void);
//bool testWifi(void);
//void launchWeb(void);
//void setupAP(void);


#endif
