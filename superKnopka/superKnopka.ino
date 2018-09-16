/*
TEST CODE FOR PROXIMITY SENSOR 
Metal Detection with 3 wire sensor
*/

int monitoring;
int metalDetection = 8;

void setup(){
  Serial.begin(9600);
}

void loop(){
  monitoring = digitalRead(metalDetection);
  if(monitoring == HIGH)
  {
    Serial.println(monitoring);
  }
  else
  {
    Serial.println("LOW");
  }
  delay(500);

}
