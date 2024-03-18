#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
#include <FirebaseESP32.h>
#include  <WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#define PUMP_PIN D5
#define NUM_LEDS 50
#define DATA_PIN 13
#include <FastLED.h>
CRGB leds[NUM_LEDS];


//stringPH=PH:7.16, W:22, L:  39, T:  40;
const unsigned int MAX_MESSAGE_LENGTH = 31;
float PHval, Waterlvl, Light, Temp;
int Red, Green, Blue;





#define FIREBASE_HOST "https://e-farm-d2031-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "SgAgOkkygVXy9YSGEvTfzI7Sw7iHxliB7XUl4Hzz"

FirebaseData firebaseData1;
FirebaseJson json;

void setup() {

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  //FastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  // start the serial connection

  Serial.begin(9600);
  u8g2.begin();

  WiFiManager wifiManager;
  wifiManager.autoConnect("HydroMini");

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

    //Send data to Firebase

    Firebase.setFloat(firebaseData1, "/EFarm/HydroMini/ph", PHval);
    Firebase.setFloat(firebaseData1, "/EFarm/HydroMini/waterlevel", Waterlvl);
    Firebase.setFloat(firebaseData1, "/EFarm/HydroMini/light", Light);
    Firebase.setFloat(firebaseData1, "/EFarm/HydroMini/temp", Temp);

    //Print on OLED

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.drawStr(0, 15, "EFarm KJSCE!");
    u8g2.drawStr(0, 30, "PH=");
    u8g2.drawStr(40, 30, ph.c_str());
    u8g2.drawStr(0, 50, "WaterLvl=");
    u8g2.drawStr(95, 50, wl.c_str());
    u8g2.sendBuffer();
  }

  //FastLED


  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(Green, Red, Blue);
  }
  FastLED.show();

  // Pump control

  if (Firebase.getString(firebaseData1, "/EFarm/HydroMini/Airflow")) {                           // On successful Read operation, function returns 1
    int valve = firebaseData1.stringData();
    Serial.println (valve);
    String valve = valve;
    valve.replace("\"", "");
    valve1 = valve.toInt();
  }

  if (valve1 == 0) {
    digitalWrite(PUMP_PIN, LOW);      //valve ON
    Serial.println("Pump ON");
    delay(1000);
  }
  else {
    digitalWrite(PUMP_PIN, HIGH);     //valve OFF
    Serial.println("Pump OFF");
    delay(1000);
  }

}
