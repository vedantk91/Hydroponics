// Last edited by Deepak Hadkar, 01/11/2022, 09:25 am

#include <FirebaseESP32.h>
#include <WiFiManager.h>
WiFiManager wm;
#include <Adafruit_NeoPixel.h>
#include <DHT.h>

#define NUMPIXELS 60
#define fastled_pin 33

#define levelPin 34
#define dht1_pin 2
#define dht2_pin 4
#define flowrate_pin 35 //

#define fanpin 27   // IN2
#define valvepin 25 // IN3
#define motorpin 26 // IN4

#define DHTTYPE DHT11
DHT DHT1(dht1_pin, DHTTYPE);
DHT DHT2(dht2_pin, DHTTYPE);

Adafruit_NeoPixel pixels(NUMPIXELS, fastled_pin, NEO_GRB + NEO_KHZ800);

uint8_t sleep_flag, valve_on, motor_on, fan_on = 0;  //Status flag
uint8_t red1, green1, blue1;
uint8_t levelState = 0, valve_cycle;

unsigned long currentMillis_valve;
unsigned long previousMillis_valve = 0;
unsigned long long_cycle;

unsigned long currentMillis2;
unsigned long previousMillis2 = 0;

//FloatSensor
uint8_t countersen = 0;
uint8_t Totalsen = 0;
uint8_t rawlevel = 0;

// Flow rate Initialize
unsigned long currentMillis_flow = 0;
unsigned long previousMillis_flow = 0;
int flow_interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

// DHT
uint8_t t1, h1, t2, h2;
int tempTotal, humidTotal, tempAvg, humidAvg, counterval, Fan_on = 0;

#define FIREBASE_HOST "https://e-farm-d2031-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "SgAgOkkygVXy9YSGEvTfzI7Sw7iHxliB7XUl4Hzz"

FirebaseData firebaseData1;

