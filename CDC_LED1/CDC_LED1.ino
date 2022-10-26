#include <DigiCDC.h>
void setup() {                
  // initialize the digital pin as an output.
  SerialUSB.begin(); 
  pinMode(2,INPUT);

}  

  int analogPin = 2;
  int val = 0;
  
void loop()
{
  val = analogRead(analogPin);    // считываем напряжение с аналогового входа
  Serial.println(val);            // наблюдаем считанное значение 
   SerialUSB.delay(100);               // keep usb alive // can alos use SerialUSB.refresh();
}
