 int fanpin=25;      //IN2
 int motorpin=26;    //IN4
 int valvepin=27 ;    //IN3
 
 
 void setup() {
Serial.begin(9600);
pinMode(fanpin,OUTPUT);
pinMode(motorpin,OUTPUT);
pinMode(valvepin,OUTPUT);

digitalWrite(fanpin,HIGH);      //Fan OFF
digitalWrite(motorpin,HIGH);      //motor OFF
digitalWrite(valvepin,HIGH);      //valve OFF

}

void loop() {


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
