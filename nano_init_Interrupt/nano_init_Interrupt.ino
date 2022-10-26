#include <CS.h>
#include <avr/interrupt.h>
volatile int powerCount = 0;
//volatile int i = 0;
volatile unsigned long i2 = 0;
int divIter;
//int digit3;
CS1694 CS1694(6,5,4); //data,clock,strobe pins

void setup()  {
  cli();
  CS1694.displaySetup(true); //set to 7x10 mode, set false for 6x11 mode
  CS1694.setBrightness(0); //full brightness, range is 0-7 higher values will be capped
  CS1694.clearDisplay(); //clear any junk off the display
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
  pinMode(2,INPUT_PULLUP);
  pinMode(7,OUTPUT);
  digitalWrite(7, 0);
  attachInterrupt(digitalPinToInterrupt(2), blink, FALLING);
  TCCR2A = 0b00000010;
  TCCR2B = 0b00000101;
  TCNT2 = 0;
  OCR2A = 125;
  TIMSK2 = 0b00000010;
  sei();
  }

void loop(){
  }



ISR(TIMER2_COMPA_vect){
      i2++;
      }

void blink() {
  ledIndicate();
  i2 = 0;
  powerCount++;
}



void ledIndicate(){
  volatile unsigned long result = 500000/i2;
  CS1694.dispNum(result % 10, 1);
  CS1694.dispNum(result % 100 / 10, 2);
  CS1694.dispNum(result % 1000 / 100, 3);
  CS1694.dispNum((result / 1000) == 0 ? 36 : result / 1000, 4);

  CS1694.dispNum(powerCount /2 % 10, 5);
  CS1694.dispNum(powerCount /2 % 100 / 10, 6);
  CS1694.dispNum(powerCount /2 % 1000 / 100, 7);
  
  CS1694.updateDisplay();
}
