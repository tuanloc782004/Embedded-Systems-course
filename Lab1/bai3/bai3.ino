// bai 3
const int n_leds = 10;
const int led_pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void setup() {
  for(int p : led_pins) {
    pinMode(p, OUTPUT);
  }
}

void loop() {
  for(int i = 0; i < n_leds; i+=2) {
    digitalWrite(led_pins[i], HIGH);
    delay(300);
  }

  for(int i = 1; i < n_leds; i+=2) {
    digitalWrite(led_pins[i], HIGH);
    delay(300);
  }

  for(int i = 11; i >= 0; i-=2) {
    digitalWrite(led_pins[i], LOW);
    delay(300);
  }

  for(int i = 10; i >= 0; i-=2) {
    digitalWrite(led_pins[i], LOW);
    delay(300);
  }  
}