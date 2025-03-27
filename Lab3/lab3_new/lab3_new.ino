#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2       // Chân cảm biến DHT11
#define DHTTYPE DHT11  // Loại cảm biến
#define BUTTON 3       // Chân nút nhấn

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
bool isCelsius = true;  // Chế độ hiển thị
bool isLCDOn = true;    // Trạng thái màn hình

void setup() {
    pinMode(BUTTON, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    dht.begin();
    Serial.begin(9600); // Khởi động Serial Monitor
}

void loop() {
    handleButtonPress();
    
    // Khai báo biến nhiệt độ ở đây để dùng chung
    float tempC = dht.readTemperature();
    float tempF = dht.readTemperature(true);
    
    if (isLCDOn) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nhiet do:");
        lcd.setCursor(0, 1);
        lcd.print(isCelsius ? tempC : tempF);
        lcd.print("\xDF");
        lcd.print(isCelsius ? "C" : "F");
    } else {
        lcd.clear();
    }

    // In ra Serial Monitor để kiểm tra
    Serial.print("Nhiet do: ");
    Serial.print(isCelsius ? tempC : tempF);
    Serial.println(isCelsius ? " C" : " F");

    delay(50); }

void handleButtonPress() {
    static bool lastButtonState = HIGH;
    static unsigned long firstPressTime = 0;
    static bool waitForSecondPress = false;
    
    bool buttonState = digitalRead(BUTTON);
    
    if (buttonState == LOW && lastButtonState == HIGH) {
        unsigned long currentTime = millis();

        if (!isLCDOn) { 
            isLCDOn = true;
            lcd.backlight();
        } else {
            if (waitForSecondPress && (currentTime - firstPressTime < 500)) {
                isLCDOn = false;
                lcd.noBacklight();
                waitForSecondPress = false;
            } else {
                waitForSecondPress = true;
                firstPressTime = currentTime;
                isCelsius = !isCelsius;
            }
        }
    }
    
    lastButtonState = buttonState;
}
