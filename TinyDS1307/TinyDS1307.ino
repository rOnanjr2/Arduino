#include <TinyDS1307.h>


void setup() {
  Serial.begin(9600);
  TinyWireM.begin();                    // initialize I2C lib
  tinyrtc.begin();                      // inititalize DS1307
//    if (!tinyrtc.isrunning()) {           // if this is first run (or there's no battery on DS1307)
//        tinyrtc.adjust(2014,3,27,21,13,59); // set up time Y, M, D, HH, MM, SS
//    }
}

void loop() {
  tinyrtc.read();     // read data from DS1307 to get fresh/actual values
    unsigned char seconds = tinyrtc.second(); // 0 - 59
    unsigned char minutes = tinyrtc.minute(); // 0 - 59
    unsigned char hours   = tinyrtc.hour();   // 0 - 23
    unsigned char day     = tinyrtc.day();    // 1 - 31
    unsigned char month   = tinyrtc.month();  // 1 - 12
    unsigned short year   = tinyrtc.year();   // 2000 - ...
    Serial.print(seconds);
    delay(1000);

}
