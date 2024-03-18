 #include <FastLED.h>

#define NUM_LEDS 50


#define DATA_PIN 2
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
 #include "DHT.h"

#define DHTPIN 4


#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

 int fanpin=25;      //IN2
 int motorpin=26;    //IN4
 int valvepin=27 ;    //IN3
 
 
 void setup() {
Serial.begin(9600);

Serial.println(F("DHT11 test!"));

dht.begin();
pinMode(fanpin,OUTPUT);
pinMode(motorpin,OUTPUT);
pinMode(valvepin,OUTPUT);

digitalWrite(fanpin,HIGH);      //Fan OFF
digitalWrite(motorpin,HIGH);      //motor OFF
digitalWrite(valvepin,HIGH);      //valve OFF
pinMode(DATA_PIN,OUTPUT);

FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

}

void loop() {

  delay(2000);

 float h = dht.readHumidity();

  float t = dht.readTemperature();

  float f = dht.readTemperature(true);
  
//  if (isnan(h) || isnan(t) || isnan(f)) {
//    Serial.println(F("Failed to read from DHT sensor!"));
//    return;
//  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  
  for(int i=0;i<NUM_LEDS;i++){
  leds[i] = CRGB(255,255, 0);
}
  FastLED.show();
  delay(500);


digitalWrite(fanpin,LOW);       //Fan ON
Serial.println("Fan ON");
delay(1000);
digitalWrite(motorpin,LOW);    //motor ON
Serial.println("Motor ON");
delay(4000);
digitalWrite(motorpin,HIGH);      //motor OFF
Serial.println("Motor OFF");
delay(1000);
digitalWrite(valvepin,LOW);       //valve ON
Serial.println("Valve ON");
delay(10000);
digitalWrite(valvepin,HIGH);      //valve OFF
Serial.println("Valve OFF");
delay(1000);
digitalWrite(fanpin,HIGH);      //Fan OFF
Serial.println("Fan OFF");
delay(5000);



 

}





  
