// Low-level signal can be generated when the magnet S pole is close to the
// front of the sensor OR the N pole is close to the back, and the internal LED
// indicator will light up, the screen wiil display 0.

#include <M5StickCPlus.h>
#define HALL 33

void header() {
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString("HALL", 42, 3, 2);
    M5.Lcd.drawString("Sensor", 30, 30, 2);
}

void setup() {
    M5.begin(true, false, true);
    header();
    pinMode(HALL, INPUT);
}

void loop() {
    bool status = digitalRead(HALL);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(51, 70, 2);
    M5.Lcd.printf("Hall");
    M5.Lcd.setCursor(34, 100, 2);
    M5.Lcd.printf("status:");
    M5.Lcd.setCursor(45, 150, 6);
    M5.Lcd.printf("%d", status);
    //    delay(100);
}
