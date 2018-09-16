int switchPin = 8;
int ledPin = 13;
boolean lastButton = LOW;
boolean ledOn = false;

void setup() {
  
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

boolean debounce(boolean last){ //stabilizacia

  boolean current = digitalRead(switchPin);
  if(last != current){
   delay(5);
   current = digitalRead(switchPin);
 }
 return current;
}

void loop() {

boolean currentButton = debounce(lastButton);
 if(lastButton == LOW && currentButton == HIGH)
 {
  ledOn = !ledOn;
 }

lastButton = currentButton;
 digitalWrite(ledPin, ledOn);
  
}
