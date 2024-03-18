#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <FirebaseESP8266.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <FastLED.h>

#define PUMP_PIN D5
#define NUM_LEDS 50
#define DATA_PIN 13

CRGB leds[NUM_LEDS];

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// stringPH=PH:7.16, W:22, L:  39, T:  40;
const unsigned int MAX_MESSAGE_LENGTH = 31;
float PHval, Waterlvl, Light, Temp;
int Red, Green, Blue;
int pump_on = 0;  //Flags
int airflow = 0;

unsigned long previousMillis = 0; // last time update
unsigned long currentMillis;
long interval;                    // interval received from Firebase
long time_interval;               // Fetched from EEPROM

#define FIREBASE_HOST "https://e-farm-d2031-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "SgAgOkkygVXy9YSGEvTfzI7Sw7iHxliB7XUl4Hzz"

FirebaseData firebaseData1;
FirebaseJson json;

void setup()
{
  // start the serial connection
  Serial.begin(9600);
  u8g2.begin();

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  // FastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed

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
  if (Serial.available() > 0)
  {
    // Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    // Read the next available byte in the serial receive buffer

    String inByte = Serial.readString();

    inByte.trim();

    // Whole Msg

    Serial.println(inByte);

    // Cut the String

    PHval = (inByte.substring(3, 8)).toFloat();
    Waterlvl = (inByte.substring(11, 13)).toFloat();
    Light = (inByte.substring(18, 22)).toFloat();
    Temp = (inByte.substring(28, 30)).toFloat();
  }

  // Print individual values

  Serial.println(PHval);
  Serial.println(Waterlvl);
  Serial.println(Light);
  Serial.println(Temp);

  String tp = String(Temp);
  String ph = String(PHval);
  String wl = String(Waterlvl);
  String li = String(Light);

  // Send data to Firebase

  Firebase.setFloat(firebaseData1, "/HydroMini/ph", PHval);
  Firebase.setFloat(firebaseData1, "/HydroMini/waterlevel", Waterlvl);
  Firebase.setFloat(firebaseData1, "/HydroMini/light", Light);
  Firebase.setFloat(firebaseData1, "/HydroMini/temp", Temp);
  Firebase.setFloat(firebaseData1, "/HydroMini/ph", PHval);
  Firebase.setFloat(firebaseData1, "/HydroMini/waterlevel", Waterlvl);
  Firebase.setFloat(firebaseData1, "/HydroMini/light", Light);
  Firebase.setFloat(firebaseData1, "/HydroMini/temp", Temp);

  // Print on OLED

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(0, 15, "EFarm KJSCE!");
  u8g2.drawStr(0, 30, "PH=");
  u8g2.drawStr(40, 30, ph.c_str());
  u8g2.drawStr(0, 50, "WaterLvl=");
  u8g2.drawStr(95, 50, wl.c_str());
  u8g2.sendBuffer(); // Print on OLED

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(0, 15, "EFarm KJSCE!");
  u8g2.drawStr(0, 30, "PH=");
  u8g2.drawStr(40, 30, ph.c_str());
  u8g2.drawStr(0, 50, "WaterLvl=");
  u8g2.drawStr(95, 50, wl.c_str());
  u8g2.sendBuffer();

  // Fast LED

  for (int i = 0; i < NUM_LEDS; i++)
  {
    Serial.print(i);
    leds[i] = CRGB(0, 125, 125);
  }
  FastLED.show();
  delay(1000);

  // Interval control

  if (Firebase.getString(firebaseData1, "/HydroMini/Interval"))
  { // On successful Read operation, function returns 1
    String value = firebaseData1.stringData();
    Serial.println(value);
    interval = value.toInt();
  }
  Serial.print("Interval ");
  Serial.println(interval);

  // Pump time control

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) //60
  {
    previousMillis = currentMillis;
    airflow = !airflow;
  }
  if (airflow == 0 && pump_on == 0)
  {
    digitalWrite(PUMP_PIN, LOW); // AirPump ON
    Serial.print("Pump ON  ");
    Serial.println(PUMP_PIN);
    Firebase.setString(firebaseData1, "/HydroMini/Airflow", "0");
    pump_on = 1;
  }
  else if (airflow == 1 && pump_on == 1)
  {
    digitalWrite(PUMP_PIN, HIGH); // AirPump OFF
    Serial.print("Pump OFF  ");
    Serial.println(PUMP_PIN);
    Firebase.setString(firebaseData1, "/HydroMini/Airflow", "1");
    pump_on = 0;
  }


}
