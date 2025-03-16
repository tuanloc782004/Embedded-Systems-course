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
unsigned long lastPressTime = 0;
int pressCount = 0;

void setup() {
    pinMode(BUTTON, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    dht.begin();
}

void loop() {
    handleButtonPress();
    
    if (isLCDOn) {
        float tempC = dht.readTemperature();
        float tempF = dht.readTemperature(true);
        
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
    delay(50);
}

void handleButtonPress() {
    static bool lastButtonState = HIGH;
    static unsigned long firstPressTime = 0;
    static bool waitForSecondPress = false;
    
    bool buttonState = digitalRead(BUTTON);
    
    if (buttonState == LOW && lastButtonState == HIGH) {
        unsigned long currentTime = millis();

        if (!isLCDOn) { 
            // Nếu màn hình đang tắt, bấm nút sẽ bật lại ngay
            isLCDOn = true;
            lcd.backlight();
        } else {
            // Nếu màn hình đang bật
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
