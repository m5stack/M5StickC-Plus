#include <M5StickCPlus.h>

void setup() {
  // put your setup code here, to run once:
    M5.begin();
    
    Wire.begin(32,33);

    M5.Lcd.setRotation(3);
    M5.Lcd.setCursor(60, 0, 4);
    M5.Lcd.print("Ultrasonic");
}

float readEUS()
{
    uint32_t data;
    Wire.beginTransmission(0x57);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(120);
    Wire.requestFrom(0x57,3);
    data  = Wire.read();data <<= 8;
    data |= Wire.read();data <<= 8;
    data |= Wire.read();
    return float(data) / 1000;
    
}

void loop() {
                                                                            
    float newvalue = 0;

    while(1)
    {
        newvalue = readEUS();
        M5.Lcd.setCursor(50,67,4);

        if(( newvalue < 1500 )&&( newvalue > 20 ))
        {
            M5.Lcd.printf("%.2fmm",newvalue);
            if(( newvalue < 100 )&&( newvalue > 20 ))
            {
               M5.Lcd.fillRect(155, 68, 15, 25, BLACK);
            }
            if(( newvalue < 1000 )&&( newvalue > 20 ))
            {
               M5.Lcd.fillRect(170, 68, 15, 25, BLACK);
            }
        }
        delay(100);
    }

}
