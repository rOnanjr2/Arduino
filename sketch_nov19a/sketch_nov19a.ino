
#define BUTTON_1 7
#define BUTTON_2 8

void setup() {
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  char button_1 = digitalRead(BUTTON_1);
  char button_2 = digitalRead(BUTTON_2);
  char message[80];
  sprintf(message, "button 1: %d, button 2: %d", button_1, button_2);
  Serial.println(message);
  delay(500);
}
