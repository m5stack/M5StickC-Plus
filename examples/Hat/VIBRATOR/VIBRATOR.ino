#include <M5StickCPlus.h>

#define CTRL_PIN 26

enum Vibrate_mode_t {
    stop = 0,
    weak,
    medium,
    strong,
    very_strong,
    strongest,
};
static uint8_t mode = stop;

void Vibrator_hat_update() {
    ledcWrite(1, mode * 51);

    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf(" VIBRATOR HAT\n");
    M5.Lcd.printf(" G%d PWM:%3d%%", CTRL_PIN, ledcRead(1) / 51 * 20);
    M5.Lcd.printf("\n\n        Press\n           ->");

    M5.Lcd.fillRoundRect(110, 70, 20, 50, 1,
                         (mode >= weak ? TFT_GREEN : TFT_BLACK));
    M5.Lcd.fillRoundRect(86, 70, 20, 50, 1,
                         (mode >= medium ? TFT_GREENYELLOW : TFT_BLACK));
    M5.Lcd.fillRoundRect(62, 70, 20, 50, 1,
                         (mode >= strong ? TFT_YELLOW : TFT_BLACK));
    M5.Lcd.fillRoundRect(38, 70, 20, 50, 1,
                         (mode >= very_strong ? TFT_ORANGE : TFT_BLACK));
    M5.Lcd.fillRoundRect(14, 70, 20, 50, 1,
                         (mode >= strongest ? TFT_RED : TFT_BLACK));
}

void setup() {
    M5.begin();

    ledcAttachPin(CTRL_PIN, 1);
    ledcSetup(1, 10000, 8);

    Vibrator_hat_update();
}

void loop() {
    if (M5.BtnA.isPressed()) {
        mode++;
        if (mode > strongest) mode = stop;
        while (M5.BtnA.read())
            ;
        Vibrator_hat_update();
    }

    delay(50);
    M5.update();
}
