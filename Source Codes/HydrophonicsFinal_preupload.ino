#include <FirebaseESP32.h>
#include  <WiFi.h>
#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 2
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

DHT dht(DHTPIN, DHTTYPE);

int fanpin = 25;    //IN2
int motorpin = 26;  //IN4
int valvepin = 27 ;  //IN3

#define FIREBASE_HOST "https://smartsilo-4115a-default-rtdb.firebaseio.com/"
#define WIFI_SSID "AndroidAPD9EE" // Change the name of your WIFI
#define WIFI_PASSWORD "Vedant@1712" // Change the password of your WIFI
#define FIREBASE_Authorization_key "olLWg0GODF6Y5IqQ8FgshqvknrkMUyR2B2W2V9UM"


FirebaseData firebaseData1;
FirebaseJson json;

void setup() {
  Serial.begin(9600);

  Serial.println(F("DHT11 test!"));

  dht.begin();

  pinMode(fanpin, OUTPUT);
  pinMode(motorpin, OUTPUT);
  pinMode(valvepin, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  digitalWrite(fanpin, HIGH);     //Fan OFF
  digitalWrite(motorpin, HIGH);     //motor OFF
  digitalWrite(valvepin, HIGH);     //valve OFF


  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_Authorization_key);

  if (!Firebase.beginStream(firebaseData1, "Electriculture" ))
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

void loop() {

  //DHT

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

  //FastLED

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 0);
  }
  FastLED.show();
  delay(500);


  digitalWrite(fanpin, LOW);      //Fan ON
  Serial.println("Fan ON");
  delay(1000);
  digitalWrite(motorpin, LOW);   //motor ON
  Serial.println("Motor ON");
  delay(4000);
  digitalWrite(motorpin, HIGH);     //motor OFF
  Serial.println("Motor OFF");
  delay(1000);
  digitalWrite(valvepin, LOW);      //valve ON
  Serial.println("Valve ON");
  delay(10000);
  digitalWrite(valvepin, HIGH);     //valve OFF
  Serial.println("Valve OFF");
  delay(1000);
  //digitalWrite(fanpin,HIGH);      //Fan OFF
  //Serial.println("Fan OFF");
  //delay(5000);


  // fan off logic

  if (t < 25) {
    digitalWrite(fanpin, HIGH);
    Serial.println("Fan OFF");
  }

  //Fan Control for user

  if (Firebase.getString(firebaseData, "/EFarm/HydroPhonics/Fan")) {                           // On successful Read operation, function returns 1
    int fan = firebaseData.stringData();
    Serial.println (fan);
    String fan = fan;
    fan.replace("\"", "");
    fan1 = fan.toInt();
  }

  if (fan1 == 0) {
    digitalWrite(fanpin, LOW);
    Serial.println("Fan ON");
    delay(1000);
  }
  else {
    digitalWrite(fanpin, HIGH);
    Serial.println("Fan OFF");
    delay(1000);
  }

  //Valve control for user

  if (Firebase.getString(firebaseData, "/EFarm/HydroPhonics/Valve")) {                           // On successful Read operation, function returns 1
    int valve = firebaseData.stringData();
    Serial.println (valve);
    String valve = valve;
    valve.replace("\"", "");
    valve1 = valve1.toInt();
  }

  if (valve1 == 0) {
    digitalWrite(valvepin, LOW);
    Serial.println("Valve ON");
    delay(1000);
  }
  else {
    digitalWrite(valvepin, HIGH);
    Serial.println("Valve OFF");
    delay(1000);
  }

}
