#include <SPI.h>                             // Подключаем библиотеку SPI
#include <Adafruit_GFX.h>                    // Подключаем библиотеку Adafruit_GFX
#include <Max72xxPanel.h>                    // Подключаем библиотеку Max72xxPanel
int pinCS = 9;                               // Указываем к какому выводу подключен контакт CS
int numberOfHorizontalDisplays = 1;          // Количество матриц по горизонтали
int numberOfVerticalDisplays = 4;            // Количество матриц по-вертикали

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "";
int wait = 100;                             // интервал, чем меньше тем бытрее бежит строка
int spacer = 1;                             // Промежуток между символами (кол-во точек)
int width = 5 + spacer;                     // Ширина шрифта составляет 5 пикселей

void setup() {
  Serial.begin(9600);
  tape = utf8rus("Пусть будет классным настроение. Моя мамуля, с днем рождения!");
  matrix.setIntensity(5);                    // Задаем яркость от 0 до 15
  matrix.setRotation(3);                     // Направление текста 1,2,3,4
}

void loop() {

  for ( int i = 0 ; i < width * tape.length() + matrix.width() - spacer; i++ )
  {
    matrix.fillScreen(LOW);

    int letter = i / width;                   // номер символа выводимого на матрицу

    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2;         // отцентрировать текст по вертикали

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
    matrix.write();                       // выведим значения на матрицу
    delay(wait);
  }
}

/* Функция перекодировки русских букв из UTF-8 в Win-1251 */
String utf8rus(String source)
{
  int i, k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;
    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
            n = source[i]; i++;
            if (n == 0x81) {
              n = 0xA8;
              break;
            }
            if (n >= 0x90 && n <= 0xBF) n = n + 0x2F;
            break;
          }
        case 0xD1: {
            n = source[i]; i++;
            if (n == 0x91) {
              n = 0xB7;
              break;
            }
            if (n >= 0x80 && n <= 0x8F) n = n + 0x6F;
            break;
          }
      }
    }
    m[0] = n; target = target + String(m);
  }
  return target;
}
