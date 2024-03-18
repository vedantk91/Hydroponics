const unsigned int MAX_MESSAGE_LENGTH = 12;
int PHval,Waterlvl,Light,Temp;
//stringPH=ph=23.67, W=100, L=160, T=31 ;
int green,red,blue;
int PUMP=D7;
int pumpval;


#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 13
#include <U8g2lib.h>
CRGB leds[NUM_LEDS];

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void setup() {
 u8g2.begin();
 FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
 pinMode(PUMP, OUTPUT);
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
   char inByte = Serial.read();

   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
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
String stringPH=String(message);
     //Serial.println(stringPH.substring(1,4));
     //Reset for the next message
     message_pos = 0;
     
PHval=(stringPH.substring(3,8)).toFloat();
Waterlvl=(stringPH.substring(12,15)).toFloat();
Light=(stringPH.substring(19,22)).toFloat();
Temp=(stringPH.substring(26,28)).toFloat();
Serial.println(PHval);
Serial.println(Waterlvl);
Serial.println(Light);
Serial.println(Temp);

//OLED
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB12_tr); // choose a suitable font
  //u8g2.drawStr(35,35,Temp);  // write something to the internal memory
  u8g2.setCursor(0, 0); //start at the beginning
  u8g2.print(Temp);
    u8g2.setCursor(0, 0); //start at the beginning
  u8g2.print(PHval);
    u8g2.setCursor(0, 0); //start at the beginning
  u8g2.print(Light);
    u8g2.setCursor(0, 0); //start at the beginning
  u8g2.print(Waterlvl);
  u8g2.sendBuffer();    // transfer internal memory to the display


  //FASTLED
  for(int i=0;i<NUM_LEDS;i++){
  leds[i] = CRGB(green,red, blue);
}
  FastLED.show();
  //https://forum.arduino.cc/t/how-to-display-a-string-variable-with-the-u8g2-library/622278/2

  //PUMP

  digitalWrite(PUMP, pumpval); 

  delay(1000); 

   }
 }
}
