/*
  Module RTC
  GND -> GND
  VCC -> 5V
  SDA -> A4/SDA
  SCL -> A5/SCL
*/
#include "RTClib.h"
RTC_DS1307 rtc;

DateTime now;
DateTime last_log;
TimeSpan timespan;

/*
  Module SD
  GND  ->  GND
  VCC  ->  5V
  MISO -> D12 / D50
  MOSI -> D11 / D51
  SCK  -> D13 / D52
  CS   -> D10 / D53 Configurable
*/
#include <SdFat.h>
#define CS_PIN 10
#define FILENAME "a.txt"

SdFat sd;
SdFile myFile;

/* Cảm biến DHT11 */
#include <DHT.h>
#define DHTPIN 2      // Chân dữ liệu của DHT11 nối với chân 2 Arduino
#define DHTTYPE DHT11 // Loại cảm biến
DHT dht(DHTPIN, DHTTYPE);
float temp_C;

char buffer[100];

void setup() {
  Serial.begin(9600);
  delay(500);
  initialize_SD();
  delay(500);
  initialize_RTC();
  delay(500);
  dht.begin();  // Khởi động DHT11
  
  // delete_file(FILENAME); // comment this line for append else overwrite

  last_log = rtc.now() - TimeSpan(10);
}

void loop() {
  now = rtc.now();
  timespan = now - last_log;
  delay(100);
  if (timespan.seconds() < 10) {
    return;
  }
  last_log = now;

  temp_C = dht.readTemperature();  // Đọc nhiệt độ từ DHT11
  if (isnan(temp_C)) { // Kiểm tra nếu đọc lỗi
    Serial.println("Lỗi! Không thể đọc nhiệt độ từ DHT11");
    return;
  }

  sprintf(buffer, "%s Nhiệt độ: %.1f°C", datetime_to_str(now).c_str(), temp_C);
  Serial.println("write to file '" + String(FILENAME) +  "': " + String(buffer));
  write_to_file(FILENAME, buffer);
}

void initialize_SD() {
  Serial.println("\nInitializing SD card...");
  while (!sd.begin(CS_PIN, SPI_HALF_SPEED)) {
    Serial.println("\tInitialization failed!");
    delay(500);
  }
  Serial.println(" > Initialization successful!");
}

void write_to_file(const char* filename, const char* data) {
  uint8_t oflag = O_WRITE | O_CREAT | O_APPEND;
  if (myFile.open(filename, oflag)) {
    myFile.println(data);
    myFile.close();
  } else {
    Serial.println("Failed to open file for writing");
  }
}

void read_all_lines(const char* filename) {
  if (myFile.open(filename, O_READ)) {
    uint32_t file_size = myFile.fileSize();
    while (myFile.curPosition() < file_size) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("Failed to open file for reading");
  }
}

void delete_file(const char* filename) {
  if (sd.exists(filename) && sd.remove(filename))
    Serial.println("File deleted successfully");
  else
    Serial.println("Failed to delete file");
}

void initialize_RTC() {
  Serial.println("\nInitialize RTC...");
  while (!rtc.begin()) {
    Serial.println("\tCouldn't find RTC");
    delay(1000);
  }
  Serial.println(" > Initialize RTC successfully!");
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(20));
  }
}

String datetime_to_str(DateTime dt) {
  sprintf(buffer, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
  return buffer;
}
