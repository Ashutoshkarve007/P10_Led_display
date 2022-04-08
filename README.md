# P10_Led_display

Communication is done between ESP32 and SIM7600EI

Pin Change Via Jumpers:

0E      >>  I04
Data_1  >>  I023
Clk     >>  Io18

We remove U6 From Board

Program is working in Normal ESP32 Wroom
PinOut>>

#define PIN_DMD_nOE		4		// D22 active low Output Enable, setting this low lights all the LEDs in the selected rows. Can pwm it at very high frequency for brightness control.
#define PIN_DMD_A		15		// D19
#define PIN_DMD_B		12		// D21
#define PIN_DMD_CLK		18		// D18_SCK  is SPI Clock if SPI is used
#define PIN_DMD_SCLK		14		// D02
#define PIN_DMD_R_DATA    23	// D23_MOSI is SPI Master Out if SPI is used

![alt text](https://github.com/Ashutoshkarve007/P10_Led_display/blob/main/connector_pin_change.png)
