#include <Stepper.h>

const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

const int stepsPerRevolution = 2048; // шагов за один оборот

Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4); 

void setup() {
  myStepper.setSpeed(10); // скорость 5 об/минуту
}

void loop() {
  myStepper.step(stepsPerRevolution); // шаг в одном направлении
  delay(1000);
 
//  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution); // шаг в обратную сторону
  delay(1000); 
}
