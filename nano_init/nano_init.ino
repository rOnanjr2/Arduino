#include <TimeLib.h>
#include <CS.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
volatile byte cnt = 0;
int divIter;
//int digit3;
CS1694 CS1694(6,5,4); //data,clock,strobe pins

void setup()  {
  CS1694.displaySetup(true); //set to 7x10 mode, set false for 6x11 mode
  CS1694.setBrightness(0); //full brightness, range is 0-7 higher values will be capped
  CS1694.clearDisplay(); //clear any junk off the display
  setTime(1652036400);
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
  TIMSK0 |= (1 << OCIE0B);
//test();
//cli();
  }

void loop(){
 digitalWrite(13, 0);
//  sleep_mode();
//  ledIndicate();
//  delay(500);
//  semicolon();
//  delay(500);
  }

ISR(TIMER0_COMPB_vect) {
  digitalWrite(13, 1);
  if (cnt < 20) {       //31
    cnt++;
    }else {
      ledIndicate();
      cnt = 0;
      }
  }



void ledIndicate(){

//  CS1694.dispNum(0,0);
//  CS1694.dispNum(1,1);
//  CS1694.dispNum(2,2);
//  CS1694.dispNum(3,3);
//  CS1694.dispNum(4,4);
//  CS1694.dispNum(5,5);
//  CS1694.dispNum(6,6);
//  CS1694.dispNum(7,7);
  ring(CS1694.digit3);
  CS1694.updateDisplay();
  CS1694.convertVar(millis()/100);
  CS1694.updateDisplay();
}



void ring(int divider){
  int i = map(divider, 0, 9, 9, 0);
  if(divIter < i) {
    divIter++;
  }else {
    CS1694.dispRing();
    divIter = 0;
  }}

void test(){
//  CS1694.dispNum(8,0);
//  CS1694.dispNum(8,1);
//  CS1694.dispNum(8,2);
//  CS1694.dispNum(8,3);
//  CS1694.dispNum(8,4);
//  CS1694.dispNum(8,5);
//  CS1694.dispNum(8,6);
//  CS1694.dispNum(8,7);
//  CS1694.dispNum(8,8);
//  CS1694.dispNum(8,9);
//  CS1694.dispNum(35,7);
//  CS1694.updateDisplay();
}
