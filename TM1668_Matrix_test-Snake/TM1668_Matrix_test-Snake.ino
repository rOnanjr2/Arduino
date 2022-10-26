#include <TM-1668.h>
#include <avr/interrupt.h>
#include <Adafruit_GFX.h>
#include <TM16xxMatrixGFX.h>
// volatile byte i = 0;
// volatile unsigned int incomingByte = 0;
TM1668 TM1668(16,15,14); //data,clock,strobe pins

#define MATRIX_NUMCOLUMNS 6 
#define MATRIX_NUMROWS 7 


const int pinRandom = A0;
const int wait = 300;
const int length = 8;

int x[length], y[length];
int ptr, nextPtr;

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
  // ADCSRA = 0b00000111;//ADC disable
  // Serial.begin(9600);
  sei();


for ( ptr = 0; ptr < length; ptr++ ) {
    x[ptr] = MATRIX_NUMCOLUMNS / 2;
    y[ptr] = MATRIX_NUMROWS / 2;
  }
  nextPtr = 0;

  randomSeed(analogRead(pinRandom));


}

void loop() {

  // Shift pointer to the next segment
  ptr = nextPtr;
  nextPtr = next(ptr);

  TM1668.setPixel(x[ptr], y[ptr], true); // Draw the head of the snake
  //matrix.write(); // Send bitmap to display
  TM1668.updateDisplay();

  delay(wait);

  if ( ! occupied(nextPtr) ) {
    TM1668.setPixel(x[nextPtr], y[nextPtr], false); // Remove the tail of the snake
  }

  for ( int attempt = 0; attempt < 10; attempt++ ) {

    // Jump at random one step up, down, left, or right
    switch ( random(4) ) {
    case 0: x[nextPtr] = constrain(x[ptr] + 1, 0, MATRIX_NUMCOLUMNS - 1); y[nextPtr] = y[ptr]; break;
    case 1: x[nextPtr] = constrain(x[ptr] - 1, 0, MATRIX_NUMCOLUMNS - 1); y[nextPtr] = y[ptr]; break;
    case 2: y[nextPtr] = constrain(y[ptr] + 1, 0, MATRIX_NUMROWS - 1); x[nextPtr] = x[ptr]; break;
    case 3: y[nextPtr] = constrain(y[ptr] - 1, 0, MATRIX_NUMROWS - 1); x[nextPtr] = x[ptr]; break;
    }

    if ( ! occupied(nextPtr) ) {
      break; // The spot is empty, break out the for loop
    }
  }
}

boolean occupied(int ptrA) {
  for ( int ptrB = 0 ; ptrB < length; ptrB++ ) {
    if ( ptrA != ptrB ) {
      if ( equal(ptrA, ptrB) ) {
        return true;
      }
    }
  }

  return false;
}

int next(int ptr) {
  return (ptr + 1) % length;
}

boolean equal(int ptrA, int ptrB) {
  return x[ptrA] == x[ptrB] && y[ptrA] == y[ptrB];
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
