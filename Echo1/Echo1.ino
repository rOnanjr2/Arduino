#include <DigiCDC.h>

  int led = 1;
  int val = 0;
  bool on = HIGH;
  bool off = LOW;

void setup()
{                
  SerialUSB.begin();
  pinMode(1, OUTPUT);
  pinMode(2, INPUT);
  analogReference(DEFAULT);
}
 
void loop()
{
    val = analogRead(1);
    SerialUSB.println(val);
    SerialUSB.delay(500);
    SerialUSB.refresh();
}

