#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 21
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Ready - Show RFID card");
}

void loop() {
  // Main Program (Run until RFID detected)
  while(!checkRFID()) {
    Serial.println("Running main program...");
    delay(1000);
  }

  // Secondary Program (Execute when RFID detected)
  while(true) {
    Serial.println("Executing secondary commands...");
    delay(1000);
  }
}

bool checkRFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) return false;
  if (!mfrc522.PICC_ReadCardSerial()) return false;
  
  Serial.print("Detected UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  mfrc522.PICC_HaltA();
  return true;
}



