#include <M5StickCPlus.h>
#include "CBACK_DRIVER.h"

bool flag = true;

CBACK_DRIVER Driver;

void setup() {
    M5.begin();
    Driver.Init();
    Serial.println("C Back Driver Test...");
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setRotation(1);
    M5.Lcd.drawCentreString("C BACK Driver", 120, 30, 4);
    M5.Lcd.drawCentreString("Click Btn A Start", 120, 65, 4);
}

void loop() {
    if (M5.BtnA.wasPressed()) {
        flag = !flag;
    }
    if (flag) {
        for (int i = 1; i <= 4; i++) {
            Driver.SetServoAngle(i, 90);
            // Driver.ReadServoPulse(i,1500);
        }
        Driver.SetOutput(1);
    } else {
        for (int i = 1; i <= 4; i++) {
            Driver.SetServoAngle(i, 120);
            // Driver.ReadServoPulse(i,2000);
        }
        Driver.SetOutput(0);
    }
    Serial.print("PortB ADC: ");
    Serial.println(Driver.ReadADC());
    M5.update();
}
