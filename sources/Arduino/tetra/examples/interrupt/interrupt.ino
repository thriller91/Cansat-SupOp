#define INTERRUPT_PIN 1

volatile unsigned long start_missions = 0;
volatile boolean start_flag = false;


void setup() {
  Serial.begin(9600);
  attachInterrupt(INTERRUPT_PIN, start_counter, RISING);
}

void loop() {
  while(!start_flag);
  Serial.println(millis() - start_missions);
  delay(1000);
}

void start_counter() {
  start_missions = millis();
  start_flag = true;
  detachInterrupt(INTERRUPT_PIN);
}
