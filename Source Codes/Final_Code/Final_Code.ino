// Last edited by Deepak Hadkar, 02/08/2022

#include <U8g2lib.h>
#include <Wire.h>
#include <FirebaseESP8266.h>
#include <ESP8266httpUpdate.h> // For programming over tha air (OTA)
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>

#define PUMP_PIN D5
#define NUMPIXELS 50
#define PIN 13

String URL_fw_Bin; // Used to store URL pointing the binary file for OTA updates
String fprt_default = "70 94 DE DD E6 C4 69 48 3A 92 70 A1 48 56 78 2D 18 64 E0 B7";

String tmp_str = "";
String s1, ts, data, s2;
char* ss;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// stringPH=PH:7.16, W:22, L:  39, T:  40;
const unsigned int MAX_MESSAGE_LENGTH = 31;
float PHval, Waterlvl, Light, Temp;

byte pump_on = LOW;
uint8_t sleep_flag = 0;
int done_flag = 0;
int interval_flag = 0;
int repeat_interval_flag = 1;
int red1, green1, blue1;

int interval;
int repeat_interval;

unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long currentMillis;
unsigned long currentMillis1;
unsigned long currentMillis2;
unsigned long sleep = 60000;

#define FIREBASE_HOST "https://e-farm-d2031-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "SgAgOkkygVXy9YSGEvTfzI7Sw7iHxliB7XUl4Hzz"

FirebaseData firebaseData1;
FirebaseJson json;

WiFiManager wm;

void setup()
{
  // start the serial connection
  Serial.begin(9600);
  u8g2.begin();

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(0, INPUT);

  digitalWrite(PUMP_PIN, LOW);

  // FastLED
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  for (int i = 0; i < NUMPIXELS; i++) // For each pixel...
  {
    pixels.setPixelColor(i, pixels.Color(0, 125, 125));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }

  // Print on OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(0, 15, "EFarm KJSCE!");
  u8g2.drawStr(0, 50, "HydroMini");
  u8g2.sendBuffer();


  wm.autoConnect("HydroMini");
  wm.setConfigPortalTimeout(60);
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
  wifi_control();  // Wifi handling

  boot_reset();  // wm.rest button

  updatecode(); // OTA

  if (Serial.available() > 0)
  {
    // Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    // Read the next available byte in the serial receive buffer

    String inByte = Serial.readString();

    inByte.trim();  // Whole Msg
    Serial.println(inByte);  // Cut the String

    PHval = (inByte.substring(3, 8)).toFloat();
    Waterlvl = (inByte.substring(11, 13)).toFloat();
    Light = (inByte.substring(18, 22)).toFloat();
    Temp = (inByte.substring(28, 30)).toFloat();

    String tp = String(Temp);
    String ph = String(PHval);
    String wl = String(Waterlvl);
    String li = String(Light);

    // Send data to Firebase

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
    u8g2.sendBuffer();
  }

  // Fast LED

  if (Firebase.getString(firebaseData1, "/HydroMini/Red"))
  { // On successful Read operation, function returns 1
    String red = firebaseData1.stringData();
    red1 = red.toInt();
  }
  if (Firebase.getString(firebaseData1, "/HydroMini/Blue"))
  { // On successful Read operation, function returns 1
    String blue = firebaseData1.stringData();
    blue1 = blue.toInt();
  }
  if (Firebase.getString(firebaseData1, "/HydroMini/Green"))
  { // On successful Read operation, function returns 1
    String green = firebaseData1.stringData();
    green1 = green.toInt();
  }

  for (int i = 0; i < NUMPIXELS; i++) // For each pixel...
  {
    pixels.setPixelColor(i, pixels.Color(green1, red1, blue1));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }

  // Interval control

  if (Firebase.getString(firebaseData1, "/HydroMini/On_Interval"))
  { // On successful Read operation, function returns 1
    String value = firebaseData1.stringData();
    interval = value.toInt(); //intValue
  }
  Serial.print("Interval ");
  Serial.println(interval);

  if (Firebase.getString(firebaseData1, "/HydroMini/Off_Interval"))
  { // On successful Read operation, function returns 1
    String value1 = firebaseData1.stringData();
    repeat_interval = value1.toInt();
  }

  // Pump time control

  currentMillis = millis();
  if ((currentMillis - previousMillis >= interval) && repeat_interval_flag == 1) // 1 hrs
  {
    previousMillis = currentMillis;
    Serial.println("Interval MILLI");
    if (pump_on == LOW)
    {
      pump_on = HIGH;
      done_flag = 1;
    }
    else
    {
      pump_on = LOW;
      done_flag = 0;
    }
    interval_flag = 1;
    repeat_interval_flag = 0;
    previousMillis1 = currentMillis1;
  }

  currentMillis1 = millis();
  if ((currentMillis1 - previousMillis1 >= repeat_interval) && interval_flag == 1) // 5 min
  {
    previousMillis1 = currentMillis1;
    Serial.println("Repeat MILLI");
    if (pump_on == LOW)
    {
      pump_on = HIGH;
      done_flag = 1;
    }
    else
    {
      pump_on = LOW;
      done_flag = 0;
    }
    repeat_interval_flag = 1;
    interval_flag = 0;
    previousMillis = currentMillis;
  }

  if (pump_on == LOW && done_flag == 0)
  {
    digitalWrite(PUMP_PIN, LOW); // AirPump ON
    Serial.print("Pump ON  ");
    Serial.println(PUMP_PIN);
    Firebase.setString(firebaseData1, "/HydroMini/Airflow", "0");
    done_flag = 1;
  }
  else if (pump_on == HIGH && done_flag == 1)
  {
    digitalWrite(PUMP_PIN, HIGH); // AirPump OFF
    Serial.print("Pump OFF  ");
    Serial.println(PUMP_PIN);
    Firebase.setString(firebaseData1, "/HydroMini/Airflow", "1");
    done_flag = 0;
  }


  // When Off
}

