#include <SoftwareSerial.h>
#include <Nextion.h>
#include "NexButton.h"
#include "NexText.h"


//motor pins
int in1 = 4;
int in2 = 5;
int in3 = 6;
int in4 = 7;

const int dl = 5;
boolean beginPoint = false;
float currentPosition;  
float distance;
float radius = 2;

int switchPin = 8;               //button input
boolean lastButton = LOW;

// Nextion elements
NexButton startButton2 = NexButton(2, 6, "startButton"); // where 2 - page, 6 - id, startButton -objname
NexButton startButton3 = NexButton(3, 6, "startButton");
NexButton b0 = NexButton(0, 1, "b0");
NexText distanceText = NexText(0, 3, "distanceText");

// Clickable elements
NexTouch *nex_listen_list[] = 
{
  &b0,
  &startButton2,
  &startButton3,
  NULL
};


// Callback for specific button
void startButtonPopCallback(void *ptr)
{
  move(20);
}

// Callback for specific button
void startButton3PopCallback(void *ptr)
{
  move(30);
}


// Callback for go to 0 button
void b0ButtonPopCallback(void *ptr)
{
  goToNull();
}

void setup()
{      
  Serial.begin(9600);
  nexInit();
  
  startButton2.attachPop(startButtonPopCallback, &startButton2);
  startButton3.attachPop(startButton3PopCallback, &startButton3);
  b0.attachPop(b0ButtonPopCallback, &b0);
  
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);   
    pinMode(switchPin, INPUT);

//    goToNull(); 
}

void loop()
{
  nexLoop(nex_listen_list);   
}


void move(float distance)
{
  float count;
  int i;

  distanceText.setText("wait...");
  
    if(currentPosition < distance)
    {
      distance = distance - currentPosition;
      count = 180 * distance / (M_PI * radius * 0.703);
      for(int i = 0; i <= count; i++)
        inClockwise();
      currentPosition += distance;
    }
    else if (currentPosition > distance)
    {
      distance = currentPosition - distance;
      count = 180 * distance / (M_PI * radius * 0.703);
      for(int i = 0; i <= count; i++)
        againstClockwise();
      currentPosition -= distance;
    }
    
  beginPoint = false;

  String curPos2 =  String(currentPosition, 2);  
  distanceText.setText(curPos2.c_str());
}


void goToNull()
{
  
  while(!beginPoint)
  {
      boolean currentButton = debounce(lastButton);
      
      if(!currentButton == HIGH){  againstClockwise(); }
      else
      { 
        beginPoint = true;
        distanceText.setText("0");
        currentPosition = 0;
      }
      
      lastButton = currentButton;
   }
}



boolean debounce(boolean last)
{       //stabilizacia

  boolean current = digitalRead(switchPin);
  if(last != current)
  {
   delay(5);
   current = digitalRead(switchPin);
  }
 return current;
}

void againstClockwise()
{
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

void inClockwise()
{
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

