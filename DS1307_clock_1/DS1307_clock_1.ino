#include <RTClib.h>
#include "GyverTimers.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>
#include <CS.h>

byte cnt = 0;

CS1694 CS1694(14,15,16); //data,clock,strobe pins
RTC_DS1307 rtc;

void setup () {
  pinMode(17, OUTPUT);
  digitalWrite(17, 1);
  rtc.begin();
  rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
//  Serial.begin(9600);

  CS1694.displaySetup(true); //set to 7x10 mode, set false for 6x11 mode
  CS1694.setBrightness(0); //full brightness, range is 0-7 higher values will be capped
  CS1694.clearDisplay(); //clear any junk off the display
  
  pinMode(13, OUTPUT);
  pinMode(5, INPUT_PULLUP);

 Timer0.setPeriod(1000000);     // Устанавливаем период таймера 1000000 мкс -> 1 гц
  Timer0.enableISR(CHANNEL_B);
//TIMSK0 |= (1 << OCIE0B);
//  sei();

    rtc.writeSqwPinMode(DS1307_SquareWave1HZ);

 // rtc.adjust(DateTime(2021, 11, 21, 3, 7, 30));
  }


void loop () {
  digitalWrite(13, 0);
  }


ISR(TIMER0_B) {
  digitalWrite(13, 1);
  if (cnt < 60) {
    cnt++;
    }else {
      ledIndicate();
      cnt = 0;
      }
  }

void ledIndicate(){
  sei();
  DateTime now = rtc.now();
  CS1694.dispNum(now.second() % 10, 0);
  CS1694.dispNum(now.second() / 10, 1);
  CS1694.dispNum(now.minute() % 10, 2);
  CS1694.dispNum(now.minute() / 10, 3);
  CS1694.dispNum(now.hour()   % 10, 4);
  CS1694.dispNum(now.hour()   / 10, 5);
  CS1694.dispNum(33,10);

  CS1694.updateDisplay();

//  Serial.println(F(__TIME__));
}

//ISR(TIMER2_A){
//    digitalWrite(13, !digitalRead(13));
//}

//    digitalWrite(13, !digitalRead(13));
