/*
    Description: Read the ADC value measured by the Watering Unit, and the water
   pump can be switched on and off through the middle button.
*/

#include <M5StickCPlus.h>

#define INPUT_PIN 33
#define PUMP_PIN  32

bool flag = true;
int rawADC;

void setup() {
    M5.begin();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString("Watering", 70, 10);
    M5.Lcd.drawString("TEST", 70, 40, 1);
    M5.Lcd.drawString("ON/OFF", 70, 190, 1);
    M5.Lcd.drawString("PUMP", 70, 210, 1);
    pinMode(INPUT_PIN, INPUT);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(25, OUTPUT);
    digitalWrite(25, 0);
}

char info[30];

void loop() {
    rawADC = analogRead(INPUT_PIN);
    M5.Lcd.setCursor(20, 100);
    M5.Lcd.print("ADC:" + String(rawADC));
    Serial.print("Watering ADC value: ");
    Serial.println(rawADC);
    if (M5.BtnA.wasPressed()) {
        digitalWrite(PUMP_PIN, flag);
        flag = !flag;
    }
    M5.update();
    delay(100);
}
