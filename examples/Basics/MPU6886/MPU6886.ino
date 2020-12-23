#include <M5StickCPlus.h>

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float temp = 0;
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(50, 15);
  M5.Lcd.println("MPU6886 TEST");
  M5.Lcd.setCursor(30, 30);
  M5.Lcd.println("  X       Y       Z");
  M5.Imu.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.Imu.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.Imu.getAccelData(&accX,&accY,&accZ);
  M5.Imu.getTempData(&temp);

  M5.Lcd.setCursor(30, 45);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ", gyroX, gyroY,gyroZ);
  M5.Lcd.setCursor(170, 45);
  M5.Lcd.print("o/s");
  M5.Lcd.setCursor(30, 60);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ",accX * 1000,accY * 1000, accZ * 1000);
  M5.Lcd.setCursor(185, 60);
  M5.Lcd.print("mg");
  M5.Lcd.setCursor(30, 75);
  M5.Lcd.printf("Temperature : %.2f C", temp);
  delay(100);
}
