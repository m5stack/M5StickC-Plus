/*
    Description: LASER Unit wireless UART application: burn the program to two
  M5StickCPlus     And connect LASER.TX and LASER.RX to PORT port respectively.
    Point LASER.TX to LASER.RX and press the button on the panel to send
  characters to the receiver of LASER.RX.
*/

#include <M5StickCPlus.h>
void setup() {
    M5.begin();
    // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t
    // txPin, bool invert)
    Serial2.begin(9600, SERIAL_8N1, 33, 32);

    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(GREEN);

    M5.Lcd.setCursor(40, 5);
    M5.Lcd.print("LASER");
    M5.Lcd.setCursor(40, 25);
    M5.Lcd.print("TX/RX");
    M5.Lcd.setCursor(0, 50);  // RX print set
}

void loop() {
    if (M5.BtnA.wasReleased()) {
        Serial2.write("Holle\r\n");
    }
    M5.update();
    if (Serial2.available()) {
        delay(2);
        String ch = Serial2.readString();
        M5.Lcd.print(ch);
    }
}
