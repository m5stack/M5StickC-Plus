#include <M5StickCPlus.h>

void setup() 
{
  M5.Lcd.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(YELLOW);  
  M5.Lcd.setTextSize(1);

  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("M5StickC I2C Tester");
  
  //For HY2.0-4P  
  Wire.begin();
  //For HAT
  Wire1.begin(0,26);

  delay(3000);
  M5.Lcd.fillScreen( BLACK );
}

int textColor=YELLOW;

void loop() 
{
  int address;
  int error;
  int error1;
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("scanning Address [HEX]");
  
  for(address = 1; address < 127; address++ ) 
  {
    Wire.beginTransmission(address);
    Wire1.beginTransmission(address);
    error = Wire.endTransmission();
    error1 = Wire1.endTransmission();
    if((error==0)||(error1==0))
    {
      M5.Lcd.print(address,HEX);M5.Lcd.print(" ");
    }
    else M5.Lcd.print(".");

    delay(10);
  }

  if(textColor==YELLOW) textColor=GREEN;
  else textColor=YELLOW;
  M5.Lcd.setTextColor(textColor,BLACK);  
}