void wifi_control() {
  if ((WiFi.status() != WL_CONNECTED) && sleep_flag == 0)
  {
    //Turn off WiFi
    Firebase.setString(firebaseData1, "/HydroMini/Wifi_Status", "Sleep");
    WiFi.forceSleepBegin();
    Serial.print("Wifi Off...!!!");
    sleep_flag = 1;
  }
  currentMillis2 = millis();
  if ((currentMillis2 - previousMillis2 >= sleep) && sleep_flag == 1) // 1 min
  {
    previousMillis2 = currentMillis2;

    WiFi.forceSleepWake();
    Serial.println("Wifi Wakeup !");
    delay(10000);
    
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
    Firebase.setString(firebaseData1, "/HydroMini/Wifi_Status", "Wake");
  }
  sleep_flag = 0;
  }
}
void boot_reset() {
  if (digitalRead(0) == LOW)
  {
    delay(200);
    if (digitalRead(0) == LOW)
    {
      Serial.println("Reset ESP");
      wm.resetSettings();
      ESP.restart();
    }
  }
}

void updatecode() {
  // Flash new code from URL
  String otaurlsize = read_String(25, 2);
  int urlsize = otaurlsize.toInt();
  String otaurll = read_String(30, urlsize + 10);
  if (otaurll.length() > 10) {
    URL_fw_Bin = otaurll;
    Serial.println("Updating code from:\t:");
    Serial.println(URL_fw_Bin);
    Serial.println("Initiating New Firmware Update");
    for (int i = 0; i <= 9; i++) {
      Serial.print(".");
      delay(300);
    }
    Serial.println("");
    WiFiClient Client;
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

    // Add optional callback notifiers
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);
    if (otaurll.charAt(4) == 's') {
      String fprt = read_String(150, 59);
      Serial.print("Fimrware HTTPS URL:\t");
      Serial.println(URL_fw_Bin);
      Serial.print("Fingerprint:\t");
      Serial.println(fprt);
      t_httpUpdate_return rett = ESPhttpUpdate.update(URL_fw_Bin, "", fprt);
      switchstatements(rett);
    }
    else {
      Serial.print("Fimrware HTTP URL:\t");
      Serial.println(URL_fw_Bin);
      t_httpUpdate_return rett = ESPhttpUpdate.update(Client, URL_fw_Bin);
      switchstatements(rett);
    }
  }
  else {
    Serial.println("URL not correct, Code not uploaded");
  }
}

void switchstatements(HTTPUpdateResult ret) {
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}
String read_String(int addrr, int readlen)
{
  // Function to read string from memory
  int i = 0;
  char datas[99];
  int len = 0;
  unsigned char k;
  k = EEPROM.read(addrr);
  while (i < readlen)
  {
    k = EEPROM.read(addrr + len);
    datas[len] = k;
    Serial.print("IN read_String loop: \t");
    Serial.println(datas[len]);
    len++;
    i++;
    delay(50);
  }
  datas[len] = '\0';
  Serial.println(len);

  return String(datas);
}
void writeString(int addrw, String data)
{
  // Function to write string to memory
  Serial.println("Entering writeString");
  int _size = data.length();
  int i;
  for (i = 0; i < _size; i++)
  {
    EEPROM.write(addrw + i, data[i]);
    delay(10);
  }
  EEPROM.write(addrw + _size, '\0');
  delay(20);
  EEPROM.commit();
  Serial.println("Exiting writeString");
}

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}
