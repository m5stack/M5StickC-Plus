#include <M5StickCPlus.h>

#ifdef __cplusplus
extern "C" {
#include "IMU_6886.h"
}
#endif

IMU_6886 imu6886;

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temp = 0;

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(40, 5);
    M5.Lcd.println("MPU6886 TEST");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(50, 30);
    M5.Lcd.println("  X      Y       Z");
    M5.Lcd.setCursor(45, 90);
    M5.Lcd.println("  Pitch   Roll    Yaw");
    imu6886.Init(32, 33);
}

void loop() {
    // put your main code here, to run repeatedly:
    imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
    imu6886.getAccelData(&accX, &accY, &accZ);
    imu6886.getTempData(&temp);
    imu6886.getAhrsData(&pitch, &roll, &yaw);
    M5.Lcd.setCursor(45, 45);
    M5.Lcd.printf("%.2f   %.2f   %.2f   ", gyroX, gyroY, gyroZ);
    M5.Lcd.setCursor(185, 45);
    M5.Lcd.print("o/s");
    M5.Lcd.setCursor(45, 60);
    M5.Lcd.printf("%.2f   %.2f   %.2f   ", accX, accY, accZ);
    M5.Lcd.setCursor(185, 60);
    M5.Lcd.print("G");
    M5.Lcd.setCursor(45, 75);
    M5.Lcd.printf("Temperature : %.2f  C", temp);
    M5.Lcd.setCursor(45, 105);
    M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);
    delay(100);
}
