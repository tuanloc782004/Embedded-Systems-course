#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9   // Chân RST của module RFID
#define SDA_PIN 10  // Chân SDA (SS)
MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup() {
    Serial.begin(9600);  // Khởi động Serial Monitor
    SPI.begin();         // Khởi động giao tiếp SPI
    mfrc522.PCD_Init();  // Khởi động module RFID
    Serial.println("Đặt thẻ RFID lên đầu đọc...");
}

void loop() {
    // Kiểm tra xem có thẻ nào được đặt lên không
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Đọc dữ liệu từ thẻ
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print("UID của thẻ: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    mfrc522.PICC_HaltA();       // Dừng giao tiếp với thẻ
    mfrc522.PCD_StopCrypto1();  // Dừng mã hóa
}
