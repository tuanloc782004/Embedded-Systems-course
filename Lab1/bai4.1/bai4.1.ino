// bai 4.1
const int n_leds = 10;
const int led_pins[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2};
int time;

void setup() {
  for(int p : led_pins) {
    pinMode(p, OUTPUT);
  }
}
 
void loop() {
  for(int p : led_pins) {
    digitalWrite(p, HIGH);
    time = (analogRead(0) + 400) / 4;
    delay(time);
  }

  for(int i = n_leds - 1; i >= 0; i--) {
    digitalWrite(led_pins[i], LOW);
    time = (analogRead(0) + 400) / 4;
    delay(time);
  }
}