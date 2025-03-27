// thay đổi nhiệt độ mới ghi đè
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <DHT.h>

//SCL 5, SDA 4
//MISO 12, MOSI 11, SCK 13, CS 10
#define DHTPIN 6
#define DHTTYPE DHT11
#define SD_CS 10
#define FILENAME "log.txt"

RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);
File dataFile;

float lastTemperature = -1000.0; // Lưu nhiệt độ cũ để so sánh

void setup() {
    Serial.begin(9600);
    
    // Khởi động DHT11
    dht.begin();
    
    // Khởi động RTC
    if (!rtc.begin()) {
        Serial.println("❌ Không tìm thấy RTC!");
        while (1);
    }

    // Luôn cập nhật thời gian từ máy tính
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Khởi động thẻ SD
    if (!SD.begin(SD_CS)) {
        Serial.println("❌ Không tìm thấy module SD!");
        while (1);
    }
    
    Serial.println("✅ Hệ thống đã sẵn sàng.");
}


void loop() {
    DateTime now = rtc.now();
    digitalWrite(SD_CS, HIGH);
    delay(10);

    float temperature = dht.readTemperature();

    if (isnan(temperature)) {
        Serial.println("❌ Lỗi! Không thể đọc DHT11.");
        temperature = -99.9;
    }

    // Nếu nhiệt độ không thay đổi, thoát vòng lặp
    if (temperature == lastTemperature) {
        return;
    }

    // Cập nhật nhiệt độ mới
    lastTemperature = temperature;

    Serial.print("📅 ");
    Serial.print(now.day()); Serial.print("/");
    Serial.print(now.month()); Serial.print("/");
    Serial.print(now.year()); Serial.print(" ");
    Serial.print(now.hour()); Serial.print(":");
    Serial.print(now.minute()); Serial.print(":");
    Serial.print(now.second());

    Serial.print(" 🌡 ");
    Serial.print(temperature);
    Serial.println(" °C");

    // 🔥 **Cách ghi đè: Xóa file cũ trước khi ghi mới**
    SD.remove(FILENAME); // Xóa file cũ
    dataFile = SD.open(FILENAME, FILE_WRITE); // Tạo file mới
    if (dataFile) {
        dataFile.print(now.day()); dataFile.print("/");
        dataFile.print(now.month()); dataFile.print("/");
        dataFile.print(now.year()); dataFile.print(" ");
        dataFile.print(now.hour()); dataFile.print(":");
        dataFile.print(now.minute()); dataFile.print(":");
        dataFile.print(now.second());

        dataFile.print(" - Nhiet do: ");
        dataFile.print(temperature);
        dataFile.println(" °C");

        dataFile.close();
        Serial.println("📝 Dữ liệu đã được ghi đè vào " FILENAME);
    } else {
        Serial.println("❌ Lỗi khi mở file!");
    }

    SD.end();
    delay(10);
    SD.begin(SD_CS);
    delay(10000);
}
