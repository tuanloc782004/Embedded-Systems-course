/*
  SDA  -->  D10       Configurable
  SCK  -->  D13 / D52
  MOSI -->  D11 / D51
  MISO -->  D12 / D50
  IRQ  -->
  GND  -->  GND
  RST  -->   D9       Configurable
  3.3V --> 3.3V
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SDA_PIN 10
MFRC522 mfrc522(SDA_PIN, RST_PIN);


#define N_AUTHORIZED 1
int AUTHORIZED_UID[N_AUTHORIZED][4] = {
  { 0xC3, 0x45, 0x65, 0xA7 },
  // { 0x43, 0x70, 0x43, 0x16 },
};

int UID[4];
int LED_PIN = 3;
int authorized_count = 0;
int LED_STATE = LOW;
bool is_blink = false;
int current_timestamp;
size_t last_blink_timestamp = 0;
size_t last_authorized_timestamp = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  SPI.begin();                        // Init SPI bus
  mfrc522.PCD_Init();                 // Init MFRC522
  delay(4);                           // Optional delay. Some board do need more time after init to be ready.
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  if (is_blink) {
    int current_timestamp = millis();
    if (current_timestamp - last_blink_timestamp >= 200) {
      digitalWrite(LED_PIN, LED_STATE = (LED_STATE == LOW));
      last_blink_timestamp = current_timestamp;
    }
  }

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID: ");
  for (int i = 0; i < mfrc522.uid.size; i++) {
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.print(UID[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  // check authorized

  if (is_authorized()) {
    Serial.println("True");
    authorized_count++;
    if (authorized_count == 2) {  // just from turn off -> on and authorize 2 time in a row
      last_authorized_timestamp = 0;
    }
    current_timestamp = millis();
    if (current_timestamp - last_authorized_timestamp <= 2000) {
      if (LED_STATE == HIGH) {
        digitalWrite(LED_PIN, LED_STATE = LOW);
        authorized_count = 0;
      } else {
        digitalWrite(LED_PIN, LED_STATE = HIGH);
      }
    } else {
      digitalWrite(LED_PIN, LED_STATE = HIGH);
    }
    last_authorized_timestamp = current_timestamp;

    is_blink = false;
  } else {
    Serial.println("False");
    is_blink = true;
    authorized_count = 0;
    last_authorized_timestamp = 0;
  }

  mfrc522.PICC_HaltA();  // stop read
  mfrc522.PCD_StopCrypto1();
}

bool is_authorized() {
  int *cur_uid;
  for (int i = 0, match_count; i < N_AUTHORIZED; i++) {
    cur_uid = AUTHORIZED_UID[i];
    match_count = 0;
    for (int j = 0; j < 4; j++) {
      if (cur_uid[j] == UID[j])
        match_count++;
      else
        break;
    }
    if (match_count == 4)
      return true;
  }
  return false;
}