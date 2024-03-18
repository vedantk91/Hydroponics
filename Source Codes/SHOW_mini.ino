#include <U8g2lib.h>
#include <FastLED.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#define NUM_LEDS 50
#define DATA_PIN 13
CRGB leds[NUM_LEDS];

int PHval,Waterlvl,Light,Temp;
String stringpH;
int PUMP=D5;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    pinMode(PUMP, OUTPUT);
}

void loop() {
  
  while (Serial.available()) 
  {
    Serial.write(Serial.read());
  }

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB12_tr); // choose a suitable font
  u8g2.drawStr(35,35,"EFarm");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display

  for(int i=0;i<NUM_LEDS;i++){
  leds[i] = CRGB(0,125, 125);
}
  FastLED.show();
  delay(500);
  delay(1000);  

  digitalWrite(PUMP, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(PUMP, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}
