#include <TM-1668.h>
#include <arduinoFFT.h>
arduinoFFT FFT = arduinoFFT();
TM1668 TM1668(16,15,14);
#define bands 128
double realComponent[bands];
double imagComponent[bands];
int spectralHeight[] = {0, 1, 3, 7, 15, 31, 63, 127};
byte lastHeight[7];
int  index, c, value;

void setup()  {
  TM1668.displaySetup(1); //7x10 mode, false for 6x11 mode
  TM1668.setBrightness(7); //brightness, 0-7
  // TCCR0A = 0;
  // TCCR0B = 0;
  // TCCR1A = 0;
  // TCCR1B = 0;
  // TCCR2A = 0b00000010; //timer 2 set to CTC mode
  // TCCR2B = 0b00000101; //divide by 128
  // TCNT2 = 0;  //reset counter
  // OCR2A = 125; //compare constant for 1 ms interrupt
  // TIMSK2 = 0b00000010; //interrupt mask
  TM1668.clearDisplay();
  ACSR = 0x80; //analogComparator disable
  // Serial.begin(250000);
  // pinMode(7, OUTPUT);
  // digitalWrite(7, 1);
  analogReference(3);

  ADCSRA = 0b10010101;

}

void loop() {
  for(int i=0; i < bands; i++)
  {
    realComponent[i] = analogRead(A5) - 511;
    imagComponent[i] = 0;
  }

  FFT.Windowing(realComponent, bands, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(realComponent, imagComponent, bands, FFT_FORWARD);
  FFT.ComplexToMagnitude(realComponent, imagComponent, bands);

  TM1668.drawColumn(spectralHeight[computeHeight(1, 0)], 0);
  TM1668.drawColumn(spectralHeight[computeHeight(3, 1)], 1);
  TM1668.drawColumn(spectralHeight[computeHeight(8, 2)], 2);
  TM1668.drawColumn(spectralHeight[computeHeight(16,3)], 3);
  TM1668.drawColumn(spectralHeight[computeHeight(32,4)], 4);
  TM1668.drawColumn(spectralHeight[computeHeight(55,5)], 5);
  TM1668.updateDisplay();



  // delay(100);
  // Serial.println();
  // Serial.println();
  // for(int i=0; i<bands; i++) {
  //   Serial.print(i);
  //   Serial.print("  ");
  //   Serial.println(realComponent[i]);
  // }


}

int computeHeight(int i, byte colNumber) {
  index = constrain(log10(realComponent[i]) * 10, 0, 40);
  index = map(index, 17, (40 - colNumber * 3), 0, 7);
  index = constrain(index, 0, 7);
  if (index > lastHeight[colNumber]) {
    lastHeight[colNumber] = index;
    return index;
  }else if(index < lastHeight[colNumber]) {
    if (lastHeight[colNumber] > 0) {
      lastHeight[colNumber]--;
    }
    return lastHeight[colNumber];
  }else {
    return lastHeight[colNumber];
  }
}