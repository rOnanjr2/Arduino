#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 14
int correction = 0;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
uint8_t sensor1[8] = { 0x28, 0x02, 0x00, 0x07, 0xA7, 0xD2, 0x01, 0xED };
uint8_t sensor2[8] = { 0x28, 0xFF, 0xD0, 0xBE, 0x20, 0x17, 0x04, 0xE5 };
uint8_t sensor3[8] = { 0x28, 0xFF, 0xC3, 0x37, 0x21, 0x17, 0x04, 0x36 };

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}



void loop(void)
{
  sensors.requestTemperatures();
  
  Serial.print("Sensor 1: ");
  correction = 6;
  printTemperature(sensor1);
  
  Serial.print("Sensor 2: ");
  correction = 0;
  printTemperature(sensor2);
  
  Serial.print("Sensor 3: ");
  correction = 0;
  printTemperature(sensor3);
  
  Serial.println();
  delay(100);
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print((tempC) + correction);
//  Serial.print((char)176);
  Serial.println(" ºC");
//  Serial.print(DallasTemperature::toFahrenheit(tempC));
//  Serial.print((char)176);
//  Serial.println("F");
}
