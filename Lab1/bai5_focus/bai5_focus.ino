const int led_pin = 3;  // Tất cả đèn LED sẽ cắm chung vào chân 3

void setup() {
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  int brightness = map(analogRead(A0), 0, 1023, 0, 255); // Biến trở điều chỉnh độ sáng
  Serial.println(brightness);

  // Điều chỉnh độ sáng chỉ trên chân số 3
  analogWrite(led_pin, brightness);

  delay(50); // Giữ phản hồi nhanh nhưng không quá gấp
}
