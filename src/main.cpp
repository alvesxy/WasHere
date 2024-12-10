#include <MFRC522.h>
#include <SPI.h>

#include <ReadFile.h>
#include <Arduino.h>

MFRC522 identificator;

void setup() {

    Serial.begin(115200);
    SPI.begin();

    ReadFile read = ReadFile("/application.json");
    
    identificator = MFRC522(read.getSS(), read.getRST());
    identificator.PCD_Init();
    
}

void loop() {

    if (!identificator.PICC_IsNewCardPresent() || !identificator.PICC_ReadCardSerial()) {
        return;
    }

    String identifier;

    for (byte i = 0; i < identificator.uid.size; i++) {
        identifier += identificator.uid.uidByte[i], DEC;
    }

    identificator.PICC_HaltA();

}