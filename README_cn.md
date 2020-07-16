# M5StickC-Plus Arduino Library

English | [中文](README_cn.md)

* **[点击此处](https://docs.m5stack.com/#/zh_CN/core/m5stickc_plus)** 查看产品文档。

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/image/product_jpg/assets/img/product_pics/core/minicore/m5stickc_plus/m5stickc_plus_01.jpg" alt="M5StickC_01" width="350">

## 描述


**M5StickC PLUS** 是M5StickC的大屏幕版本，主控采用ESP32-PICO-D4模组，具备蓝牙4.2与WIFI功能，小巧的机身内部集成了丰富的硬件资源，如红外、RTC、麦克风、LED、IMU、按键、蜂鸣器、PMU等，在保留原有M5StickC功能的基础上加入了无源蜂鸣器，同时屏幕尺寸升级到1.14寸、135*240分辨率的TFT屏幕，相较之前的0.96寸屏幕增加18.7%的显示面积，电池容量达到120mAh，接口同样支持HAT与Unit系列产品.


**开关机操作：**

* 开机：按复位按键，持续至少 2 秒

* 关机：按复位按键，持续至少 6 秒

**注意：**

* M5StickC Plus支持的波特率: 1200 ~115200, 250K, 500K, 750K, 1500K

* G36/G25共用同一个端口，当使用其中一个引脚时要将另外一个引脚设置为浮空输入
* 比如要使用G36引脚作为ADC输入，则配置G25引脚为浮空状态

```arduino
setup()
{
   M5.begin();
   pinMode(36, INPUT);
   gpio_pulldown_dis(GPIO_NUM_25);
   gpio_pullup_dis(GPIO_NUM_25);
}
```

## 管脚映射

**RED LED & 红外发射管 & 按键 A & 按键 B**

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

分辨率：135 * 240

<table>
 <tr><td>ESP32 </td><td>GPIO15</td><td>GPIO13</td><td>GPIO23</td><td>GPIO18</td><td>GPIO5</td></tr>
 <tr><td>TFT LCD</td><td>TFT_MOSI</td><td>TFT_CLK</td><td>TFT_DC</td><td>TFT_RST</td><td>TFT_CS</td></tr>
</table>

**GROVE PORT**

<table>
 <tr><td>ESP32 </td><td>GPIO33</td><td>GPIO32</td><td>5V</td><td>GND</td></tr>
 <tr><td>GROVE port</td><td>SCL</td><td>SDA</td><td>5V</td><td>GND</td></tr>
</table>

**麦克分 (SPM1423)**

<table>
 <tr><td>ESP32 </td><td>GPIO0</td><td>GPIO34</td></tr>
 <tr><td>MICPHONE</td><td>CLK</td><td>DATA</td></tr>
</table>

**6-Axis 姿态传感器 (MPU6886) & 电源管理 IC (AXP192)**

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


## 相关链接

-  **datasheet**

    - [ESP32-PICO](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/esp32-pico-d4_datasheet_en.pdf)
    - [ST7789v2](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/ST7789V.pdf)
    - [BM8563](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/BM8563_V1.1_cn.pdf)
    - [MPU6886](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/MPU-6886-000193%2Bv1.1_GHIC_en.pdf)
    - [AXP192](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/AXP192_datasheet_en.pdf)
    - [SPM1423](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/SPM1423HM4H-B_datasheet_en.pdf)