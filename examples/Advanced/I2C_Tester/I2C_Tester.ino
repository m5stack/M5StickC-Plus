/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC-Plus sample source code
*                          配套  M5StickC-Plus 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc_plus
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc_plus
*
* Describe: I2C Scanner.  I2C探测
* Date: 2021/9/18
*******************************************************************************
  This program scans the addresses 1-127 continuosly and shows the devices found
on the TFT. 该程序连续扫描地址 1-127 并显示在外部(内部)I2C发现的设备。
*/
#include <M5StickCPlus.h>

void setup() {
    M5.begin();
    // For HY2.0-4P
    Wire.begin();  // Init wire and join the I2C network.
                   // 初始化wire，并且加入到I2C网络
    // For HAT
    // Wire1.begin(0,26);
    // Wire.begin(21, 22); //Detect internal I2C, if this sentence is not added,
    // it will detect external I2C.  检测内部I2C,若不加此句为检测外部I2C
    M5.Lcd.setRotation(3);  // Rotate the screen.  旋转屏幕
    M5.Lcd.setTextColor(
        YELLOW);  // Set the font color to yellow.  设置字体颜色为黄色
    M5.Lcd.println("M5StickC I2C Tester");  // Print a string on the screen.
                                            // 在屏幕上打印字符串
}

int textColor = YELLOW;

void loop() {
    int address;
    int error;
    int error1;
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("scanning Address [HEX]");

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(
            address);  // Data transmission to the specified device address
                       // starts.   开始向指定的设备地址进行传输数据
        Wire1.beginTransmission(address);
        error = Wire.endTransmission(); /*Stop data transmission with the slave.
                  停止与从机的数据传输 0: success.  成功 1: The amount of data
                  exceeds the transmission buffer capacity limit.
                  数据量超过传送缓存容纳限制 return value:              2:
                  Received NACK when sending address.  传送地址时收到 NACK 3:
                  Received NACK when transmitting data.  传送数据时收到 NACK
                                             4: Other errors.  其它错误 */
        error1 = Wire1.endTransmission();
        if ((error == 0) || (error1 == 0)) {
            M5.Lcd.print(address, HEX);
            M5.Lcd.print(" ");
        } else
            M5.Lcd.print(".");

        delay(10);
    }

    if (textColor == YELLOW)
        textColor = GREEN;
    else
        textColor = YELLOW;
    M5.Lcd.setTextColor(textColor,
                        BLACK);  // Set the foreground color of the text to
                                 // textColor and the background color to BLACK.
                                 // 设置文字的前景色为textColor背景色为BLACK
}
