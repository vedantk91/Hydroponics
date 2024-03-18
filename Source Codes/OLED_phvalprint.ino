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
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(0, 10, "Hello World!");
        u8g2.sendBuffer();

}

void loop() {

  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    //   char inByte = Serial.read();
    String inByte = Serial.readString();
    //Serial.println(inByte.length());
    inByte.trim();
    //Serial.println(inByte.length());
    Serial.println(inByte);
    //Serial.println(inByte.substring(3, 8));

    PHval = (inByte.substring(3, 8)).toFloat();
    Waterlvl = (inByte.substring(11, 13)).toFloat();
    Light = (inByte.substring(18, 22)).toFloat();
    Temp = (inByte.substring(28, 30)).toFloat();
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

    //  u8g2.drawStr(0, 10, "Hello World!");  // write something to the internal memory
      u8g2.drawStr(0, 40, ph.c_str());
         u8g2.drawStr(0, 40, ph.c_str());
      u8g2.sendBuffer();
      // u8g2.drawStr(20, 80, tp.c_str());
      // u8g2.drawStr(40, 40, wl.c_str());
      //u8g2.drawStr(60, 40, li.c_str());
      // u8g2.sendBuffer();
      //delay(1000);
      // write something to the internal memory
      // u8g2.drawStr(5, 30, fixed.c_str());  // write String from SRAM
      //u8g2.drawStr(0, 40, F("ORIGINAL IN FLASH").c_str());  // does not work
      //u8g2.setCursor(0, 40);  //separate setCursor(), print() accepts all Print.h arguments
      //u8g2.print(F("ORIGINAL IN FLASH"));  //

      //u8g2.clearBuffer();          // clear the internal memory
      // u8g2.setFont(u8g2_font_ncenB12_tr); // choose a suitable font
      //u8g2.drawStr(35,35,Temp);  // write something to the internal memory
      //u8g2.setCursor(0, 0); //start at the beginning
      //u8g2.print(Temp);
      //    u8g2.setCursor(0, 0); //start at the beginning
      //  u8g2.print(PHval);
      //    u8g2.setCursor(0, 0); //start at the beginning
      //  u8g2.print(Light);
      //    u8g2.setCursor(0, 0); //start at the beginning

      //u8g2.drawStr(35,35,Temp);
      //  u8g2.print(Waterlvl);
      // u8g2.sendBuffer();    // transfer internal memory to the display
      //u8g2.clearBuffer();          // clear the internal memory
      //u8g2.setFont(u8g2_font_ncenB12_tr); // choose a suitable font
      //u8g2.drawStr(35,35,"EFarm");  // write something to the internal memory
      //u8g2.sendBuffer();          // transfer internal memory to the display
      
    //   u8g2.clearBuffer();
    //  u8g2.setFont(u8g2_font_ncenB12_tr);
    //
    //        u8g2.drawStr(0, 10, "Hello World!");  // write something to the internal memory
   //         u8g2.drawStr(0, 40, ph.c_str());

    //Message coming in (check not terminating character) and guard for over message size
    //inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1
    /*      if ( 0)

      {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
      }
      //Full message received...
      else
      {
      //Add null character to string
      message[message_pos] = '\0';

      //     int i;//Print the message (or do other things)
      //   for(i=0;i<12;i++){
      //   if(i==1){
       //   Serial.println(message[i]);  }}

      String stringPH = String(message);
      //Serial.println(stringPH.substring(1,4));
      //Reset for the next message
      message_pos = 0;
      Serial.println(stringPH);
      PHval = (stringPH.substring(3, 8)).toFloat();
      Waterlvl = (stringPH.substring(11, 13)).toFloat();
      Light = (stringPH.substring(18, 22)).toFloat();
      Temp = (stringPH.substring(28, 30)).toFloat();
      Serial.println(PHval);
      Serial.println(Waterlvl);
      Serial.println(Light);
      Serial.println(Temp);

      //OLED
      String tp = String(Temp);
      String ph = String(PHval);
      String wl = String(Waterlvl);
      String li = String(Light);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB12_tr);

      u8g2.drawStr(0, 10, "Hello World!");  // write something to the internal memory
      u8g2.drawStr(0, 40, ph.c_str());
      u8g2.sendBuffer();
      // u8g2.drawStr(20, 80, tp.c_str());
      // u8g2.drawStr(40, 40, wl.c_str());
      //u8g2.drawStr(60, 40, li.c_str());
      // u8g2.sendBuffer();
      //delay(1000);
      // write something to the internal memory
      // u8g2.drawStr(5, 30, fixed.c_str());  // write String from SRAM
      //u8g2.drawStr(0, 40, F("ORIGINAL IN FLASH").c_str());  // does not work
      //u8g2.setCursor(0, 40);  //separate setCursor(), print() accepts all Print.h arguments
      //u8g2.print(F("ORIGINAL IN FLASH"));  //

      //u8g2.clearBuffer();          // clear the internal memory
      // u8g2.setFont(u8g2_font_ncenB12_tr); // choose a suitable font
      //u8g2.drawStr(35,35,Temp);  // write something to the internal memory
      //u8g2.setCursor(0, 0); //start at the beginning
      //u8g2.print(Temp);
      //    u8g2.setCursor(0, 0); //start at the beginning
      //  u8g2.print(PHval);
      //    u8g2.setCursor(0, 0); //start at the beginning
      //  u8g2.print(Light);
      //    u8g2.setCursor(0, 0); //start at the beginning

      //u8g2.drawStr(35,35,Temp);
      //  u8g2.print(Waterlvl);
      // u8g2.sendBuffer();    // transfer internal memory to the display
      //u8g2.clearBuffer();          // clear the internal memory
      //u8g2.setFont(u8g2_font_ncenB12_tr); // choose a suitable font
      //u8g2.drawStr(35,35,"EFarm");  // write something to the internal memory
      //u8g2.sendBuffer();          // transfer internal memory to the display
      }*/
    //   u8g2.clearBuffer();
    //  u8g2.setFont(u8g2_font_ncenB12_tr);
    //
    //        u8g2.drawStr(0, 10, "Hello World!");  // write something to the internal memory
    //        u8g2.drawStr(0, 40, ph.c_str());

  }
}
