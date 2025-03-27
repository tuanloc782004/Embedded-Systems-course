#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <DHT.h>

// Chân kết nối phần cứng
#define DHTPIN 6          // Chân kết nối DHT11
#define DHTTYPE DHT11     // Loại cảm biến DHT11
#define SD_CS 10          // Chân Chip Select của module SD
#define FILENAME "minh.txt" // Tên file lưu dữ liệu

RTC_DS1307 rtc;  // Đối tượng RTC
DHT dht(DHTPIN, DHTTYPE); // Đối tượng cảm biến DHT11
File dataFile;  // Đối tượng file

float lastTemperature = -1000; // Lưu giá trị nhiệt độ trước đó

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

    // Xóa dữ liệu cũ trong file khi khởi động
    dataFile = SD.open(FILENAME, O_WRITE | O_TRUNC);
    if (dataFile) {
        dataFile.close();
        Serial.println("🗑 File đã được xóa dữ liệu cũ.");
    } else {
        Serial.println("❌ Không thể xóa dữ liệu cũ.");
    }
}

void loop() {
    // Đọc thời gian từ RTC
    DateTime now = rtc.now();

    // Ngắt kết nối SD để tránh lỗi khi đọc DHT11
    digitalWrite(SD_CS, HIGH);
    delay(10);

    // Đọc nhiệt độ từ DHT11
    float temperature = dht.readTemperature();
    
    if (isnan(temperature)) {
        Serial.println("❌ Lỗi! Không thể đọc dữ liệu từ DHT11.");
        return;
    }

    // Kiểm tra nếu nhiệt độ thay đổi thì mới ghi vào thẻ SD
    if (temperature != lastTemperature) {
        lastTemperature = temperature; // Cập nhật nhiệt độ mới nhất

        // Hiển thị trên Serial Monitor
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

        // Ghi dữ liệu vào thẻ SD
        dataFile = SD.open(FILENAME, O_WRITE | O_TRUNC);
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
            Serial.println("📝 Dữ liệu mới đã được ghi vào " FILENAME);
        } else {
            Serial.println("❌ Lỗi khi mở file!");
        }

        // Reset module SD để tránh lỗi
        SD.end();
        delay(10);
        SD.begin(SD_CS);
    }

    delay(5000); // Đọc lại sau mỗi 5 giây
}
