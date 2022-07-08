/*
    Description: Detect the current OP 90/180 Unit Photoelectric switch status.
*/

#include <M5StickCPlus.h>

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    // Wire.begin();
    M5.Lcd.setCursor(30, 5, 4);
    M5.Lcd.println("90/180");
    M5.Lcd.setCursor(20, 30, 4);
    M5.Lcd.println("OPTICAL");

    pinMode(33, INPUT_PULLUP);
}

void loop() {
    // put your main code here, to run repeatedly:
    M5.Lcd.setCursor(12, 100, 4);
    M5.Lcd.printf("IR receive:");
    M5.Lcd.setCursor(40, 140, 8);
    M5.Lcd.printf("%d", digitalRead(33));
}
