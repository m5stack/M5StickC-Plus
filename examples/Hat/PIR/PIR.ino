#include <M5StickCPlus.h>

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 4);
    M5.Lcd.println("PIR TEST");

    pinMode(36, INPUT_PULLUP);
}

void loop() {
    // put your main code here, to run repeatedly:

    M5.Lcd.setCursor(110, 55, 6);
    M5.Lcd.println(digitalRead(36));
}
