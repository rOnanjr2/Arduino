#include <RTClib.h>
#include "GyverTimers.h"
#include "GyverPower.h"
#include <CS.h>
#define load1 8
#define on true
#define off false
//#define B_READ(bp,bb) bool((bp) & (bb))
 byte cnt = 0;
 byte cnt2 = 0;
 byte timeSeconds;
 byte timeHours;
 byte timeMinutes;
 bool isCorrected;
 byte alarmOnHr;
 byte alarmOnMn;
 byte alarmOffHr;
 byte alarmOffMn;
 byte setupIndex;
CS1694 CS1694(14,15,16);            //data,clock,strobe pins
RTC_DS1307 rtc;

void setup () {
  power.hardwareDisable(PWR_ADC | PWR_TIMER0);
  pinMode(17, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(load1, OUTPUT);
  digitalWrite(17, 1);
  rtc.begin();
  rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
  CS1694.displaySetup(true);        //set to 7x10 mode, set false for 6x11 mode
  CS1694.setBrightness(0);          //full brightness, range is 0-7 higher values will be capped
  //CS1694.clearDisplay();            //clear any junk off the display
  Timer2.setPeriod(1000);        //Устанавливаем период таймера 1000000 мкс -> 1 гц
  Timer2.enableISR(CHANNEL_B);
  OCR2A = 251;
  rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
  //rtc.adjust(DateTime(2021, 12, 21, 16, 18, 00));
  DateTime now = rtc.now();
  timeSeconds = now.second();
  timeMinutes   = now.minute();
  timeHours = now.hour();
  attachInterrupt(0, everySecond, RISING);
  power.setSleepMode(IDLE_SLEEP); // если нужен другой режим сна, см константы в GyverPower.h (по умолчанию POWERDOWN_SLEEP)
  power.bodInSleep(false);
  //Serial.begin(9600);
  //timeCorrect();
  }

void loop () {
  digitalWrite(13, 0);
  power.sleep(SLEEP_FOREVER);
  }

void everySecond() {
  digitalWrite(13, 1);
  sei();
  timeRefresh();
  TCNT2 = 0;
  cnt = 3;
  if (setupIndex == 0) {checkTime();}
  //CS1694.dispRing();
  //Serial.println(38 + timeSeconds % 10, DEC);
  }

ISR(TIMER2_B) {
  digitalWrite(13, 1);
  if (cnt < 31) {cnt++;} else{ledIndicate(); cnt = 0;}
  }

void timeRefresh(){
  DateTime now = rtc.now();
  timeSeconds = now.second();
  timeMinutes = now.minute();
  timeHours = now.hour();
  }

void ledIndicate(){
  if (cnt2 <= 2) {cnt2++;} else{checkButtons(CS1694.readButtons()); cnt2 = 0;}
  if (setupIndex > 0) {drawAlarm();} else{drawTime();}
  CS1694.updateDisplay();
  }

void drawTime() {
  CS1694.dispNum(timeSeconds % 10, 0);
  CS1694.dispNum(timeSeconds / 10, 1);
  CS1694.dispNum(timeMinutes % 10, 2);
  CS1694.dispNum(timeMinutes / 10, 3);
  CS1694.dispNum(timeHours   % 10, 4);
  CS1694.dispNum(timeHours   / 10, 5);
  CS1694.dispNum(36, 6);
  CS1694.dispNum(36, 10);
  //CS1694.dispRing();
  }

void checkTime() {
  if(timeHours == 4 && timeMinutes == 59 && timeSeconds == 58 && !isCorrected) {isCorrected = timeCorrect();}
  if(timeHours == 4 && timeMinutes == 59 && timeSeconds == 59) {isCorrected = false;}
  if(timeHours == alarmOnHr && timeMinutes == alarmOnMn) {digitalWrite(load1, on);}
  if(timeHours == alarmOffHr && timeMinutes == alarmOffMn) {digitalWrite(load1, off);}
  }

bool timeCorrect() {
  DateTime now = rtc.now();
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() - 17));
  return true;
  }

void checkButtons(byte bt) {
  //  ringIndex < 7 ? ringIndex++ : ringIndex = 0;
  CS1694.dispRing();
  if (bitRead(bt, 0)) {
    if (setupIndex < 4) {
      setupIndex++;
      if (setupIndex == 3) {
        alarmOffHr = alarmOnHr;
        alarmOffMn = alarmOnMn;
        alarmOffMn++;
      }
    }else {setupIndex = 0;}}
  if (bitRead(bt, 2)) {decrement();}
  if (bitRead(bt, 1)) {increment();}
  }

void drawAlarm () {
  if (setupIndex == 1 || setupIndex == 2) {
    CS1694.dispNum(alarmOnMn % 10, 0);
    CS1694.dispNum(alarmOnMn / 10, 1);
    CS1694.dispNum(alarmOnHr % 10, 2);
    CS1694.dispNum(alarmOnHr / 10, 3);}

  if (setupIndex == 3 || setupIndex == 4) {
    CS1694.dispNum(alarmOffMn % 10, 0);
    CS1694.dispNum(alarmOffMn / 10, 1);
    CS1694.dispNum(alarmOffHr % 10, 2);
    CS1694.dispNum(alarmOffHr / 10, 3);}

  if (setupIndex == 1 || setupIndex == 2) {CS1694.dispNum(36, 4);}
  if (setupIndex == 3 || setupIndex == 4) {CS1694.dispNum(15, 4);}
  if (setupIndex == 1 || setupIndex == 2) {CS1694.dispNum(23, 5);}
  if (setupIndex == 3 || setupIndex == 4) {CS1694.dispNum(15, 5);}
  CS1694.dispNum(0, 6);
  if (setupIndex == 1 || setupIndex == 3) {CS1694.dispNum(18, 10);}
  if (setupIndex == 2 || setupIndex == 4) {CS1694.dispNum(32, 10);}
  }

void increment() {
  if (setupIndex == 1) {
    if (alarmOnMn < 59) {alarmOnMn++;} else{alarmOnMn = 0;}}
  if (setupIndex == 3) {
    if (alarmOffMn < 59) {alarmOffMn++;} else{alarmOnMn = 0;}}

  if (setupIndex == 2) {
    if (alarmOnHr < 23) {alarmOnHr++;} else{alarmOnHr = 0;}}
  if (setupIndex == 4) {
    if (alarmOffHr < 23) {alarmOffHr++;} else{alarmOnHr = 0;}}
  }

void decrement() {
  if (setupIndex == 1) {
    if (alarmOnMn > 0) {alarmOnMn--;} else{alarmOnMn = 59;}}
  if (setupIndex == 3) {
    if (alarmOffMn > 0) {alarmOffMn--;} else{alarmOffMn = 59;}}

  if (setupIndex == 2) {
    if (alarmOnHr > 0) {alarmOnHr--;} else{alarmOnHr = 23;}}
  if (setupIndex == 4) {
    if (alarmOffHr > 0) {alarmOnHr--;} else{alarmOffHr = 23;}}
  }