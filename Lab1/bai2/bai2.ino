// bai 2
const int n_leds = 10;
const int led_pins[] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 };

void setup() {
  for (int p : led_pins) {
    pinMode(p, OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < n_leds / 2; i++) {
    digitalWrite(led_pins[i], HIGH);
    digitalWrite(led_pins[n_leds - i - 1], HIGH);
    delay(300);
  }

  for (int i = n_leds / 2 - 1; i >= 0; i--) {
    digitalWrite(led_pins[i], LOW);
    digitalWrite(led_pins[n_leds - i - 1], LOW);
    delay(300);
  }
}