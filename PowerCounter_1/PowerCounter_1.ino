#include <CS.h>
#include <avr/interrupt.h>
volatile byte i = 0;
volatile unsigned long timerCapture = 0;
volatile int powerConsumption = 0;
volatile int powerInstant = 0;
int divIter;
CS1694 CS1694(6,5,4); //data,clock,strobe pins

void setup()  {
  cli();
  CS1694.displaySetup(true); //7x10 mode, false for 6x11 mode
  CS1694.setBrightness(7); //brightness, 0-7
  pinMode(13, OUTPUT); 
  digitalWrite(13, 0); //Internal red LED
  pinMode(2,INPUT_PULLUP); //optocoupler positive
  pinMode(7,OUTPUT);  // optocoupler negative
  digitalWrite(7, 0); //ground for optocoupler
  attachInterrupt(digitalPinToInterrupt(2), blink, FALLING); //optocoupler interrupt
  TCCR2A = 0b00000010; //timer 2 set to CTC mode
  TCCR2B = 0b00000101; //divide by 128
  TCNT2 = 0;  //reset counter
  OCR2A = 125; //compare constant for 1 ms interrupt
  TIMSK2 = 0b00000010; //interrupt mask
  powerInstant = 0;
  timerCapture = 0;
  CS1694.clearDisplay();
//  Serial.begin(9600);
  sei();
  }

void loop() {}

ISR(TIMER2_COMPA_vect){
      timerCapture++;
      if (i < 30) {
        i++;
        }else {
          refreshScreen();
          i = 0;
          }
      }

void blink() {
  powerInstant = 500000/timerCapture; //Расчет моментальной мощности
  timerCapture = 0;
  powerConsumption++;
//  Serial.println(powerInstant, DEC);
       
}

void refreshScreen() {
  CS1694.dispRing();
  CS1694.dispNum(powerInstant % 10, 1);
  CS1694.dispNum((powerInstant < 10 && powerInstant % 100 / 10 == 0) ? 36 : powerInstant % 100 / 10, 2); //если число меньше десяти - не показывать незначащий ноль
  CS1694.dispNum((powerInstant < 100 && powerInstant % 1000 / 100 == 0) ? 36 : powerInstant % 1000 / 100, 3); //если число меньше сотни - не показывать незначащий ноль
  CS1694.dispNum((powerInstant / 1000) == 0 ? 36 : powerInstant / 1000, 4); //не показывать незначащий ноль
  CS1694.dispNum(powerConsumption * 10 / 64 % 10, 5);
  CS1694.dispNum(powerConsumption * 10 / 64 % 100 / 10, 6);
  CS1694.dispNum(powerConsumption * 10 / 64 % 1000 / 100, 7);
  CS1694.updateDisplay();
  }
