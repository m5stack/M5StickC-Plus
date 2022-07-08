//#include "FastLED.h"
#include <math.h>
#include "display.h"
#include <M5StickCPlus.h>
#include "yunBoard.h"

uint16_t LIGHT_COLOR;
uint8_t baseY0 = 40;

uint8_t lightR = 40;
uint8_t lightG = 40;
uint8_t lightB = 0;

extern uint16_t light;
void display_light(void) {
    uint16_t lit;
    lit = 4096 - light;
    // Serial.printf("lit:%d\r\n", lit);
    lit = lit / 1000;
    // Serial.printf("lit:%d\r\n", lit);
    switch (lit) {
        case 0:
            display_light0();
            break;
        case 1:
            display_light1();
            break;
        case 2:
            display_light2();
            break;
        case 3:
            display_light3();
            break;
        case 4:
            display_light4();
            break;
        default:
            display_light0();
            break;
    }
}

void display_light0(void) {
    uint8_t i, j;
    baseY0      = 40;
    LIGHT_COLOR = TFT_BLUE;
    M5.Lcd.fillCircle(160, 0, 30, LIGHT_COLOR);
    LIGHT_COLOR = TFT_BLACK;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 7; i++) {
            M5.Lcd.drawLine(int(159 - baseY0 * sin(M_PI_2 / 6 * i)),
                            int(baseY0 * cos(M_PI_2 / 6 * i)),
                            int(159 - (baseY0 + 5) * sin(M_PI_2 / 6 * i)),
                            int((baseY0 + 5) * cos(M_PI_2 / 6 * i)),
                            LIGHT_COLOR);
        }
        baseY0 += 10;
    }
}
void display_light1(void) {
    uint8_t i, j;
    baseY0      = 40;
    LIGHT_COLOR = TFT_YELLOW;
    M5.Lcd.fillCircle(160, 0, 30, LIGHT_COLOR);
    for (j = 0; j < 4; j++) {
        if (j > 0)
            LIGHT_COLOR = TFT_BLACK;
        else
            LIGHT_COLOR = TFT_YELLOW;
        for (i = 0; i < 7; i++) {
            M5.Lcd.drawLine(int(159 - baseY0 * sin(M_PI_2 / 6 * i)),
                            int(baseY0 * cos(M_PI_2 / 6 * i)),
                            int(159 - (baseY0 + 5) * sin(M_PI_2 / 6 * i)),
                            int((baseY0 + 5) * cos(M_PI_2 / 6 * i)),
                            LIGHT_COLOR);
        }
        baseY0 += 10;
    }
}

void display_light2(void) {
    uint8_t i, j;
    baseY0      = 40;
    LIGHT_COLOR = TFT_YELLOW;
    M5.Lcd.fillCircle(160, 0, 30, LIGHT_COLOR);
    for (j = 0; j < 4; j++) {
        if (j > 1)
            LIGHT_COLOR = TFT_BLACK;
        else
            LIGHT_COLOR = TFT_YELLOW;
        for (i = 0; i < 7; i++) {
            M5.Lcd.drawLine(int(159 - baseY0 * sin(M_PI_2 / 6 * i)),
                            int(baseY0 * cos(M_PI_2 / 6 * i)),
                            int(159 - (baseY0 + 5) * sin(M_PI_2 / 6 * i)),
                            int((baseY0 + 5) * cos(M_PI_2 / 6 * i)),
                            LIGHT_COLOR);
        }
        baseY0 += 10;
    }
}
void display_light3(void) {
    uint8_t i, j;
    baseY0      = 40;
    LIGHT_COLOR = TFT_YELLOW;
    M5.Lcd.fillCircle(160, 0, 30, LIGHT_COLOR);
    for (j = 0; j < 4; j++) {
        if (j > 2)
            LIGHT_COLOR = TFT_BLACK;
        else
            LIGHT_COLOR = TFT_YELLOW;
        for (i = 0; i < 7; i++) {
            M5.Lcd.drawLine(int(159 - baseY0 * sin(M_PI_2 / 6 * i)),
                            int(baseY0 * cos(M_PI_2 / 6 * i)),
                            int(159 - (baseY0 + 5) * sin(M_PI_2 / 6 * i)),
                            int((baseY0 + 5) * cos(M_PI_2 / 6 * i)),
                            LIGHT_COLOR);
        }
        baseY0 += 10;
    }
}
void display_light4(void) {
    uint8_t i, j;
    baseY0      = 40;
    LIGHT_COLOR = TFT_YELLOW;
    M5.Lcd.fillCircle(160, 0, 30, LIGHT_COLOR);
    for (j = 0; j < 4; j++) {
        if (j > 3)
            LIGHT_COLOR = TFT_BLACK;
        else
            LIGHT_COLOR = TFT_YELLOW;
        for (i = 0; i < 7; i++) {
            M5.Lcd.drawLine(int(159 - baseY0 * sin(M_PI_2 / 6 * i)),
                            int(baseY0 * cos(M_PI_2 / 6 * i)),
                            int(159 - (baseY0 + 5) * sin(M_PI_2 / 6 * i)),
                            int((baseY0 + 5) * cos(M_PI_2 / 6 * i)),
                            LIGHT_COLOR);
        }
        baseY0 += 10;
    }
}
void led_off(void) {
    lightR = 0;
    lightG = 0;
    lightB = 0;
    led_set_all(lightR << 16 | lightG << 8 | lightB);
}
uint32_t cycle_time = 0;
void led_breath(void) {
    static int8_t i   = -5;
    static int8_t dir = 1;
    if (millis() > cycle_time) {
        cycle_time = millis() + 50;
        if (dir == 1)
            i++;
        else
            i--;

        if (i > 9) {
            dir = -1;
            i   = 10;
        }
        if (i < -10) {
            dir = 1;
            i   = -11;
        }
        lightR = (lightR + dir * 5);
        lightG = (lightG + dir * 5);
        lightB = 0;
        Serial.print(i);
        Serial.print("\r\n");
        Serial.printf("R:%d, G:%d, B:%d \r\n", lightR, lightG, lightB);
        led_set_all(lightR << 16 | lightG << 8 | lightB);
    }
}
