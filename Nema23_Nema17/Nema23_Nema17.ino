#include <SoftwareSerial.h>
#include <Nextion.h>
#include "NexButton.h"
#include "NexTouch.h"
#include "NexText.h"
#include "NexPage.h"
#include <AccelStepper.h>

// defines pins numbers
int sensorPin23 = 6; //сенсор
int sensorPin17 = 5; //сенсор
//nema23
int stepPin23 = 9; //рух CLK+
int dirPin23 = 8;  //напрямок CW+
int enPin23 = 10;   //EN+
//nema17
int stepPin17 = 11; //рух CLK+
int dirPin17 = 12;  //напрямок CW+
int enPin17 = 13;   //EN+

AccelStepper nema23(1, stepPin23, dirPin23);
AccelStepper nema17(1, stepPin17, dirPin17);

int koef23 = 1280; //Коефіцієнт для розрахунку відстані 200 шагів за оборот
int koef17 = 100;
float currentPosition23 = -1; //поточна відстань
float currentPosition17 = -1; //поточна відстань
int speed17 = 800;  //швидкість в кроках за сек
int speed23 = 3500;

int direct;
int previousDirect = -1;
float luftSteps = 2000; //кількість кроків на люфт
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
  for (int i; i < 5; i++) text_char2[i] = '\0';
  distanceText23.getText(text_char, 4);
  distanceText17.getText(text_char2, 4);
  move(atof(text_char), atof(text_char2));
  distance23.setFloatText(currentPosition23, 2);
}

// Callback for specific button
void start1PopCallback(void *ptr)
{
  for (int i; i < 5; i++) text_char[i] = '\0';
  for (int i; i < 5; i++) text_char2[i] = '\0';
  distanceText1_23.getText(text_char, 4);
  distanceText1_17.getText(text_char2, 4);
  move(atof(text_char), atof(text_char2)); 
}

// Callback for specific button
void start2PopCallback(void *ptr)
{
  for (int i; i < 5; i++) text_char[i] = '\0';
  for (int i; i < 5; i++) text_char2[i] = '\0';
  distanceText2_23.getText(text_char, 4);
  distanceText2_17.getText(text_char2, 4);
  move(atof(text_char), atof(text_char2)); 
}

// Callback for go to 0 button
void b0ButtonPopCallback(void *ptr)
{
  goToNull(); // вийти в ноль
  distance23.setText("0.00");
  distance17.setText("0.00");
}

void goToHandPageButtonPopCallback(void *ptr)
{
  handPage.show();
  distance23.setFloatText(currentPosition23, 2);
  distance17.setFloatText(currentPosition17, 2);
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

  nema17.setMaxSpeed(speed17);
  nema17.setSpeed(speed17);  
  nema17.setAcceleration(speed17);

  nema23.setMaxSpeed(speed23);
  nema23.setSpeed(speed23);  
  nema23.setAcceleration(speed23); //прискорення 3500 for nema23

  pinMode(sensorPin23,INPUT);
  pinMode(enPin23, OUTPUT);
  digitalWrite(enPin23, HIGH); //turn off driver

  pinMode(sensorPin17,INPUT);
  pinMode(enPin17, OUTPUT);
  digitalWrite(enPin17, HIGH); //turn off driver

  goToNull();
}


void loop() 
{
  nexLoop(nex_listen_list);   
}



//Рухати Nema23 + Nema17
void move(float mm23, float mm17)
{
  if(mm23 < 0 || mm23 > 70 || mm17 < 0 || mm17 >70) //check!!
  {
    errorText.setText("Nevirno!!");
    delay(500);
    handPage.show();
  }
  else
  {
    float distToGo23 = mm23*koef23;
    float distToGo17 = mm17*koef17;
    
    boolean luft = isLuft(mm23); 
    if(luft)
    {
      distToGo23 += luftSteps;
      previousDirect = direct;
    }
        
    nema17.moveTo(distToGo17);
    nema23.moveTo(distToGo23);
    
    digitalWrite(enPin23, LOW);           //turn on driver
    digitalWrite(enPin17, LOW);
    
    int sensor23 = digitalRead(sensorPin23);
    int sensor17 = digitalRead(sensorPin17); 

    boolean finish1 = false, finish2 = false, finish = false;
    while(finish) 
    {
      /************** move Nema 17****************/
      if(nema17.distanceToGo() != 0)
      {
        nema17.run();                     // Move Stepper into position
        sensor17 = digitalRead(sensorPin17);  
        if(sensor17 != 1)                 //Fatal stop
        {
          nema17.stop();
          nema17.setCurrentPosition(0);
          break;
        }
      }
      else 
      {
        finish1 = true;
      }

      /************** move Nema 23****************/
      if(nema23.distanceToGo() != 0)
      {
        nema23.run();                     // Move Stepper into position
        sensor23 = digitalRead(sensorPin23);  
        if(sensor23 != 1)                 //Fatal stop
        {
          nema23.stop();
          nema23.setCurrentPosition(0);
          break;
        }
      }
      else 
      {
        finish2 = true;
      }

      /************** exit from loop ****************/
     if(finish1 && finish2)
     {
       finish = true;
//       break;
     }
    }
    digitalWrite(enPin23, HIGH);          //turn off driver
    digitalWrite(enPin17, HIGH);
    
    if(luft && nema23.currentPosition() != 0)
    {
      nema23.setCurrentPosition(nema23.currentPosition() - luftSteps);
    }
    currentPosition23 = nema23.currentPosition() / koef23; 
    currentPosition17 = nema17.currentPosition() / koef17; 
  }
}

//check if will be luft
boolean isLuft(float distToGo23)
{
  if(distToGo23 >= currentPosition23)  direct = 1;
  else direct = -1;
  if(direct != previousDirect)  return true;
  else return false;
}



//Вийти в ноль Nema23
void goToNull()
{
  int initial_homing = -1;
  int sensor23 = digitalRead(sensorPin23);
  int sensor17 = digitalRead(sensorPin17);

  digitalWrite(enPin23, LOW);
  digitalWrite(enPin17, LOW);

  boolean finish1 = false, finish2 = false, finish = false;
  while(finish)
  {
    /************** move Nema 17****************/
    if(sensor17 == 1 && currentPosition17 != 0)
    {
      sensor17 = digitalRead(sensorPin17);
      nema17.moveTo(initial_homing);  
      nema17.run(); 
      initial_homing--;
    }
    else
    {
      finish1 = true;
    }
    
    /************** move Nema 23****************/
    if(sensor23 == 1 && currentPosition23 != 0)
    {
      sensor23 = digitalRead(sensorPin23);
      nema23.moveTo(initial_homing);  
      nema23.run(); 
      initial_homing--;
    }
    else
    {
      finish2 = true;
    }
    
     /************** exit from loop ****************/
     if(finish1 && finish2)
     {
       finish = true;
//       break;
     }
   }
   
  digitalWrite(enPin23, HIGH);  
  digitalWrite(enPin17, HIGH);
  
  nema23.setCurrentPosition(0);
  nema17.setCurrentPosition(0);
  currentPosition17 = nema17.currentPosition(); 
  currentPosition23 = nema23.currentPosition(); 
}

