// Adafruit IO Analog Out Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-analog-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this should correspond to a pin with PWM capability
//#define LED_PIN 5
int Red,Green,Blue;

// set up the 'analog' feed
AdafruitIO_Feed *analog5 = io.feed("red");
AdafruitIO_Feed *analog6 = io.feed("blue");
AdafruitIO_Feed *analog7 = io.feed("green");



void setup() {


  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'analog' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  analog5->onMessage(handleMessage1);
  analog6->onMessage(handleMessage2);
  analog7->onMessage(handleMessage3);
  

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  analog5->get();
  analog6->get();
  analog7->get();


}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

Serial.println(Red);
Serial.println(Blue);
Serial.println(Green);

}

// this function is called whenever an 'analog' message
// is received from Adafruit IO. it was attached to
// the analog feed in the setup() function above.
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
