// Insert file Library
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_Black_16.h>

SoftDMD dmd(1,1);
// Number of P10 panels used X, Y
DMD_TextBox box(dmd, 2, 1, 32, 16); 
// Set Box (dmd, x, y, Height, Width) 
void setup() {
  dmd.setBrightness(10); // Set brightness 0 - 255 
  dmd.selectFont(Arial_Black_16); // Font used
  dmd.begin();     // Start DMD 
  box.print("Hii"); // Display TEXT SFE
}
void loop() {
}
