void setup()
{
  pinMode(9, OUTPUT);
  pinMode(14,INPUT);
  TCCR1A |= (1 << WGM10)|(1 << WGM11);  //Set Timer1 to "10-bit fast PWM" mode
  TCCR1B |= (1 << WGM12);               //Set Timer1 to "10-bit fast PWM" mode
}

void loop()
{
  short bright (analogRead (0));
  analogWrite (9, bright);
  delay(5);
}
