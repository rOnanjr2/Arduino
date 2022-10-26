void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 //Serial.begin (9600);
 pinMode (9,OUTPUT);
 Serial.begin (9600);
}
char pin=9;
// the loop function runs over and over again forever
void loop() {
  int i=analogRead(0);
  Serial.println(i);
  delay(333);
  }
