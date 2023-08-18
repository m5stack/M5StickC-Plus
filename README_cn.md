# M5StickC-Plus Library

[![Arduino Compile](https://github.com/m5stack/M5StickC-Plus/actions/workflows/arduino-action-stickc-plus-compile.yml/badge.svg)](https://github.com/m5stack/M5StickC-Plus/actions/workflows/arduino-action-stickc-plus-compile.yml)
[![Arduino Lint](https://github.com/m5stack/M5StickC-Plus/actions/workflows/Arduino-Lint-Check.yml/badge.svg)](https://github.com/m5stack/M5StickC-Plus/actions/workflows/Arduino-Lint-Check.yml)
[![Clang Format](https://github.com/m5stack/M5StickC-Plus/actions/workflows/clang-format-check.yml/badge.svg)](https://github.com/m5stack/M5StickC-Plus/actions/workflows/clang-format-check.yml)

English | [中文](README.md)

<img src="https://static-cdn.m5stack.com/resource/docs/static/assets/img/product_pics/core/minicore/m5stickc_plus/m5stickc_plus_01.webp" alt="M5StickC_01" width="350">

* **如果查看 M5StickC Plus 的详细介绍文档，[点击这里](https://docs.m5stack.com/#/zh_CN/core/m5stickc_plus)**

* **如果想要购买 M5StickC Plus 的话，[点击这里](https://item.taobao.com/item.htm?spm=a1z10.5-c-s.w4002-22404213529.29.5ada1d91KUL15v&id=623449145184)**


## 描述

**M5StickC PLUS** 是M5StickC的大屏幕版本，主控采用ESP32-PICO-D4模组，具备蓝牙4.2与WIFI功能，小巧的机身内部集成了丰富的硬件资源，如红外、RTC、麦克风、LED、IMU、按键、蜂鸣器、PMU等，在保留原有M5StickC功能的基础上加入了无源蜂鸣器，同时屏幕尺寸升级到1.14寸、135*240分辨率的TFT屏幕，相较之前的0.96寸屏幕增加18.7%的显示面积，电池容量达到120mAh，接口同样支持HAT与Unit系列产品.


**开关机操作：**

* 开机：按复位按键，持续至少 2 秒

* 关机：按复位按键，持续至少 6 秒

**注意：**

* M5StickC Plus支持的波特率: 1200 ~115200, 250K, 500K, 750K, 1500K

* G36/G25共用同一个端口，当使用其中一个引脚时要将另外一个引脚设置为浮空输入
* 比如要使用G36引脚作为ADC输入，则配置G25引脚为浮空状态

## 更多信息

**UIFlow 快速上手**: [点击这里](https://docs.m5stack.com/zh_CN/quick_start/m5stickc_plus/uiflow)

**MicroPython API**: [点击这里](https://docs.m5stack.com/zh_CN/mpy/display/m5stack_lvgl)

**Arduino IDE 环境搭建**: [点击这里](https://docs.m5stack.com/zh_CN/quick_start/m5stickc_plus/arduino)

**M5StickC Arduino API**: [点击这里](https://docs.m5stack.com/zh_CN/api/stickc/system_m5stickc)

**引脚地图**: [点击这里](https://docs.m5stack.com/zh_CN/core/m5stickc_plus)
