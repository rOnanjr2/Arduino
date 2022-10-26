void setup() {
   pinMode(2, OUTPUT); // объявляем пин 10 как выход
   pinMode(1, OUTPUT);
}

void loop() {
  tone (2, 5000);
  delay(100);
  noTone(2);
  for(int i = 0; i < 100 ; i++) {
    delay(10);
    digitalWrite(1, 1);
    delay(990);
    digitalWrite(1, 0);
    }
}
