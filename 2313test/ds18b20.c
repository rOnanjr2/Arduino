#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ds18b20.h"


/*
 * ds18b20 - инициализация
 */
uint8_t resetDS18B20() {
  uint8_t i;

  // импульс сброса, минимум 480?s
  DS18B20_PORT &= ~(1 << DS18B20_DQ);
  DS18B20_DDR |= (1 << DS18B20_DQ); // выход
  _delay_us(480);

  // Когда DS18B20 обнаруживает положительный перепад, он ждет от 15?s до 60?s
  DS18B20_DDR &= ~(1 << DS18B20_DQ); // вход
  _delay_us(60);

  // и затем передает импульс присутствия, перемещая шину в логический «0» на длительность от 60?s до 240?s.
  i = (DS18B20_PIN & (1 << DS18B20_DQ));
  _delay_us(420);

  //return the read value, 0=ok, 1=error
  return i;
}

/*
 * написать один бит
 */
inline void writeBitDS18B20(uint8_t bit) {
  // логический «0» на 1?s
  DS18B20_PORT &= ~(1 << DS18B20_DQ);
  DS18B20_DDR |= (1 << DS18B20_DQ); // выход
  _delay_us(1);

  // если нужно написать 1, нужно освободить линию (если не - держать низкий уровень)
  if (bit) {
    DS18B20_DDR &= ~(1 << DS18B20_DQ); // вход
  }

  // ждать 60?s и освободить линию
  _delay_us(60);
  DS18B20_DDR &= ~(1 << DS18B20_DQ); // вход
}

/*
 * прочитать один бит
 */
uint8_t readBitDS18B20(void) {
  uint8_t bit = 0;

  // логический «0» на 1?s
  DS18B20_PORT &= ~(1 << DS18B20_DQ);
  DS18B20_DDR |= (1 << DS18B20_DQ); // вход
  _delay_us(1);

  // освободить линию и ждать 14?s
  DS18B20_DDR &= ~(1 << DS18B20_DQ); // вход
  _delay_us(14);

  // прочитать значение
  if (DS18B20_PIN & (1 << DS18B20_DQ)) {
    bit = 1;
  }

  // ждать 45?s и вернуть значение
  _delay_us(45);
  return bit;
}

/*
 * написать один байт
 */
void writeByteDS18B20(uint8_t byte) {
  uint8_t i = 8;
  while (i--) {
    writeBitDS18B20(byte & 1);
    byte >>= 1;
  }
}

/*
 * прочитать один байт
 */
uint8_t readByteDS18B20(void) {
  uint8_t i = 8, n = 0;
  while (i--) {
    n >>= 1;
    n |= (readBitDS18B20() << 7);
  }
  return n;
}

/*
 * получить температуру
 */
double getTempDS18B20() {
  uint8_t temperatureL;
  uint8_t temperatureH;
  double retd = 0;

  if (DS18B20_STOPINTERRUPTONREAD == 1) {
    cli();
  }

  resetDS18B20(); // сброс
  writeByteDS18B20(DS18B20_CMD_SKIPROM); // пропуск ПЗУ(ROM)
  writeByteDS18B20(DS18B20_CMD_CONVERTTEMP); // начать преобразование показаний температуры

  while (!readBitDS18B20()); // ждать, пока преобразование не завершится

  resetDS18B20(); // сброс
  writeByteDS18B20(DS18B20_CMD_SKIPROM); // пропуск ПЗУ(ROM)
  writeByteDS18B20(DS18B20_CMD_RSCRATCHPAD); // читать scratchpad

  // прочитать 2 байта из scratchpad
  temperatureL = readByteDS18B20();
  temperatureH = readByteDS18B20();

  if (DS18B20_STOPINTERRUPTONREAD == 1) {
    sei();
  }

  // преобразовать полученное 12 битное значение
  retd = ((temperatureH << 8) + temperatureL) * 0.0625;

  return retd;
}