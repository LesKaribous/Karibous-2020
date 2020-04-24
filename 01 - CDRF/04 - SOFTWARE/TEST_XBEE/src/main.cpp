#include <Arduino.h>

void setup() {
  Serial5.begin(9600);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  while(Serial5.available())
  {
    Serial5.print(Serial5.read());
  }
  Serial5.print(" | ");
  Serial.println("Yo !");
  delay(200);
  digitalWrite(13,LOW);
  delay(2000);
}


//4185D4C9 - XBEE_A
//4185C2AC - XBEE_B
