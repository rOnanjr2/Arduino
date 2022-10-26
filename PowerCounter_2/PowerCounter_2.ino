#include <CS.h>
#include <avr/interrupt.h>
volatile byte i = 0;
volatile byte j = 0;
// volatile unsigned int d =0;
volatile bool decrementFlag = 0;
volatile unsigned long timerCapture = 0;
volatile unsigned long powerConsumption = 0;
volatile unsigned int powerInstant = 0;
volatile unsigned int powerInstantLast = 0;
volatile unsigned int powerInstantCalc = 0;
//volatile int incomingByte = 0;
int divIter;
CS1694 CS1694(6,5,4); //data,clock,strobe pins

void setup()  {
  cli();
  CS1694.displaySetup(true); //7x10 mode, false for 6x11 mode
  CS1694.setBrightness(7); //brightness, 0-7
  pinMode(13, OUTPUT); 
  digitalWrite(13, 0); //Internal red LED
  pinMode(2,INPUT_PULLUP); //optocoupler positive
//pinMode(7,OUTPUT);  // optocoupler negative
  digitalWrite(7, 0); //ground for optocoupler
  attachInterrupt(digitalPinToInterrupt(2), blink, FALLING); //optocoupler interrupt
  TCCR0A = 0;
  TCCR0B = 0;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR2A = 0b00000010; //timer 2 set to CTC mode
  TCCR2B = 0b00000101; //divide by 128
  TCNT2 = 0;  //reset counter
  OCR2A = 125; //compare constant for 1 ms interrupt
  TIMSK2 = 0b00000010; //interrupt mask
  powerInstant = 0;
  timerCapture = 0;
  CS1694.clearDisplay();
  Serial.begin(9600);
  ACSR = 0x80; //analogComparator disable
  ADCSRA = 0b00000111;//ADC disable
  CS1694.semicolon(2,1); //2-left colon, 4-right
  sei();
}

void loop() {}

ISR(TIMER2_COMPA_vect){
  timerCapture++;
  if (divIter < 30000 / constrain(powerInstant, 1, 3000)) {
    divIter++;
  }else if(powerInstant > 0) {
    CS1694.dispRing();
    divIter = 0;
  }

  if (i < 30) {
    i++;
  }else {
    refreshScreen();
    i = 0;
  }

  // if (d < 1000) {
  //   d++;
  // }else {
  //   debug();
  //   d = 0;
  // }

}

void blink() {
  powerInstant = powerInstantCalc;
  powerInstantLast = powerInstantCalc;
  timerCapture = 0;
  if (powerConsumption < 409600)
  {
    powerConsumption++;
  }else {
    powerConsumption = 0;
  }
  decrementFlag = 0;
}

void refreshScreen() {
  powerInstantCalc = 500000/timerCapture;
  if(powerInstantLast > powerInstantCalc && decrementFlag == 1) {
    powerInstant = powerInstantCalc;
  }

  if(powerInstantLast == powerInstant){j++;}

  if (j >= 30) {
   decrementFlag = 1;
   j = 0;
 }

  if(powerInstant < 3){powerInstant =0;}

  CS1694.dispNum(powerInstant % 10, 1);
  CS1694.dispNum((powerInstant < 10 && powerInstant % 100 / 10 == 0) ? 36 : powerInstant % 100 / 10, 2); //если число меньше десяти - не показывать незначащий ноль
  CS1694.dispNum((powerInstant < 100 && powerInstant % 1000 / 100 == 0) ? 36 : powerInstant % 1000 / 100, 3); //если число меньше сотни - не показывать незначащий ноль
  CS1694.dispNum((powerInstant / 1000) == 0 ? 36 : powerInstant / 1000, 4); //не показывать незначащий ноль
  CS1694.dispNum(powerConsumption * 10 / 64 % 10, 5);
  CS1694.dispNum(powerConsumption * 10 / 64 % 100 / 10, 6);
  CS1694.dispNum(powerConsumption * 10 / 64 % 1000 / 100, 7);
  CS1694.dispBin(powerConsumption * 10 / 64 % 100000 / 1000);
  CS1694.semicolon(6, !digitalRead(2));
  CS1694.updateDisplay();
}


// void debug() {
//   if (Serial.available() > 0) {
//      incomingByte = Serial.read();
//     Serial.println(powerConsumption);
//     Serial.println(powerInstant);
//     Serial.println(powerInstantLast);
//     Serial.println(powerInstantCalc);
//     Serial.println(TCCR0A);
//     Serial.println(TCCR0B);
//     Serial.println(ACSR, BIN);
//     Serial.println(ADCSRA, BIN);
//   }
// }