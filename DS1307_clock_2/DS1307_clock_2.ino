#include <RTClib.h>
#include "GyverTimers.h"
#include "GyverPower.h"
#include <CS.h>
volatile byte cnt = 0;
volatile byte timeSeconds;
volatile byte timeHours;
volatile byte timeMinutes;
CS1694 CS1694(14,15,16);            //data,clock,strobe pins
RTC_DS1307 rtc;

void setup () {
  power.hardwareDisable(PWR_ADC | PWR_TIMER0);
  pinMode(17, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  digitalWrite(17, 1);
  rtc.begin();
  rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
  CS1694.displaySetup(true);        //set to 7x10 mode, set false for 6x11 mode
  CS1694.setBrightness(0);          //full brightness, range is 0-7 higher values will be capped
  CS1694.clearDisplay();            //clear any junk off the display
  Timer2.setPeriod(1000);        //Устанавливаем период таймера 1000000 мкс -> 1 гц
  Timer2.enableISR(CHANNEL_B);
  OCR2A = 251;
  rtc.writeSqwPinMode(DS1307_SquareWave1HZ);  //  rtc.adjust(DateTime(2021, 11, 21, 3, 7, 30));
  DateTime now = rtc.now();
  timeSeconds = now.second();
  timeMinutes   = now.minute();
  timeHours = now.hour();
  attachInterrupt(0, everySecond, RISING);
  power.setSleepMode(IDLE_SLEEP); // если нужен другой режим сна, см константы в GyverPower.h (по умолчанию POWERDOWN_SLEEP)
  power.bodInSleep(false);
  // Serial.begin(9600);
  }

void loop () {
  digitalWrite(13, 0);
  power.sleep(SLEEP_FOREVER);
  }

void everySecond() {
   sei();
  // Timer2.disableISR(CHANNEL_B);
  timeRefresh();
  // Timer2.enableISR(CHANNEL_B);
//  Timer2.restart();
  TCNT2 = 0;
  cnt = 3;
  // Serial.println(OCR2B);
  // Serial.println(OCR2A);
  // Serial.println(TCCR2A, BIN);
  // Serial.println(TCCR2B, BIN);
  // Serial.println();
  }

ISR(TIMER2_B) {
  digitalWrite(13, 1);
  if (cnt < 31) {
    cnt++;
    }else {
      ledIndicate();
      cnt = 0;
      }
  }




void timeRefresh(){
  digitalWrite(13, 1);
  DateTime now = rtc.now();
  timeSeconds = now.second();
  timeMinutes = now.minute();
  timeHours = now.hour();
  }

void ledIndicate(){
//  sei();
  drawTime(); //  CS1694.dispNum(33,10);              //33 - две точки, 34 - одна
//  detachInterrupt(0);
  CS1694.updateDisplay();
//  attachInterrupt(0, everySecond, RISING);
  }

void drawTime() {
  CS1694.dispNum(timeSeconds % 10, 0);
  CS1694.dispNum(timeSeconds / 10, 1);
  CS1694.dispNum(timeMinutes % 10, 2);
  CS1694.dispNum(timeMinutes / 10, 3);
  CS1694.dispNum(timeHours   % 10, 4);
  CS1694.dispNum(timeHours   / 10, 5);
  }
