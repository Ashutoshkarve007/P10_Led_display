# P10_Led_display

**Communication is done between ESP32 and SIM7600EI**

> We remove U6 From Board

Program is working in Normal ESP32 Wroom
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

![alt text](https://github.com/Ashutoshkarve007/P10_Led_display/blob/main/Images/wiring%20Diagram.PNG)
