#include <SoftwareSerial.h>
#include <Nextion.h>
#include "NexButton.h"
#include "NexTouch.h"
#include "NexText.h"
#include "NexPage.h"
#include <AccelStepper.h>

// defines pins numbers
int sensorPin = 6; //сенсор
int stepPin = 9; //рух CLK+
int dirPin = 8;  //напрямок CW+
int enPin =10;   //EN+

AccelStepper nema23(1, stepPin, dirPin);

int koef23 = 1067; //Коефіцієнт для розрахунку відстані 200 шагів за оборот
float currentPosition23 = -1; //поточна відстань
int direct;
int previousDirect = -1;
float luftSteps = 533; //кількість кроків на люфт
boolean isNull;
char text_char[4];
char text_char2[4];


// Nextion elements
//go to null
NexButton b0 = NexButton(2, 12, "b2"); 
 
//Ручне введення
NexButton goToHandPage = NexButton(1, 6, "b0");
NexPage handPage = NexPage(2, 0, "hand");
NexButton handStart = NexButton(2, 5, "b0");
NexText distanceText23 = NexText(2, 1, "t0");  //input
NexText distanceText17 = NexText(2, 2, "t1");  //input
NexText distance23 = NexText(2, 8, "t5");  //output
NexText distance17 = NexText(2, 9, "t6");  //output
NexText errorText = NexText(2, 7, "t4");

//page1 №4
NexTouch start1 = NexTouch(4, 3, "m2"); // where 4 - page, 3 - id, m2 -objname
NexText distanceText1_23 = NexText(4, 4, "t0");
NexText distanceText1_17 = NexText(4, 5, "t1");

//page2 №5
NexTouch start2 = NexTouch(5, 3, "m2");
NexText distanceText2_23 = NexText(5, 4, "t0"); 
NexText distanceText2_17 = NexText(5, 5, "t1"); 


// Clickable elements
NexTouch *nex_listen_list[] = 
{
  &b0,
  &handStart,
  &goToHandPage,
  &start1,
  &start2,
  NULL
};


// Callback for hand start
void handStartPopCallback(void *ptr)
{
  for (int i; i < 5; i++) text_char[i] = '\0';
  distanceText23.getText(text_char, 4);
  move(atof(text_char));
  distance23.setFloatText(currentPosition23, 2);
}

// Callback for specific button
void start1PopCallback(void *ptr)
{
  for (int i; i < 5; i++) text_char[i] = '\0';
  distanceText1_23.getText(text_char, 4);
  move(atof(text_char)); 
}

// Callback for specific button
void start2PopCallback(void *ptr)
{
  for (int i; i < 5; i++) text_char[i] = '\0';
  distanceText2_23.getText(text_char, 4);
  move(atof(text_char)); //вказати відстань в мм
}

// Callback for go to 0 button
void b0ButtonPopCallback(void *ptr)
{
  goToNull(); // вийти в ноль
  distance23.setText("0");
}

void goToHandPageButtonPopCallback(void *ptr)
{
  handPage.show();
  distance23.setFloatText(currentPosition23, 2);
}
 

void setup()
{
  Serial.begin(9600);

  handStart.attachPop(handStartPopCallback, &handStart);
  b0.attachPop(b0ButtonPopCallback, &b0);
  goToHandPage.attachPop(goToHandPageButtonPopCallback, &goToHandPage);
  //підключаютьсь колбеки до кнопки "старт" кожної сторінки
  start1.attachPop(start1PopCallback, &start1);
  start2.attachPop(start2PopCallback, &start2);

  nema23.setMaxSpeed(5000);
  nema23.setSpeed(5000);  
  nema23.setAcceleration(5000); //прискорення 3500 for nema23

  pinMode(sensorPin,INPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, HIGH); //turn off driver

  goToNull();
}

void loop() 
{
  nexLoop(nex_listen_list);   
}

//Рухати Nema23
void move(float mm23)
{
  if(mm23 < 0 || mm23 > 70)
  {
    errorText.setText("Nevirno!!");
    delay(500);
    handPage.show();
  }
  else
  {
    float distToGo = mm23*koef23;
    boolean luft = isLuft(mm23); 
    if(luft)
    {
      distToGo += luftSteps * direct;
      previousDirect = direct;
    }
    nema23.moveTo(distToGo);
    digitalWrite(enPin, LOW);           //turn on driver
//    int sensor = 0;

    while(nema23.distanceToGo() != 0) {
      nema23.run();                     // Move Stepper into position
      
//      sensor = digitalRead(sensorPin);  
//      if(sensor != 1)          //Fatal stop
//      {
//        nema23.stop();
//        nema23.setCurrentPosition(0);
//        break;
//      }
    }
    
    digitalWrite(enPin, HIGH);          //turn off driver

    if(luft && nema23.currentPosition() != 0)
    {
      nema23.setCurrentPosition(nema23.currentPosition() - luftSteps * direct);
    }
    currentPosition23 = nema23.currentPosition() / koef23; 
  }
}

boolean isLuft(float distToGo)
{
  if(distToGo >= currentPosition23)  direct = 1;
  else direct = -1;
  if(direct != previousDirect)  return true;
  else return false;
}

//Вийти в ноль Nema23
void goToNull()
{
  int initial_homing = -1;
  int sensor = digitalRead(sensorPin);
  nema23.moveTo(-75*koef23);  

  digitalWrite(enPin, LOW);
  while(sensor == 1 && currentPosition23 != 0)
  {
    sensor = digitalRead(sensorPin);
    nema23.run(); 
//    initial_homing=initial_homing-100;
   }
  
  digitalWrite(enPin, HIGH);
  nema23.setCurrentPosition(0);
  currentPosition23 = nema23.currentPosition(); 
}

