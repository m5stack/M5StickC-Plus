# M5StickC-Plus Arduino Library

English | [中文](README_cn.md)

* **For the Detailed documentation of M5StickC, please click [here](https://docs.m5stack.com/#/en/core/m5stickc_plus)**

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/image/product_jpg/assets/img/product_pics/core/minicore/m5stickc_plus/m5stickc_plus_01.jpg" alt="M5StickC_01" width="350">

## Description

**M5StickC PLUS** is powered by **ESP32-PICO-D4** with **Bluetooth 4.2** and **WiFi**.It's an upgrade big screen product of M5StickC.It is a portable, easy-to-use, open source, IoT development board. What it can do? This tiny block is able to realize your idea, enlighten your creativity, and help with your IoT prototying in a very short time. It will take away a lot of pains from the development process. M5stickC Plus is one of the core devices in M5Stack product series. The compact body is integrated with rich hardware resources, such as infrared, **RTC**, **Microphone**, **LED**, **IMU**, **Buttons**, **PMU**,etc. Compared with StickC, a buzzer is added and the big screen is upgraded to a 1.14-inch, **135 * 240** resolution LCD Screen.Compared with the previous display area, the display area is increased by 18.7%, and the battery capacity is **120mAh**.It also supports HAT and Unit family products.

**Power switch operation：**

* Power on ：Long press power button for 2 seconds

* Power off ：Short press power button for 6 seconds

**Notice:**

* Baud rate supported by M5StickC Plus: 1200 ~115200, 250K, 500K, 750K, 1500K

* G36/G25 share the same port, when one of the pins is used, the other pin should be set as a floating input
* For example, to use the G36 pin as the ADC input, Configuration the G25 pin as FLOATING

```arduino
setup()
{
   M5.begin();
   pinMode(36, INPUT);
   gpio_pulldown_dis(GPIO_NUM_25);
   gpio_pullup_dis(GPIO_NUM_25);
}
```

## PinMap

**RED LED & IR Transmitter & BUTTON A & BUTTON B**

<table>
 <tr><td>ESP32 </td><td>GPIO10</td><td>GPIO9</td><td>GPIO37</td><td>GPIO39</td><td>GPIO2</td></tr>
 <tr><td>RED LED</td><td>LED Pin</td><td></td><td></td><td></td><td></td></tr>
 <tr><td>IR Transmitter</td><td></td><td>Transmitter Pin</td><td></td><td></td><td></td></tr>
 <tr><td>BUTTON A</td><td></td><td></td><td>Button Pin</td><td></td><td></td></tr>
 <tr><td>BUTTON B</td><td></td><td></td><td></td><td>Button Pin</td><td></td></tr>
 <tr><td>Buzzer</td><td></td><td></td><td></td><td></td><td>Buzzer Pin</td></tr>
</table>


**TFT LCD**

Driver IC：ST7789v2

Resolution：135 * 240

<table>
 <tr><td>ESP32 </td><td>GPIO15</td><td>GPIO13</td><td>GPIO23</td><td>GPIO18</td><td>GPIO5</td></tr>
 <tr><td>TFT LCD</td><td>TFT_MOSI</td><td>TFT_CLK</td><td>TFT_DC</td><td>TFT_RST</td><td>TFT_CS</td></tr>
</table>

**GROVE PORT**

<table>
 <tr><td>ESP32 </td><td>GPIO33</td><td>GPIO32</td><td>5V</td><td>GND</td></tr>
 <tr><td>GROVE port</td><td>SCL</td><td>SDA</td><td>5V</td><td>GND</td></tr>
</table>

**MIC (SPM1423)**

<table>
 <tr><td>ESP32 </td><td>GPIO0</td><td>GPIO34</td></tr>
 <tr><td>MICPHONE</td><td>CLK</td><td>DATA</td></tr>
</table>

**6-Axis posture sensor (MPU6886) & power management IC (AXP192)**

<table>
 <tr><td>ESP32 </td><td>GPIO22</td><td>GPIO21</td>
 <tr><td>6-Axis IMU sensor</td><td>SCL</td><td>SDA</td>
 <tr><td>Power management IC</td><td>SCL</td><td>SDA</td>
</table>

**AXP192**

<table>
 <tr><td>Microphone</td><td>RTC</td><td>TFT backlight</td><td>TFT IC</td><td>ESP32/3.3V MPU6886</td><td>5V GROVE</td>
 <tr><td>LDOio0</td><td>LDO1</td><td>LDO2</td><td>LDO3</td><td>DC-DC1</td><td>IPSOUT</td>
</table>


## Related Link

-  **datasheet**

    - [ESP32-PICO](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/esp32-pico-d4_datasheet_en.pdf)
    - [ST7789v2](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/ST7789V.pdf)
    - [BM8563](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/BM8563_V1.1_cn.pdf)
    - [MPU6886](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/MPU-6886-000193%2Bv1.1_GHIC_en.pdf)
    - [AXP192](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/AXP192_datasheet_en.pdf)
    - [SPM1423](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/SPM1423HM4H-B_datasheet_en.pdf)