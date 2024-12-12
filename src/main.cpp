/**
 * @link https://www.linkedin.com/in/c-alves-f?
 * 
 * Table to connect esp32 with rfid
 * 
 * ESP32 | RFID
 * 3V3 | 3.3v
 * GND | GND
 * defined in application.json as RST | RST
 * defined in application.json as SS | SDA
 * 19 | MISO
 * 23 | MOSI
 * 18 | SCK
*/

#include <MFRC522.h>
#include <SPI.h>

#include <Arduino.h>

#include <ReadFile.h>
#include <Request.h>
#include <PinState.h>

MFRC522 identificator;
Request request;
PinState pinState;

void setup() {

    Serial.begin(115200);
    SPI.begin();

    ReadFile read = ReadFile("/application.json");
    
    identificator = MFRC522(read.getSS(), read.getRST());
    identificator.PCD_Init();
    
    request = Request(read.getURL(), read.getType(), read.getSSID(), read.getPassword(), read.getSecret(), read.getIssuer());
    pinState = PinState(read.getContentString());
}

void loop() {

    if (!identificator.PICC_IsNewCardPresent() || !identificator.PICC_ReadCardSerial()) {
        return;
    }

    String identifier;

    for (byte i = 0; i < identificator.uid.size; i++) {
        identifier += identificator.uid.uidByte[i], DEC;
    }

    pinState.explain(request.send(request.toJson(identifier.toInt())));

    identificator.PICC_HaltA();

}