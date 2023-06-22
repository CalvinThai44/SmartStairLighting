  #include <stdio.h>
  #include <stdlib.h>
  const int ledCount = 10; // the number of LEDs in the bar graph
  // an array of pin numbers to which LEDs are attached
  int ledPins[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  int lightLevel;
  char Port1;
  int value;

void setup() {
  Serial.begin(9600);
  // loop over the pin array and set them all to output:
  pinMode(12, OUTPUT);
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
    
      lightLevel = analogRead(A0); // read from A0
      Serial.print("Light Level: ");
      Serial.println(lightLevel); 

    if(Serial.available()){
      debugPin(12); //Send 1 on serial to turn on I/O Pin
    }
 // the ith LED of LED bar graph will light up in turn
  if (lightLevel > 640){ // if the button is not pressed
    bottomtoTop();
  }
  delay(1000);
  
}

void bottomtoTop() {
 // All LEDs on in a waterfall
 for (int i = 0; i < ledCount; i++) {
    delay(500);
    digitalWrite(ledPins[i], HIGH);
 }
  delay(1000);
  ledOFF();
}

void toptoBottom() {
 // All LEDs on in a waterfall
 for (int i = 0; i < ledCount; i++) {
    delay(500);
    digitalWrite(ledPins[i], HIGH);
 }
  delay(1000);
  ledOFF();
}

void ledOFF() {
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  
}
void debugPin(int pin){
  Port1 = Serial.read();
  if(Port1 == '1'){
    digitalWrite(pin, HIGH);
  }  
  else{
      digitalWrite(pin, LOW);
    }
  Serial.print("Serial Output: "); // print the string "received:"
  Serial.println(Port1); // print the received character
}
