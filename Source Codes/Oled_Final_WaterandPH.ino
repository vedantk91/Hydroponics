const unsigned int MAX_MESSAGE_LENGTH = 31;
float PHval, Waterlvl, Light, Temp;
//stringPH=PH:7.16, W:22, L:  39, T:  40;

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void setup() {
  u8g2.begin();
  Serial.begin(9600);
}

void loop() {


//Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
  //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

//Read the next available byte in the serial receive buffer

    String inByte = Serial.readString();

    inByte.trim();
    
//Whole Msg

    Serial.println(inByte);

//Cut the String 

    PHval = (inByte.substring(3, 8)).toFloat();
    Waterlvl = (inByte.substring(11, 13)).toFloat();
    Light = (inByte.substring(18, 22)).toFloat();
    Temp = (inByte.substring(28, 30)).toFloat();
    
//Print individual values

    Serial.println(PHval);
    Serial.println(Waterlvl);
    Serial.println(Light);
    Serial.println(Temp);

    String tp = String(Temp);
    String ph = String(PHval);
    String wl = String(Waterlvl);
    String li = String(Light);
    
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.drawStr(0, 15, "EFarm KJSCE!");
    u8g2.drawStr(0, 30, "PH=");
    u8g2.drawStr(40, 30, ph.c_str());
    u8g2.drawStr(0, 50, "WaterLvl=");
    u8g2.drawStr(95, 50, wl.c_str());
    u8g2.sendBuffer();
    
  }
}
