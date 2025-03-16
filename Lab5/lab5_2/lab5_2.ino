#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 3  // LED nối với chân D3

MFRC522 mfrc522(SS_PIN, RST_PIN);

String validUID = "C3 45 65 A7";  // UID hợp lệ
bool isLedOn = false;             // Trạng thái LED
bool lastScanValid = false;        // Theo dõi lần quét trước
bool blinkError = false;           // Nhấp nháy LED nếu sai thẻ

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // Tắt LED ban đầu
    Serial.println("Đưa thẻ vào để kiểm tra...");
}

void loop() {
    // Nếu quét sai, LED nhấp nháy mãi
    if (blinkError) {
        digitalWrite(LED_PIN, HIGH);
        delay(300);
        digitalWrite(LED_PIN, LOW);
        delay(300);
    }

    if (!mfrc522.PICC_IsNewCardPresent()) return;
    if (!mfrc522.PICC_ReadCardSerial()) return;

    // Đọc UID thẻ
    String UID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) UID += "0"; // Thêm số 0 nếu cần
        UID += String(mfrc522.uid.uidByte[i], HEX);
        UID += " ";
    }
    UID.trim(); // Xóa khoảng trắng cuối

    Serial.print("UID quét được: ");
    Serial.println(UID);

    // Kiểm tra thẻ hợp lệ
    if (UID.equalsIgnoreCase(validUID)) {
        blinkError = false; // Dừng nhấp nháy nếu quét đúng thẻ
        if (isLedOn && lastScanValid) {
            Serial.println("Thẻ hợp lệ 2 lần liên tiếp! Tắt LED.");
            digitalWrite(LED_PIN, LOW);
            isLedOn = false;
        } else {
            Serial.println("Thẻ hợp lệ! Bật LED.");
            digitalWrite(LED_PIN, HIGH);
            isLedOn = true;
        }
        lastScanValid = true; // Đánh dấu lần quét này là hợp lệ
    } else {
        Serial.println("Thẻ không hợp lệ! LED nhấp nháy mãi.");
        blinkError = true; // Bật chế độ nhấp nháy nếu sai thẻ
        lastScanValid = false; // Đánh dấu lần quét này không hợp lệ
    }

    mfrc522.PICC_HaltA();
}