#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  5   // GPIO5
#define SS_PIN   21  // GPIO21 (free pin)
// Default SPI pins: MOSI=23, MISO=19, SCK=18

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial connection
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID Reader Ready - Show a card");
}

void loop() {
  // Reset the loop if no new card present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(50);  // Small delay to reduce CPU usage
    return;
  }

  // Verify card selection
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read the first data block (Block 4)
  readBlock(4);

  // Halt PICC and stop crypto
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void readBlock(byte blockAddr) {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF; // Default key

  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
    Serial.println("Authentication failed");
    return;
  }

  // Read block
  if (mfrc522.MIFARE_Read(blockAddr, buffer, &size) != MFRC522::STATUS_OK) {
    Serial.println("Reading failed");
    return;
  }

  // Print the block contents
  Serial.print("Message: ");
  // Print only readable ASCII characters
  for (byte i = 0; i < 16; i++) {
    if (buffer[i] >= 32 && buffer[i] <= 126) { // Printable ASCII range
      Serial.write(buffer[i]);
    } else {
      Serial.print(" "); // Replace non-printable chars with space
    }
  }
  Serial.println();
}