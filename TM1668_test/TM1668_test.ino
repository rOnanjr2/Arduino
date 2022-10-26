#include <TM-1668.h>
#include <avr/interrupt.h>
volatile unsigned int timerCapture = 0;
volatile byte i = 0;
volatile unsigned int incomingByte = 0;
TM1668 TM1668(16,15,14); //data,clock,strobe pins

void setup()  {
  cli();
  TM1668.displaySetup(1); //7x10 mode, false for 6x11 mode
  TM1668.setBrightness(1); //brightness, 0-7
  // TCCR0A = 0;
  // TCCR0B = 0;
  // TCCR1A = 0;
  // TCCR1B = 0;
  TCCR2A = 0b00000010; //timer 2 set to CTC mode
  TCCR2B = 0b00000101; //divide by 128
  TCNT2 = 0;  //reset counter
  OCR2A = 125; //compare constant for 1 ms interrupt
  TIMSK2 = 0b00000010; //interrupt mask
  TM1668.clearDisplay();
  ACSR = 0x80; //analogComparator disable
  ADCSRA = 0b00000111;//ADC disable
  Serial.begin(9600);
  sei();
}

void loop() {

      // if (Serial.available() > 0) {
  if (incomingByte < 255) {
        // incomingByte = Serial.parseInt();
        TM1668.dispDebug(1, incomingByte / 10, incomingByte % 10);
        incomingByte++;
        // Serial.print(incomingByte / 10);
        // Serial.println(incomingByte % 10);
        delay(50);
    }
}

ISR(TIMER2_COMPA_vect){
  timerCapture++;

  if (i < 30) {
    i++;
  }else {
    TM1668.updateDisplay();
    i = 0;
  }
}