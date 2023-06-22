  #include <stdio.h>
  #include <stdlib.h>
  #include <IRremote.h>
  #include <TimeOut.h>

  void callback0();

  int RECV_PIN = 12; // Infrared receiving pin
  IRrecv irrecv(RECV_PIN); // Create a class object used to receive class
  decode_results results; // Create a decoding results class object
  TimeOut timeout0; 

  const int ledCount = 10; // the number of LEDs in the bar graph
  // an array of pin numbers to which LEDs are attached
  int ledPins[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  int lightLevel;
  bool luxLock = false;
  char Port1;
  char currMode = 'd';
  char ledState;
 
 // currMode settings || 'a' = LEDS ON, 'b' = Enable motion sensors, 'd'= Normal Settings
 // ledState settings || 'a' = BotToTop triggered, 'b' = TopToBot triggered, 'd' = lights OFF

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  // loop over the pin array and set them all to output:
  //pinMode(12, OUTPUT);
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  //Infrared Remote
  if (irrecv.decode(&results)) { // Waiting for decoding
    Serial.println(results.value, HEX); // Print out the decoded results
    irrecv.resume(); // Receive the next value
  }
 delay(100);
}

void loop() {
  timeout0.handler();  
  lightLevel = analogRead(A0); // read from A0
  Serial.print("Light Level: ");
  Serial.println(lightLevel); 

  if(Serial.available()){
    Port1 = Serial.read();
    //debugPin(12); //Send 1 on serial to turn on I/O Pin
    currMode = Port1;
  }

  if (irrecv.decode(&results)) { // Waiting for decoding
    Serial.println(results.value, HEX); // Print out the decoded results
    remotePress(results.value);
    irrecv.resume(); // Receive the next value
  }
  delay(1500);

   if ((lightLevel > 800) && (!luxLock) || currMode == 'b'){
    //if (currMode == 'a'){  //Replace with bottom Sensor high
      bottomtoTop(500); //Lights turn on this way
      delay(3000);
      ledOFF();
    //}
    if (currMode == 'b'){ //Replace with top Sensor high
      toptoBottom(500); //Lights turn on this way
      delay(3000);
      ledOFF();
    }
  }
  Serial.print("Current Mode: ");
  Serial.println(currMode);
}

void bottomtoTop(int speed) {
  ledState = 'b';
 // All LEDs ON in a waterfall
 for (int i = 0; i < ledCount; i++) {
    delay(speed);
    digitalWrite(ledPins[i], HIGH);
 }
}

void toptoBottom(int speed) {
  ledState = 'c';
 // All LEDs on in a waterfall
 for (int i = ledCount; i > 0 ; i--) {
    delay(speed);
    digitalWrite(ledPins[i], HIGH);
 }
}

void ledOFF() {
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  ledState = 'd';
}

void remotePress(unsigned long value){
  switch (value) {
    case 0xFFA25D: // Receive the power button
      switchModes();
    break;

    case 0xFF30CF: // Receive the number '1'
      currMode = 'e';
      luxLock = true;
      timeout0.cancel();
      timeout0.timeOut(10000, callback0);
    break;
    // case 0xFF18E7: // Receive the number '2'
    //   analogWrite(ledPin, 63); // Medium brightness
    // break;
    // case 0xFF7A85: // Receive the number '3'
    //   analogWrite(ledPin, 255); // Strongest brightness
    // break;
    default:
      Serial.println("Error with Remote");
  }
  
}

void switchModes(){
  if(ledState == 'd'){
      bottomtoTop(180);
    }
    else{
      ledOFF();
      timeout0.cancel();
    }
  
}

void callback0(){
  ledOFF();
  luxLock = false;
}
void debugPin(int pin){
  
  if(Port1 == '1'){
    digitalWrite(pin, HIGH);
  }  
  else{
      digitalWrite(pin, LOW);
    }
  Serial.print("Serial Output: "); // print the string "received:"
  Serial.println(Port1); // print the received character
}