void setup()
{
  Serial.begin(115200);
  Serial.println("HydroPhonics v1.5, last updated 01/11/2022, 09:25 am ");
  Serial.println();

  pinMode(levelPin, INPUT_PULLUP);
  pinMode(flowrate_pin, INPUT_PULLUP);

  pinMode(fanpin, OUTPUT);
  pinMode(motorpin, OUTPUT);
  pinMode(valvepin, OUTPUT);


  digitalWrite(fanpin, HIGH); // Fan OFF
  digitalWrite(motorpin, HIGH); // Motor OFF
  digitalWrite(valvepin, HIGH); // valve OFF

  // FastLED
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  for (int i = 0; i < NUMPIXELS; i++) // For each pixel...
  {
    pixels.setPixelColor(i, pixels.Color(255, 255, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }


  // DHT Sensor 1
  DHT1.begin();
  Serial.print("Temp1:"); Serial.print(DHT1.readTemperature());
  Serial.print("  Humid1:"); Serial.println(DHT1.readHumidity());

  // DHT Sensor 2
  DHT2.begin();
  Serial.print("Temp2:"); Serial.print(DHT2.readTemperature());
  Serial.print("  Humid2:"); Serial.println(DHT2.readHumidity());

  // Flow Rate sensor setup
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis_flow = 0;
  attachInterrupt(digitalPinToInterrupt(flowrate_pin), pulseCounter, FALLING);

  // Wifi Manager
  wm.autoConnect("HydroPonics");
  wm.setConfigPortalTimeout(60);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // Firebase
  delay(500);

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
  fastled_control();
  fan_control();
  floatSensor();
  valve_funct();
  flow_rate();

  wifi_control();
}

void fastled_control()
{
  // ###################### Fast LED ######################
  if ((WiFi.status() != WL_CONNECTED))
  {
    for (int i = 0; i < NUMPIXELS; i++) // For each pixel...
    {
      pixels.setPixelColor(i, pixels.Color(255, 255, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
    Serial.println("FastLED - No Internet Connection!!!");
  }
  else
  {
    firebase_colour();
    Serial.println("FastLED colours updated by firebase!....");

    for (int i = 0; i < NUMPIXELS; i++) // For each pixel...
    {
      pixels.setPixelColor(i, pixels.Color(red1, blue1, green1));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
    delay(500);
  }
}


void fan_control()
{
  // ###################### FAN Logic ######################

  if ((WiFi.status() != WL_CONNECTED))
  {
    digitalWrite(fanpin, LOW);
    Serial.println("Fan ON - No Internet Connection!!!");
  }
  else
  {
    tempTotal = 0;
    tempAvg = 0;
    humidTotal = 0;
    humidAvg = 0;
    counterval = 0;

    for (uint8_t i = 0; i < 10; i++)
    {
      t1 = DHT1.readTemperature(); h1 = DHT1.readHumidity();
      t2 = DHT2.readTemperature(); h2 = DHT2.readHumidity();

      if (isnan(t1)) {
        Serial.println(F("Failed to read from DHT1 sensor!"));
        return;
      }
      if (isnan(t2)) {
        Serial.println(F("Failed to read from DHT2 sensor!"));
        return;
      }

      if (t1 > 15 && t2 > 15)
      {
        tempTotal += ((t1 + t2) / 2);
        humidTotal += ((h1 + h2) / 2);
        counterval++;
      }
    }
    if (counterval == 0)
    {
      Serial.println("Failed to read Temperature greater than 15!");
    }
    else
    {
      tempAvg = tempTotal / counterval;
      humidAvg = humidTotal / counterval;
    }
    Serial.print("Temp:"); Serial.print(tempAvg);
    Serial.print("  Humid:"); Serial.println(humidAvg);

    Firebase.setString(firebaseData1, "/HydroPhonics/temp", tempAvg);
    Firebase.setString(firebaseData1, "/HydroPhonics/humid", humidAvg);
    delay(100);

    if (tempAvg < 25 && Fan_on == 1)
    {
      digitalWrite(fanpin, HIGH);
      Serial.println("Fan OFF");
      //    delay(500);
      fan_on = 0;
      Firebase.setString(firebaseData1, "/HydroPhonics/Relay/Fan", fan_on);
      delay(100);
      Fan_on = 0;
    }
    else if (tempAvg >= 25 && Fan_on == 0)
    {
      digitalWrite(fanpin, LOW);
      Serial.println("Fan ON");
      //    delay(500);
      fan_on = 1;
      Firebase.setString(firebaseData1, "/HydroPhonics/Relay/Fan", fan_on);
      delay(100);
      Fan_on = 1;
    }
  }
}


void valve_funct()
{
  /* Declaration:
    uint8_t valve_cycle, levelState;
    unsigned long long_cycle;
    unsigned long currentMillis_valve;
    unsigned long previousMillis_valve = 0;
  */
  // ###################### Valve ON/OFF ######################

  if ((WiFi.status() != WL_CONNECTED))
  {
    long_cycle = 600000;
    Serial.println("Valve ON/OFF - No Internet Connection!!!");
  }
  else
  {
    if (Firebase.getString(firebaseData1, "/HydroPhonics/Cycle"))
    { // On successful Read operation, function returns 1
      String cycle = firebaseData1.stringData();
      valve_cycle = cycle.toInt();
      long_cycle = valve_cycle * 60000;
    }
  }

  Firebase.setString(firebaseData1, "/HydroPhonics/Level", levelState);
  Serial.print("Water Level: "); Serial.println(levelState);
  //  delay(1000);
  currentMillis_valve = millis();
  if ((currentMillis_valve - previousMillis_valve >= long_cycle) || levelState == 0) // Call after every 10 min
  {
    previousMillis_valve = currentMillis_valve;
    //    Serial.print("Level Millis: "); Serial.println(levelState);
    //    delay(500);
    if (levelState == 0)
    {
      digitalWrite(motorpin, HIGH); // motor OFF
      Serial.println("Motor OFF");
      digitalWrite(valvepin, LOW); // valve ON
      Serial.println("Valve ON");
      Serial.println("Full");

      valve_on = 1;
      motor_on = 0;

      Firebase.setString(firebaseData1, "/HydroPhonics/Level", levelState);
      Firebase.setString(firebaseData1, "/HydroPhonics/Relay/Motor", motor_on);
      Firebase.setString(firebaseData1, "/HydroPhonics/Relay/Valve", valve_on);
      delay(100);
    }
    else
    {
      digitalWrite(motorpin, LOW); // motor ON
      Serial.println("Motor ON");
      digitalWrite(valvepin, HIGH); // valve OFF
      Serial.println("Valve OFF");
      Serial.println("Empty");

      valve_on = 0;
      motor_on = 1;

      Firebase.setString(firebaseData1, "/HydroPhonics/Level", levelState);
      Firebase.setString(firebaseData1, "/HydroPhonics/Relay/Motor", motor_on);
      Firebase.setString(firebaseData1, "/HydroPhonics/Relay/Valve", valve_on);
      delay(100);
    }
  }

}

void flow_rate()
{
  //##################### FlowRate sensor #####################

  currentMillis_flow = millis();
  if (currentMillis_flow - previousMillis_flow > flow_interval)
  {

    pulse1Sec = pulseCount;
    pulseCount = 0;

    flowRate = ((1000.0 / (millis() - previousMillis_flow)) * pulse1Sec) / calibrationFactor;
    previousMillis_flow = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate)); // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t"); // Print tab space
    Firebase.setString(firebaseData1, "/HydroPhonics/FlowSensor/LperMin", flowRate);

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
    Firebase.setString(firebaseData1, "/HydroPhonics/FlowSensor/mLper", totalMilliLitres);
    Firebase.setString(firebaseData1, "/HydroPhonics/FlowSensor/perL", totalMilliLitres / 1000);
  }
}

void wifi_control()
{
  //##################### Stop unnecessary Wifi searching! #####################

  if ((WiFi.status() != WL_CONNECTED) && sleep_flag == 0)
  {
    //Turn OFF WiFi
    Firebase.setString(firebaseData1, "/HydroPhonics/Wifi_Status", "Sleep");
    //    WiFi.forceSleepBegin();    //ESP8266
    esp_wifi_stop();    //ESP32
    Serial.print("Wifi Off...!!!");
    sleep_flag = 1;
  }
  currentMillis2 = millis();
  if ((currentMillis2 - previousMillis2 >= 180000) && sleep_flag == 1) // 3 min
  {
    //Turn ON WiFi
    previousMillis2 = currentMillis2;
    //    WiFi.forceSleepWake();    //ESP8266
    esp_wifi_start();    //ESP32
    Serial.println("Wifi Wakeup !");

    WiFi.reconnect();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    delay(5000);

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
      Firebase.setString(firebaseData1, "/HydroPhonics/Wifi_Status", "Wake");
    }
    sleep_flag = 0;

  }
}

void floatSensor()
{
  countersen = 0;
  Totalsen = 0;

  for (uint8_t x = 0; x < 100; x++)
  {
    rawlevel = digitalRead(levelPin);
    //    Serial.print("Level data: "); Serial.println(rawlevel);
    if (rawlevel == 0)
    {
      countersen++;
    }
  }

  //  Serial.print("Level count(0): "); Serial.println(countersen);
  if (countersen >= 100) {
    levelState = 0;
  }
  else {
    levelState = 1;
  }

  delay(200);
}

void firebase_colour()
{
  if (Firebase.getString(firebaseData1, "/HydroPhonics/FastLED/Red"))
  { // On successful Read operation, function returns 1
    String red = firebaseData1.stringData();
    red1 = red.toInt();
  }
  //  return red1;

  if (Firebase.getString(firebaseData1, "/HydroPhonics/FastLED/Blue"))
  { // On successful Read operation, function returns 1
    String blue = firebaseData1.stringData();
    blue1 = blue.toInt();
  }
  //  return blue1;

  if (Firebase.getString(firebaseData1, "/HydroPhonics/FastLED/Green"))
  { // On successful Read operation, function returns 1
    String green = firebaseData1.stringData();
    green1 = green.toInt();
  }
  //  return green1;
  delay(200);
}
