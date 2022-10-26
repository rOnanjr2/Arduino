#define M0 2
#define M1 3
#define M2 4
#define STP 5
#define DIR 6
#define UP HIGH
#define DOWN LOW

void steps(int count, int dir) {
  digitalWrite(DIR, dir);
  for (int i = count; i > 0; i--) {
    digitalWrite(STP, HIGH);
    delay(100);
    digitalWrite(STP, LOW);
    delay(100);
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  steps(5, UP);
  delay(2000);
}
