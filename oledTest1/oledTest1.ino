#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, 4);

byte blink;
byte arrPtr;
#define arrLength 86
byte arr[arrLength];
uint8_t battLevel;

void setup() {
//  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.setTextColor(SSD1306_WHITE);

//  for (byte i = 0; i <= 100; i++){

//  }
}
void loop() {
	display.clearDisplay();
	checkBatt();
	drawInfo();
	drawBatLevelVert();
	drawGraphVoltage();
	delay(50);
	display.display();
}




void drawBatLevelVert(){
	display.setTextSize(2);

	if(blink < 10){
		blink++;
	}else {
		blink = 0;
	}

	display.drawRoundRect(92, 11, 36, 53, 3, WHITE);
	display.drawRoundRect(100, 5, 20, 7, 3, WHITE);

	for(uint8_t i = 0; i < (battLevel == 100 ? 9 : battLevel /10) + 1; i++) {
		if(i < 10 && i > 0){
			display.fillRect(94, 63 - i * 5, 32, 4, WHITE);
		}
		if((blink > 4 || battLevel == 100) && battLevel > 4){
			display.fillRect(94, 63 - (i + 1) * 5, 32, 4, WHITE);
		}
	}

	if(battLevel < 100){
		display.fillRect(96, 28, 28, 19, BLACK);
		display.setCursor(battLevel > 9 ? 99 : 105, 31);
		display.println(battLevel);
	}
}

void drawGraphVoltage(){
  #define graphXPos 89
  #define graphYPos 60
	for(uint8_t x = 0; x < 85; x++){
		if(x % 2 == 0){
			display.drawPixel(graphXPos - x, 63, WHITE);
		}
		if(x % 4 == 0){
			display.drawPixel(graphXPos - x, 62, WHITE);
			display.drawPixel(graphXPos - x, 40, WHITE);
		}
		if(x % 20 == 0){
			display.drawPixel(graphXPos - x, 61, WHITE);
			display.drawPixel(graphXPos - x, 60, WHITE);
			for(uint8_t y = 0; y < 37; y++){
				if(y % 4 == 0){
					display.drawPixel(graphXPos - x, graphYPos - y, WHITE);
				}
				
			}
		}
	}
	for(uint8_t x = 0; x < 37; x++){
		if(x % 2 == 0){
			display.drawPixel(0, graphYPos - x, WHITE);
		}
		if(x % 4 == 0){
			display.drawPixel(1, graphYPos - x, WHITE);
		}
		if(x % 20 == 0){
			display.drawPixel(2, graphYPos - x, WHITE);
		}
	}

	for(uint8_t i = 0; i < arrLength; i++){
		display.drawPixel(graphXPos - i, map(arr[i < arrPtr ? arrPtr - i - 1 : arrLength - (i - arrPtr + 1 )], 0, 100, 60, 24), WHITE);
	}



	if(1){
		arr[arrPtr] = battLevel;
		if (arrPtr < arrLength - 1)
		{
			arrPtr++;
		}else {
			arrPtr = 0;
		}
		
	}
}

void drawInfo(){
	long battVoltage = map(analogRead(A3), 10, 1014, 2000, 2640);
	long battCurrent = battVoltage * 2;
	long battPower = battVoltage * battCurrent / 10000;
	display.setTextSize(1);
	display.setCursor(0, 0);
	display.println(String(battVoltage / 100) + "." + String(battVoltage % 100));
	display.println(String(battCurrent / 100) + "." + String(battCurrent % 100));
	display.println(String(battPower));
	display.setCursor(30, 0);
	display.println("V");
	display.setCursor(30, 8);
	display.println("A");
	display.setCursor(30, 16);
	display.println("W");
}


void checkBatt (){
	battLevel = map(analogRead(A3), 10, 1014, 0, 100);
}























// void drawBattLevel(uint8_t battLevel){

// 	if(blink < 10){
// 		blink++;
// 	}else {
// 		blink = 0;
// 	}

// 	String str = String(battLevel) + "%";

// 	display.drawRect(63, 29, 65, 35, WHITE);
// 	display.drawRect(67, 23, 7, 7, WHITE);
// 	display.drawRect(118, 23, 7, 7, WHITE);
// 	display.drawLine(120, 26, 122, 26, WHITE);
// 	display.drawLine(69, 26, 71, 26, WHITE);
// 	display.drawLine(121, 25, 121, 27, WHITE);

// 	for(uint8_t i = 0; i < (battLevel == 100 ? 9 : battLevel /10) + 1; i++) {
// 		if(i < 10 && i > 0){
// 			display.fillRect(60 + (i) * 6, 32, 5, 29, WHITE);
// 		}

// 		if((blink > 4 || battLevel == 100) && battLevel > 4){
// 			display.fillRect(60 + (i + 1) * 6, 32, 5, 29, WHITE);
// //      display.fillRect(72, 32, 5, 29, WHITE);
// 		}
// 	}


// 	byte textPos = 75 + (3 - str.length()) * 5;
// 	display.fillRect(textPos, 37, (str.length() * 10) + 11, 19, BLACK);


// 	display.setTextSize(2);
// 	display.setCursor(textPos + (6 - str.length()), 39);


// 	display.println(str);
// }
