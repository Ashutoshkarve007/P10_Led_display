# P10_Led_display

**Communication is done between ESP32 and SIM7600EI**

> We remove U6 From Board

DMD32 Library is woring completly fine for ESP32 and P10 Display.
Click Sketch → Include Library → Manage Libraries...
[DMD32](https://github.com/Qudor-Engineer/DMD32)

PinOut>>
<ul>
<li> #define PIN_DMD_nOE		9	<br/> >> Active low Output Enable. setting this low, lights all the LEDs in the selected rows. Brightness control. </li>
  <li> #define PIN_DMD_A 13		 </li>
  <li> #define PIN_DMD_B 12		 </li>
  <li> #define PIN_DMD_CLK 18	 </li>
  <li> #define PIN_DMD_SCLK	14   </li>
  <li> #define PIN_DMD_R_DATA 23	 </li>
</ul>
<br/>

![alt text](https://github.com/Ashutoshkarve007/P10_Led_display/blob/main/Images/connector_pin_change.png)
<ul>
  >Connection on SIM7600 and ESP32
  <li> Reference for Tx Rx connection </li>
  </ul>

![alt text](https://github.com/Ashutoshkarve007/P10_Led_display/blob/main/Images/wiring%20Diagram.PNG)
