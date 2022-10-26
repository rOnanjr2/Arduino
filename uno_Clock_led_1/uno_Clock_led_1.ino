#include <TimeLib.h>
#include <CS.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
volatile byte cnt = 0;
CS1694 CS1694(14,15,16); //data,clock,strobe pins

void setup()  {
  CS1694.displaySetup(true); //set to 7x10 mode, set false for 6x11 mode
  CS1694.setBrightness(0); //full brightness, range is 0-7 higher values will be capped
  CS1694.clearDisplay(); //clear any junk off the display
  setTime(1637378400);
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
  if (cnt < 31) {
    cnt++;
    }else {
      ledIndicate();
      cnt = 0;
      }
  }



void ledIndicate(){
  int secndl = second() % 10;
  int secndh = second() / 10;
  int mnl = minute() % 10;
  int mnh = minute() / 10;
  int hrl = hour() % 10;
  int hrh = hour() / 10;

  CS1694.dispNum(secndl,0);
  CS1694.dispNum(secndh,1);
  CS1694.dispNum(mnl,2);
  CS1694.dispNum(mnh,3);
  CS1694.dispNum(hrl,4);
  CS1694.dispNum(hrh,5);
  CS1694.dispNum(33,10);

  CS1694.updateDisplay();
}

void semicolon(){
  CS1694.dispNum(34,10);
  CS1694.updateDisplay();
  }




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
