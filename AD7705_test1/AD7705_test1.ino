#include <SPI.h>
#include "Filter.h"

ExponentialFilter<float> FilteredTemperature(10, 0);

const int dataReadyPin = 9;
const int chipSelectPin = 10;

#define pinCS chipSelectPin
#define pinDR dataReadyPin

 long acc;
byte accPtr;

//register selection
//RS2 RS1 RS0
static const byte REG_CMM = 0x0; //communication register 8 bit
static const byte REG_SETUP = 0x1; //setup register 8 bit
static const byte REG_CLOCK = 0x2; //clock register 8 bit
static const byte REG_DATA = 0x3; //data register 16 bit, contains conversion result
static const byte REG_TEST = 0x4; //test register 8 bit, POR 0x0
static const byte REG_NOP = 0x5; //no operation
static const byte REG_OFFSET = 0x6; //offset register 24 bit
static const byte REG_GAIN = 0x7; // gain register 24 bit

//channel selection for AD7706 (for AD7705 use the first two channel definitions)
//CH1 CH0
static const byte CHN_AIN1 = 0x0; //AIN1; calibration register pair 0
static const byte CHN_AIN2 = 0x1; //AIN2; calibration register pair 1
static const byte CHN_COMM = 0x2; //common; calibration register pair 0
static const byte CHN_AIN3 = 0x3; //AIN3; calibration register pair 2

//output update rate
//CLK FS1 FS0
static const byte UPDATE_RATE_20 = 0x0; // 20 Hz
static const byte UPDATE_RATE_25 = 0x1; // 25 Hz
static const byte UPDATE_RATE_100 = 0x2; // 100 Hz
static const byte UPDATE_RATE_200 = 0x3; // 200 Hz
static const byte UPDATE_RATE_50 = 0x4; // 50 Hz
static const byte UPDATE_RATE_60 = 0x5; // 60 Hz
static const byte UPDATE_RATE_250 = 0x6; // 250 Hz
static const byte UPDATE_RATE_500 = 0x7; // 500 Hz

//operating mode options
//MD1 MD0
static const byte MODE_NORMAL = 0x0; //normal mode
static const byte MODE_SELF_CAL = 0x1; //self-calibration
static const byte MODE_ZERO_SCALE_CAL = 0x2; //zero-scale system calibration, POR 0x1F4000, set FSYNC high before calibration, FSYNC low after calibration
static const byte MODE_FULL_SCALE_CAL = 0x3; //full-scale system calibration, POR 0x5761AB, set FSYNC high before calibration, FSYNC low after calibration

//gain setting
static const byte GAIN_1 = 0x0;
static const byte GAIN_2 = 0x1;
static const byte GAIN_4 = 0x2;
static const byte GAIN_8 = 0x3;
static const byte GAIN_16 = 0x4;
static const byte GAIN_32 = 0x5;
static const byte GAIN_64 = 0x6;
static const byte GAIN_128 = 0x7;

void setNextOperation(byte reg, byte channel, byte readWrite)
{
  byte r = 0;
  r = reg << 4 | readWrite << 3 | channel;

  digitalWrite(pinCS, LOW);
  SPI.transfer(r);
  digitalWrite(pinCS, HIGH);
}

//Clock Register
//   7      6       5        4        3        2      1      9
//ZERO(0) ZERO(0) ZERO(0) CLKDIS(0) CLKDIV(0)
void writeClockRegister(byte CLKDIS, byte CLKDIV, byte outputUpdateRate)
{
  byte r = CLKDIS << 4 | CLKDIV << 3 | outputUpdateRate;

  digitalWrite(pinCS, LOW);
  SPI.transfer(r);
  digitalWrite(pinCS, HIGH);
}

//Setup Register
//  7     6     5     4     3      2      1      0
//MD10) MD0(0) G2(0) G1(0) G0(0) B/U(0) BUF(0) FSYNC(1)
void writeSetupRegister(byte operationMode, byte gain, byte unipolar, byte buffered, byte fsync)
{
  byte r = operationMode << 6 | gain << 3 | unipolar << 2 | buffered << 1 | fsync;

  digitalWrite(pinCS, LOW);
  SPI.transfer(r);
  digitalWrite(pinCS, HIGH);
}

unsigned int readDataRegister()
{
  digitalWrite(pinCS, LOW);
  byte b1 = SPI.transfer(0xff);
  byte b2 = SPI.transfer(0xff);
  digitalWrite(pinCS, HIGH);

  unsigned int r = b1 << 8 | b2;

  return r;
}

void getReady()
{
  while (digitalRead(pinDR));
}

void init(byte channel, byte gain, byte updRate)
{
  setNextOperation(REG_CLOCK, channel, 0);
  writeClockRegister(0, 1, updRate);
  setNextOperation(REG_SETUP, channel, 0);
  writeSetupRegister(MODE_SELF_CAL, gain, 0, 0, 1);
  setNextOperation(REG_SETUP, channel, 0);
  writeSetupRegister(MODE_SELF_CAL, gain, 0, 0, 0);
  getReady();
}

unsigned int readADResult(byte channel)
{
  setNextOperation(REG_DATA, channel, 1);
  getReady();
  return readDataRegister();
}

double readResult(byte channel)
{
  double VRef = 2.5;
  return ((readADResult(channel) - 32768 + 69));// * 0.0091);// * 1.0 / 32768.0 * VRef;
}


void setup()
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  Serial.begin(9600);
  Serial.println("start");

  pinMode(chipSelectPin, OUTPUT);
  pinMode(dataReadyPin, INPUT_PULLUP);

  init(CHN_AIN1, GAIN_128, UPDATE_RATE_20);
  // init(CHN_AIN2, GAIN_128, UPDATE_RATE_20);

}

// void loop()
// {
// 	// if (accPtr < 20)
// 	// {
// 	// 	accPtr++;
// 	// 	acc += readResult(CHN_AIN1);
// 	// }else{
// 	// 	accPtr = 0;
// 	// 	Serial.println(acc / 20);
// 	// 	acc = 0;
// 	// }

// 	float RawTemperature = readResult(CHN_AIN1);
// 	FilteredTemperature.Filter(RawTemperature);
//     float SmoothTemperature = FilteredTemperature.Current();
// //	acc += readResult(CHN_AIN1);
//   // unsigned long currentMillis = millis();
//  // Serial.println(readResult(CHN_AIN1));
//   // Serial.print("\t");
//   Serial.print(RawTemperature);
//   Serial.println("\t");
//   // Serial.println(readResult(CHN_AIN2));
//   delay(50);
//   // Serial.println(millis() - currentMillis);
// }


void loop()
{
  // unsigned long currentMillis = millis();
  Serial.print(readResult(CHN_AIN1));
  Serial.print("\t");
  Serial.print(readResult(CHN_AIN2) - 8800);
  Serial.println("\t");
  // Serial.println(millis() - currentMillis);
}