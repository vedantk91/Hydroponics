#include "DHT.h"
//here we use multiple of ESP32 to read data
#define DHTPIN1 13
#define DHTPIN2 12
#define DHTPIN3 14
#define DHTPIN4 27
//our sensor is DHT11 type
#define DHTTYPE DHT11
//create an instance of DHT sensor
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE); 


#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"


const int pump = 2;
const int valve = 4;
const int bottomfan = 16;
const int led = 17;
const int fan1 = 26;
const int fan2 = 25;
const int fan3 = 33;
const int fan4 = 32;
const int f1s = 22;
const int f1e = 1;
const int f2s = 3;
const int f2e = 21;
const int flowin = 18;
const int flowout = 19;

 int OneS = digitalRead(f1s);
 int OneE = digitalRead(f1e);
 int TwoS = digitalRead(f2s);
 int TwoE = digitalRead(f2e);

 volatile int flow_frequency1;
 unsigned int l_sec1;
 volatile int flow_frequency2;
 unsigned int l_sec2;

 void inflow () // Interrupt function
{
   flow_frequency1++;
}

 void outflow () // Interrupt function
{
   flow_frequency2++;
}

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "Honor Play_72C1";
const char* password = "josuke123";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.43.90";


// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient Kronos;
PubSubClient client(Kronos);



// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;


// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  if(topic=="Kronos/led"){
      Serial.print("Changing Kronos LEDs to ");
      if(messageTemp == "on"){
        digitalWrite(led, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(led, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();


   if(topic=="Kronos/pump"){
      Serial.print("Changing Kronos Incoming Pump to ");
      if(messageTemp == "on"){
        digitalWrite(pump, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(pump, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();


if(topic=="Kronos/valve"){
      Serial.print("Changing Kronos Outgoing Valve to ");
      if(messageTemp == "on"){
        digitalWrite(valve, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(valve, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();


if(topic=="Kronos/bfan"){
      Serial.print("Changing Kronos Bottom Fan to ");
      if(messageTemp == "on"){
        digitalWrite(bottomfan, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(bottomfan, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();



if(topic=="Kronos/fan1"){
      Serial.print("Changing Kronos First Fan to ");
      if(messageTemp == "on"){
        digitalWrite(fan1, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(fan1, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();



if(topic=="Kronos/fan2"){
      Serial.print("Changing Kronos First Fan to ");
      if(messageTemp == "on"){
        digitalWrite(fan2, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(fan2, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();



if(topic=="Kronos/fan3"){
      Serial.print("Changing Kronos First Fan to ");
      if(messageTemp == "on"){
        digitalWrite(fan3, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(fan3, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();



if(topic=="Kronos/fan4"){
      Serial.print("Changing Kronos First Fan to ");
      if(messageTemp == "on"){
        digitalWrite(fan4, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(fan4, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();


}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("Kronos/led");
      client.subscribe("Kronos/pump");
      client.subscribe("Kronos/valve");
      client.subscribe("Kronos/bfan");
      client.subscribe("Kronos/fan1");
      client.subscribe("Kronos/fan2");
      client.subscribe("Kronos/fan3");
      client.subscribe("Kronos/fan4");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(pump, OUTPUT);
  pinMode(valve, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(bottomfan, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(fan3, OUTPUT);
  pinMode(fan4, OUTPUT);
  pinMode(f1s, INPUT);
  pinMode(f1e, INPUT);
  pinMode(f2s, INPUT);
  pinMode(f2e, INPUT);
  
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();

  Serial.begin(115200);
  pinMode(flowin, INPUT);
  digitalWrite(flowin, HIGH); // Optional Internal Pull-Up
  attachInterrupt(digitalPinToInterrupt(18), inflow, RISING);
   
  pinMode(flowout, INPUT);
  digitalWrite(flowout, HIGH); // Optional Internal Pull-Up
  attachInterrupt(digitalPinToInterrupt(19), outflow, RISING);// Setup Interrupt
  
  sei(); // Enable interrupts
  
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  

}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
  
  now = millis();
  // Publishes new temperature and humidity every 35 seconds
  if (now - lastMeasure > 5000) {
    lastMeasure = now;
     
    
    float h1 = dht1.readHumidity();   
    float t1 = dht1.readTemperature();
    
    // Computes temperature values in Celsius
    float hic1 = dht1.computeHeatIndex(t1, h1, false);
    static char temperatureTemp1[7];
    dtostrf(hic1, 6, 2, temperatureTemp1);
    static char humidityTemp1[7];
    dtostrf(h1, 6, 2, humidityTemp1);

    // Publishes Temperature and Humidity values
    client.publish("Kronos/temperature1h", temperatureTemp1);
    client.publish("Kronos/humidity1h", humidityTemp1);

    if(hic1 >= 45)
    {
      digitalWrite(fan1,HIGH);
      }
    

    // END 1

    float h2 = dht2.readHumidity();   
    float t2 = dht2.readTemperature();
 
    // Computes temperature values in Celsius
    float hic2 = dht2.computeHeatIndex(t2, h2, false);
    static char temperatureTemp2[7];
    dtostrf(hic2, 6, 2, temperatureTemp2);
    static char humidityTemp2[7];
    dtostrf(h2, 6, 2, humidityTemp2);

    // Publishes Temperature and Humidity values
    client.publish("Kronos/temperature2h", temperatureTemp2);
    client.publish("Kronos/humidity2h", humidityTemp2);

     if(hic2 >= 45)
    {
      digitalWrite(fan2,HIGH);
      }

     // End 2
    

    float h3 = dht3.readHumidity();   
    float t3 = dht3.readTemperature();
  
    // Computes temperature values in Celsius
    float hic3 = dht3.computeHeatIndex(t3, h3, false);
    static char temperatureTemp3[7];
    dtostrf(hic3, 6, 2, temperatureTemp3);
    static char humidityTemp3[7];
    dtostrf(h3, 6, 2, humidityTemp3);

    // Publishes Temperature and Humidity values
    client.publish("Kronos/temperature3h", temperatureTemp3);
    client.publish("Kronos/humidity3h", humidityTemp3);

      if(hic3 >= 45)
    {
      digitalWrite(fan3,HIGH);
      }


    // End 3

    float h4 = dht4.readHumidity();   
    float t4 = dht4.readTemperature();
  
    // Computes temperature values in Celsius
    float hic4 = dht4.computeHeatIndex(t4, h4, false);
    static char temperatureTemp4[7];
    dtostrf(hic4, 6, 2, temperatureTemp4);
    static char humidityTemp4[7];
    dtostrf(h4, 6, 2, humidityTemp4);

    // Publishes Temperature and Humidity values
    client.publish("Kronos/temperature4h", temperatureTemp4);
    client.publish("Kronos/humidity4h", humidityTemp4);

     if(hic2 >= 45)
    {
      digitalWrite(fan2,HIGH);
      }


    // End 4
    
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_sec1 = (flow_frequency1 / 2.25); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency1 = 0; // Reset Counter
      Serial.print(l_sec1, DEC); // Print litres/hour
      Serial.println(" L/sec");

      static char InputFlow[7];
      dtostrf(l_sec1, 6, 2,InputFlow);

      client.publish("Kronos/InputFlow",InputFlow );

      l_sec2 = (flow_frequency2 / 2.25); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency2 = 0; // Reset Counter
      Serial.print(l_sec2, DEC); // Print litres/hour
      Serial.println(" L/sec");

      static char OutputFlow[7];
      dtostrf(l_sec2, 6, 2,OutputFlow);

      client.publish("Kronos/OutputFlow",OutputFlow );
    
    
    Serial.print("Humidity: ");
    Serial.print(h1);
    Serial.print(" %\t Temperature: ");
    Serial.print(t1);
    Serial.print(" *C ");
    //Serial.print(f1);
    //Serial.print(" *F\t Heat index: ");
    //Serial.print(hic1);
   // Serial.println(" *C ");
    
   // Serial.print("\t output = ");
    //Serial.println(outputValue);

    //Serial.print(hif);
   // Serial.println(" *F");*/
  }
} 
