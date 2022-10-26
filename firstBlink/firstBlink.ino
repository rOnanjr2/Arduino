
char mosfetPin=9;

void setup()
{
  pinMode(12, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(14,INPUT);
  analogReference(DEFAULT);
  TCCR1A |= (1 << WGM10)|(1 << WGM11);
  TCCR1B |= (1 << WGM12);
}

void loop()
{
  int bright (analogRead (0));
  analogWrite (mosfetPin, bright);
  delay(50);
}
