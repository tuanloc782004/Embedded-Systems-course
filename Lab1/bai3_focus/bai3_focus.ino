// bai 3
const int n_leds = 10;
const int led_pins[] = {11, 9, 7, 5, 3, 2, 4, 6, 8, 10};

void setup() {
  for(int p : led_pins) {
    pinMode(p, OUTPUT);
  }
}

void loop() {
  for(int i = 0; i < n_leds; i++) {
    digitalWrite(led_pins[i], HIGH);
    delay(300);
  }

  for(int i = 0; i < n_leds; i++) {
    digitalWrite(led_pins[i], LOW);
    delay(300);
  }
}