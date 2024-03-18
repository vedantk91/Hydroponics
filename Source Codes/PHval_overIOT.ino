
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

#include "config.h"

/************************ Example Starts Here *******************************/

// analog pin 0
//#define PHOTOCELL_PIN A0

// photocell state
//int current = 0;
//int last = -1;

// set up the 'analog' feed
AdafruitIO_Feed *analog = io.feed("PHval");




void setup() {

  // start the serial connection
  Serial.begin(9600);
    u8g2.begin();


  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);

  
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

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
  



  
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // grab the current state of the photocell
  //float current = PHval;

  // return if the value hasn't changed
  //if(current == last)
    //return;

  // save the current state to the analog feed
  Serial.println();
  Serial.println();

  Serial.println();
  Serial.print("sending -> ");
  Serial.println(PHval);
  
  analog->save(PHval);
/*
  // store last photocell state
  float last = current;

  // wait three seconds (1000 milliseconds == 1 second)
  //
  // because there are no active subscriptions, we can use delay()
  // instead of tracking millis()
  delay(3000);*/
}
