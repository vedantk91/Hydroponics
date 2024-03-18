#include <FirebaseESP8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include <FastLED.h>
#include <EEPROM.h>

#define NUM_LEDS 50
#define DATA_PIN 2
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

DHT dht(DHTPIN, DHTTYPE);

int fanpin = 25;   // IN2
int motorpin = 26; // IN4
int valvepin = 27; // IN3

unsigned long previousMillis = 0; // last time update
long interval;                    // interval received from Firebase
long time_interval;               // Fetched from EEPROM

#define FIREBASE_HOST "https://e-farm-d2031-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "SgAgOkkygVXy9YSGEvTfzI7Sw7iHxliB7XUl4Hzz"

// #define WIFI_SSID "AndroidAPD9EE"   // Change the name of your WIFI
// #define WIFI_PASSWORD "Vedant@1712" // Change the password of your WIFI

FirebaseData firebaseData1;
FirebaseJson json;

void setup()
{
    Serial.begin(9600);

    Serial.println(F("DHT11 test!"));

    dht.begin();

    pinMode(fanpin, OUTPUT);
    pinMode(motorpin, OUTPUT);
    pinMode(valvepin, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);

    digitalWrite(fanpin, HIGH);   // Fan OFF
    digitalWrite(motorpin, HIGH); // motor OFF
    digitalWrite(valvepin, HIGH); // valve OFF

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed

    WiFiManager wifiManager;
  wifiManager.autoConnect("HydroPhonics");

  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    if (!Firebase.beginStream(firebaseData1, "Electriculture"))
    {
        Serial.println("------------------------------------");
        Serial.println("Can't begin stream connection...");
        Serial.println("REASON: " + firebaseData1.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("Stream success");
    }
}

void loop()
{

    // DHT

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

    Firebase.setFloat(firebaseData1, "/EFarm/HydroPhonics/temp", t);
    Firebase.setFloat(firebaseData1, "/EFarm/HydroPhonics/humidity", h);

    // FastLED

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(255, 255, 0);
    }
    FastLED.show();
    delay(500);

    digitalWrite(fanpin, LOW);
    Serial.println("Fan ON");

    cycle_control();

    // fan off logic

    if (Firebase.getString(firebaseData, "/EFarm/HydroPhonics/Fan"))
    { // On successful Read operation, function returns 1
        String fan = firebaseData.stringData();
        Serial.println(fan);
        fan.replace("\"", "");
        fan1 = fan.toInt();
    }

    if (t < 25 || fan1 == 1)
    {
        digitalWrite(fanpin, HIGH);
        Serial.println("Fan OFF");
    }
}

void cycle_control()
{

    // unsigned long previousMillis = 0; // last time update
    // long interval; // interval received from Firebase
    // long time_interval; // Fetched from EEPROM
    unsigned long currentMillis = millis();

    if (Firebase.getString(firebaseData1, "/EFarm/HydroPhonics/Interval"))
    { // On successful Read operation, function returns 1
        String value = firebaseData1.stringData();
        Serial.println(value);
        value.replace("\"", "");
        interval = value.toInt();
        EEPROM.update(0, interval);
    }

    time_interval = EEPROM.read(0);

    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;

        complete_cycle();
    }
    else
    {
        Serial.println("Sitting Idle...");
    }
}

void complete_cycle()
{
    digitalWrite(motorpin, LOW); // motor ON
    Serial.println("Motor ON");
    delay(4000);
    digitalWrite(motorpin, HIGH); // motor OFF
    Serial.println("Motor OFF");
    delay(1000);
    digitalWrite(valvepin, LOW); // valve ON
    Serial.println("Valve ON");
    delay(10000);
    digitalWrite(valvepin, HIGH); // valve OFF
    Serial.println("Valve OFF");
    delay(1000);
}
