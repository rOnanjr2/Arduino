#include <CS2313.h>

byte editDigit = 0;
byte digitVal = 0;
CS1694 CS1694(14,15,16);

void setup() {
CS1694.displaySetup(true); //set false if your display works with 6x11 leave true if segments are set up as 7x10 
CS1694.setBrightness(1);
CS1694.clearDisplay();

}

void loop() {
CS1694.dispNum(32,1);
CS1694.updateDisplay();
delay (1000);
}
