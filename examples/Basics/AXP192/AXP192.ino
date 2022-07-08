/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC-Plus sample source code
*                          配套  M5StickC-Plus 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc_plus
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc_plus
*
* Describe: APX192.
* Date: 2021/9/14
*******************************************************************************
*/
#include <M5StickCPlus.h>
#include "AXP192.h"
TFT_eSprite tftSprite = TFT_eSprite(&M5.Lcd);

/* After M5StickC Plus is started or reset
  the program in the setUp () function will be run, and this part will only be
  run once. 在 M5StickC Plus
  启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
    M5.begin();             // Init M5StickC Plus.  初始化 M5StickC Plus
    M5.lcd.setRotation(3);  // Rotate the screen. 将屏幕旋转
    tftSprite.createSprite(
        240, 135);  // Create a 240x135 canvas. 创建一块160x80的画布
    tftSprite.setRotation(3);
    M5.Axp.EnableCoulombcounter();  // Enable Coulomb counter.  启用库仑计数器
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
    tftSprite.fillSprite(BLACK);   // Fill the canvas with black.
                                   // 将画布填充为黑色
    tftSprite.setCursor(0, 0, 1);  // Set the cursor position and use the
                                   // 1-point font. 设置光标位置并采用1号字体
    tftSprite.printf("AXP Temp: %.1fC \r\n",
                     M5.Axp.GetTempInAXP192());  // Get the temperature of
                                                 // AXP192. 获取AXP192的温度
    tftSprite.printf("Bat:\r\n  V: %.3fv  I: %.3fma\r\n",
                     M5.Axp.GetBatVoltage(),
                     M5.Axp.GetBatCurrent());  // Output voltage and current of
                                               // Bat. 输出Bat的电压和电流
    tftSprite.printf("USB:\r\n  V: %.3fv  I: %.3fma\r\n",
                     M5.Axp.GetVBusVoltage(),
                     M5.Axp.GetVBusCurrent());  // Output current and voltage of
                                                // USB. 输出USB的电流和电压
    tftSprite.printf("5V-In:\r\n  V: %.3fv  I: %.3fma\r\n",
                     M5.Axp.GetVinVoltage(), M5.Axp.GetVinCurrent());
    tftSprite.printf("Bat power %.3fmw", M5.Axp.GetBatPower());
    tftSprite.pushSprite(40, 20);

    // 0x01 long press(1s), 0x02 press
    if (M5.Axp.GetBtnPress() == 0x02) {
        esp_restart();
    }

    if (M5.BtnA.wasPressed()) {
        // close tft voltage output
        M5.Axp.SetLDO2(false);
    } else if (M5.BtnB.wasPressed()) {
        // close tft voltage output
        M5.Axp.SetLDO2(true);
    }
    M5.update();  // Detect whether the keystroke state has changed.
                  // 检测按键状态是否发生变化
    delay(100);
}
