  int fanpin=25;
  int motorpin=26;
  int valvepin=27;
  
  void setup() {
Serial.begin(9600);
pinMode(fanpin,OUTPUT);
pinMode(motorpin,OUTPUT);
pinMode(valvepin,OUTPUT);

}

void loop() {
 
 digitalWrite(fanpin,HIGH);
 Serial.print("Fan ON");
 Serial.print("  Motor OFF");
 Serial.println("  Valve OFF");
 delay(5000);
 digitalWrite(motorpin,HIGH);
 Serial.print("Fan ON");
 Serial.print("  Motor ON");
 Serial.println("  Valve OFF");
 delay(4000);
 digitalWrite(motorpin,LOW);
 Serial.print("Fan ON");
 Serial.print(" Motor OFF");
 Serial.println("  Valve OFF");
 delay(1000);
 digitalWrite(valvepin,HIGH);
 Serial.print("Fan ON");
 Serial.print("  Motor OFF");
 Serial.println("  Valve ON");
 delay(10000);
 digitalWrite(valvepin,LOW);
 Serial.print("Fan ON");
 Serial.print("  Motor OFF");
 Serial.println("  Valve OFF");
 digitalWrite(fanpin,LOW);
 Serial.print("Fan OFF");
 Serial.print("  Motor OFF");
 Serial.println("  Valve OFF");
 Serial.println("");
 Serial.println("End of Loop");
 

 

}
