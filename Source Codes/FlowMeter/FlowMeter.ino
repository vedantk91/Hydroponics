
#define LED_BUILTIN 2
#define FlowRate  27

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

void setup()
{
  Serial.begin(115200);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis_flow = 0;
  attachInterrupt(digitalPinToInterrupt(FlowRate), pulseCounter, FALLING);
}

void flow_rate()
{
  currentMillis_flow = millis();
  if (currentMillis_flow - previousMillis_flow > flow_interval) {

    pulse1Sec = pulseCount;
    pulseCount = 0;
    int calc = 1000 / (currentMillis_flow - previousMillis_flow);
    flowRate = (calc * pulse1Sec) / calibrationFactor;
    previousMillis_flow = currentMillis_flow;

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
  }
}
