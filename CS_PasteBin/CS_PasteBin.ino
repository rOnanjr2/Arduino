//https://pastebin.com/jCxcwxCv 
#define getBit(value, bitn) ((((value)&(1<<(bitn)))>>(bitn))?HIGH:LOW)
 
byte ledPin = 13;                // LED connected to digital pin 13
byte clkPin = 15;
byte dataPin = 14;
byte csPin = 16;
byte delaySetup = 10;
byte delayByte = 20;
byte main_i=0;
byte push_i=0;
 
// byte KEY1 = 0x04;
// byte KEY2 = 0x10;
// byte KEY3 = 0x20;
 
byte ET6202DIG[10];
 
byte LED[10] = {
0b01111011, //  0
0b00110000, //  1
0b01101101, //  2
0b01111100, //  3  
0b00110110, //  4
0b01011110, //  5
0b01011111, //  6
0b01110000, //  7
0b11111111, //  8
0b01111110  //  9
};
 
//byte POINT[4] = {
//  0x00,  // none
//  0x04,  // e   00000100 1-left
//  0x02,  // f   00000010 2-right
//  0x06,  // e,f 00000110 1|2 = 3 - left+right
//};
 
//unsigned long prev_key=0;
 
void setup()                    // run once, when the sketch starts
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  pinMode(dataPin, OUTPUT);
  pinMode(clkPin, OUTPUT);
  pinMode(csPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clkPin, HIGH);
  digitalWrite(csPin, HIGH);
//  initET6202();
  
//  setDigit(LED[0], 0);
//  setDigit(LED[1], 1);
//  setDigit(LED[2], 7);
//  setDigit(LED[3], 2);
//  setDigit(LED[4], 3);
//  setDigit(LED[5], 4);
//  setDigit(LED[6], 5);
//  setDigit(LED[8], 6);
//  setDigit(LED[8], 8);
//  setDigit(LED[8], 9);
//  setDigit(LED[8], 10);
//  
//  updateET6202();
    byte mem[14] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,255, 255, 255, 255};
    sendCmd(0x40, NULL, 0, false);
    sendCmd(0xC0, mem, 14, false);   
    sendCmd(0x03, NULL, 0, false);
    sendCmd(0x88, NULL, 0, false);





}
 
void loop()                     // run over and over again
{

}

unsigned long sendCmd(byte cmd, byte * data, byte size, byte nocshigh ){
    byte i=0;
    byte j=0;
    noInterrupts();
    digitalWrite(csPin, LOW);    
 
    for (i=0; i<8; i++){
      digitalWrite(clkPin, LOW);
      digitalWrite(dataPin, getBit(cmd, i));
      digitalWrite(clkPin, HIGH);
    }
 
    if(data != NULL){
        for(j=0; j<size; j++){
            for (i=0; i<8; i++){
                digitalWrite(clkPin, LOW);
                digitalWrite(dataPin, getBit(data[j], i));
                digitalWrite(clkPin, HIGH);
            }     
        }
    }
 
    if(nocshigh == false){
      digitalWrite(csPin, HIGH);
      digitalWrite(dataPin, HIGH);
      digitalWrite(clkPin, HIGH);
    }
 
//    unsigned long reskey = 0x00000000;
//    int tmp = 0;
//    int sh = 0;
//    if(nocshigh == 2){
//        pinMode(dataPin, INPUT);
//        for (j=0; j<5; j++){
//            for (i=0; i<8; i++){
//                digitalWrite(clkPin, LOW);
//                digitalWrite(clkPin, HIGH);
//                if(i == 0 || i ==1 || i == 3 || i ==4){
//                    sh = i>2 ? i-1 : i;
//                    tmp = digitalRead(dataPin);
//                    reskey = reskey | (((unsigned long)(tmp & 0x01)) << (j*4 + sh));
//                    //Serial.print((j*4 + sh));
//                    //Serial.print(" ");
//                }
//            }
//        }
//        pinMode(dataPin, OUTPUT);
//        //Serial.println(reskey, HEX);
// 
//        digitalWrite(csPin, HIGH);
//        digitalWrite(dataPin, HIGH);
//        digitalWrite(clkPin, HIGH);
//    }
// 
//    interrupts();
//    return reskey;
}
 
//void setDigit(int digit, int num){
//    //digit - digit for show
//    // num - num of a LED where to show digit
//    if(num < 0 || num > 10){ return; }
//    ET6202DIG[num] = digit;
//}
 
//void updateET6202(){
//    int mem[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0};
//    byte i=0, j=0;
//    int sh=0;
//    for(i=0; i<11; i++){
//        for(j=0; j<7; j++){
//            sh = j==6 ? 2 : (j==4 ? 0 : 6-j);  // 
//            mem[j*2] &= ~(1 << i);  // clear bit
//            mem[j*2] |= (getBit(ET6202DIG[i], sh) << i); //set bit
//        }
//    }
//    sendCmd(0x40, NULL, 0, false);
//    sendCmd(0xC0, mem, 14, false);  
//    sendCmd(0x03, NULL, 0, false);
//    sendCmd(0x88, NULL, 0, false); //88-8F - Brightness level
//}
