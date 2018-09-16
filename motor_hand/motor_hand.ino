int in1 = 4;
int in2 = 5;
int in3 = 6;
int in4 = 7;
const int dl = 5;
boolean beginPoint = false;
int switchPin = 8;
boolean lastButton = LOW;

void setup() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);   
    pinMode(switchPin, INPUT);
}

void loop() {

boolean currentButton = debounce(lastButton);
if(!currentButton == HIGH && !beginPoint){
 againstClockwise();
}
else{
  beginPoint = true;
}
lastButton = currentButton;
}



boolean debounce(boolean last){ //stabilizacia

  boolean current = digitalRead(switchPin);
  if(last != current){
   delay(5);
   current = digitalRead(switchPin);
 }
 return current;
}

void againstClockwise(){
    digitalWrite( in1, HIGH ); 
    digitalWrite( in2, LOW ); 
    digitalWrite( in3, LOW ); 
    digitalWrite( in4, HIGH );
    delay(dl);
    
    digitalWrite( in1, LOW ); 
    digitalWrite( in2, LOW ); 
    digitalWrite( in3, HIGH ); 
    digitalWrite( in4, HIGH );
    delay(dl);

    digitalWrite( in1, LOW ); 
    digitalWrite( in2, HIGH ); 
    digitalWrite( in3, HIGH ); 
    digitalWrite( in4, LOW );
    delay(dl);
    
    digitalWrite( in1, HIGH ); 
    digitalWrite( in2, HIGH ); 
    digitalWrite( in3, LOW ); 
    digitalWrite( in4, LOW );
    delay(dl);
}

void inClockwise(){

    digitalWrite( in1, HIGH ); 
    digitalWrite( in2, HIGH ); 
    digitalWrite( in3, LOW ); 
    digitalWrite( in4, LOW );
    delay(dl);

    digitalWrite( in1, LOW ); 
    digitalWrite( in2, HIGH ); 
    digitalWrite( in3, HIGH ); 
    digitalWrite( in4, LOW );
    delay(dl);

    digitalWrite( in1, LOW ); 
    digitalWrite( in2, LOW ); 
    digitalWrite( in3, HIGH ); 
    digitalWrite( in4, HIGH );
    delay(dl);
    
    digitalWrite( in1, HIGH ); 
    digitalWrite( in2, LOW ); 
    digitalWrite( in3, LOW ); 
    digitalWrite( in4, HIGH );
    delay(dl);
    
}

