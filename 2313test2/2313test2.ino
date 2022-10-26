#include "ds18b20.h"



void setup() {
  // put your setup code here, to run once:
pinMode(7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
analogWrite(7, (50 - getTempDS18B20())*10);
delay(1000);
}
