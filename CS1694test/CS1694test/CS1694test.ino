#include <CS.h>

byte editDigit = 0;
byte digitVal = 0;
CS1694 CS1694(4,3,2); //data,clock,strobe pins
void setup() {

CS1694.displaySetup(true); //set to 7x10 mode, set false for 6x11 mode
  CS1694.setBrightness(0); //full brightness, range is 0-7 higher values will be capped
  CS1694.clearDisplay(); //clear any junk off the display
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

CS1694.convertVar(millis()/100); // convert a variable to 7 segment numbers for the display, takes unsigned long, shows first 6 MSD 
CS1694.updateDisplay(); //refresh the display, transferring the contents of the variable array into display memory on the chip
delay(100);
 
}
