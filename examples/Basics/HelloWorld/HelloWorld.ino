/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC-Plus sample source code
*                          配套  M5StickC-Plus 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc_plus
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc_plus
*
* Describe: Hello World.
* Date: 2021/9/14
*******************************************************************************
*/
#include <M5StickCPlus.h>

/* After M5StickC Plus is started or reset
  the program in the setUp () function will be run, and this part will only be
  run once. 在 M5StickC Plus
  启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
    M5.begin();  // Initialize M5StickC Plus.  初始化 M5StickC PLus
    M5.Lcd.setTextSize(3);  // Set font size.  设置字体大小
    M5.Lcd.setRotation(3);  // Rotate the screen. 将屏幕旋转
    // LCD display.  Lcd显示
    M5.Lcd.print("Hello World");
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
}