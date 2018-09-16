#include <SoftwareSerial.h>
#include <Nextion.h>
#include "NexButton.h"

char buffer[10] = {0};


// Nextion elements
//go to null
NexButton b0 = NexButton(0, 1, "b0"); 

// Clickable elements
NexTouch *nex_listen_list[] = 
{
  &b0,
  NULL
};


// Callback for go to 0 button
void b0ButtonPopCallback(void *ptr)
{
//    NexButton *btn = (NexButton *)ptr;
//    memset(buffer, 0, sizeof(buffer));
//    btn->getText(buffer, sizeof(buffer));
//    strcpy(buffer, "after");
//    btn->setText("hh");
//    Serial.print("0 text_char: ");

b0.setText("hg");
}
 

void setup()
{
  Serial.begin(9600);

  b0.attachPop(b0ButtonPopCallback, &b0);
}

void loop() 
{
  nexLoop(nex_listen_list);   
}


