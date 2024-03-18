const unsigned int MAX_MESSAGE_LENGTH = 31;
float PHval, Waterlvl, Light, Temp;
int Red,Green,Blue;
//stringPH=PH:7.16, W:22, L:  39, T:  40;

#define PUMP_PIN D7
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

#include "config.h"


AdafruitIO_Feed *analog1 = io.feed("PHval");
AdafruitIO_Feed *analog2 = io.feed("Temp");
AdafruitIO_Feed *analog3 = io.feed("Waterlvl");
AdafruitIO_Feed *analog4 = io.feed("Light");
AdafruitIO_Feed *analog5 = io.feed("red");
AdafruitIO_Feed *analog6 = io.feed("blue");
AdafruitIO_Feed *analog7 = io.feed("green");
AdafruitIO_Feed *digital1 = io.feed("pumpval");



void setup() {

pinMode(PUMP_PIN, OUTPUT);

// start the serial connection
  Serial.begin(9600);
    u8g2.begin();


  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

//FastLED
  analog5->onMessage(handleMessage1);
  analog6->onMessage(handleMessage2);
  analog7->onMessage(handleMessage3);

  //PUMP
  digital->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

//FastLED

  analog5->get();
  analog6->get();
  analog7->get();

//PUMP

  digital->get();

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
  



  

  io.run();


  Serial.println();
  Serial.println();

  Serial.println();
  Serial.print("sending -> ");
  Serial.print(PHval);
  Serial.print(" , ");
  Serial.print(Temp);
  Serial.print(" , ");
  Serial.print(Waterlvl);
  Serial.print(" , ");
  Serial.print(Light);
  
  analog1->save(PHval);
  analog2->save(Temp);
  analog3->save(Waterlvl);
  analog4->save(Light);

Serial.println("------RGB------");
Serial.println(Red);
Serial.println(Blue);
Serial.println(Green);

}


void handleMessage1(AdafruitIO_Data *data1) {

  // convert the data to integer
   Red = data1->toInt();

  Serial.print("receivedRed <- ");
  Serial.println(Red);

}

void handleMessage3(AdafruitIO_Data *data3) {

  // convert the data to integer
  Green = data3->toInt();

  Serial.print("receivedGreen <- ");
  Serial.println(Green);

}


void handleMessage2(AdafruitIO_Data *data2) {

  // convert the data to integer
   Blue = data2->toInt();

  Serial.print("receivedBlue <- ");
  Serial.println(Blue);

}

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");


  digitalWrite(PUMP_PIN, data->toPinLevel());
}
