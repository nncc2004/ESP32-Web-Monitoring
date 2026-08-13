#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 15

void setup() {
    Serial.begin(115200);

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);

    Serial.println("Probando SD...");

    if (!SD.begin(15)) {
        Serial.println("SD.begin fallo");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No hay tarjeta SD");
        return;
    }

    Serial.println("Tarjeta detectada");

    Serial.print("Tipo: ");

    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("Desconocido");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);

    Serial.print("Tamano: ");
    Serial.print(cardSize);
    Serial.println(" MB");
}

void loop() {
}