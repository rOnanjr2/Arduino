#include <TM-1668.h>
#include <avr/interrupt.h>
#include <Adafruit_GFX.h>
#include <TM16xxMatrixGFX.h>
// volatile byte i = 0;
// volatile unsigned int incomingByte = 0;
TM1668 TM1668(16,15,14); //data,clock,strobe pins

#define MODULE_SIZECOLUMNS 6 
#define MODULE_SIZEROWS 7 


int pinRandom = A0;
int wait = 20;

void setup()  {
  cli();
  TM1668.displaySetup(1); //7x10 mode, false for 6x11 mode
  TM1668.setBrightness(1); //brightness, 0-7
  // TCCR0A = 0;
  // TCCR0B = 0;
  // TCCR1A = 0;
  // TCCR1B = 0;
  // TCCR2A = 0b00000010; //timer 2 set to CTC mode
  // TCCR2B = 0b00000101; //divide by 128
  // TCNT2 = 0;  //reset counter
  // OCR2A = 125; //compare constant for 1 ms interrupt
  // TIMSK2 = 0b00000010; //interrupt mask
  // TM1668.clearDisplay();
  ACSR = 0x80; //analogComparator disable
  ADCSRA = 0b00000111;//ADC disable
  // Serial.begin(9600);
  sei();
}

void loop() {
  static int x = MODULE_SIZECOLUMNS / 2;
  static int y = MODULE_SIZEROWS / 2;
  int xNext, yNext;
  
  TM1668.setPixel(x, y, HIGH);
  TM1668.updateDisplay(); // Send bitmap to display

  delay(wait);

  TM1668.setPixel(x, y, LOW); // Erase the old position of our dot

  do {
    switch ( random(4) ) {
      case 0: xNext = constrain(x + 1, 0, MODULE_SIZECOLUMNS - 1); yNext = y; break;
      case 1: xNext = constrain(x - 1, 0, MODULE_SIZECOLUMNS - 1); yNext = y; break;
      case 2: yNext = constrain(y + 1, 0, MODULE_SIZEROWS - 1); xNext = x; break;
      case 3: yNext = constrain(y - 1, 0, MODULE_SIZEROWS - 1); xNext = x; break;
    }
  }
  while ( x == xNext && y == yNext ); // Repeat until we find a new coordinate

  x = xNext;
  y = yNext;
}


//       if (Serial.available() > 0) {
//         incomingByte = Serial.parseInt();
//         TM1668.setPixel(incomingByte % 100 / 10, incomingByte % 10, incomingByte % 1000 / 100);
//         Serial.print("x is: ");
//         Serial.print(incomingByte % 100 / 10);
//         Serial.print("  y is: ");
//         Serial.print(incomingByte % 10);
//         Serial.print("  state is:");
//         Serial.println(incomingByte % 1000 / 100);
//     }
// }

// ISR(TIMER2_COMPA_vect){
//   if (i < 30) {
//     i++;
//   }else {
//     refreshScreen();
//     i = 0;
//   }
// }

// void refreshScreen() {
//   TM1668.updateDisplay();
// }