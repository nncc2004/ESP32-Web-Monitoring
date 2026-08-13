#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <TFT_eSPI.h>

#define SD_CS 15

TFT_eSPI tft = TFT_eSPI();

unsigned long lastCheck = 0;
const unsigned long interval = 3000;

void drawStatus(bool ok, uint64_t sizeMB = 0) {
    if (ok) {
        tft.fillScreen(TFT_GREEN);

        tft.setTextColor(TFT_BLACK);
        tft.setTextSize(2);

        tft.setCursor(40, 90);
        tft.println("SD OK");

        tft.setCursor(20, 130);
        tft.print(sizeMB);
        tft.println(" MB");
    }
    else {
        tft.fillScreen(TFT_RED);

        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);

        tft.setCursor(35, 110);
        tft.println("SD ERROR");
    }
}

bool checkSD(uint64_t &sizeMB) {

    digitalWrite(5, HIGH);

    SD.end();

    if (!SD.begin(15)) {
        return false;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        return false;
    }

    sizeMB = SD.cardSize() / (1024 * 1024);

    return true;
}

void setup() {
    Serial.begin(115200);

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);

    tft.init();
    tft.setRotation(0);

    tft.fillScreen(TFT_BLACK);
}

void loop() {
    if (millis() - lastCheck >= interval) {
        lastCheck = millis();

        uint64_t sizeMB = 0;

        bool ok = checkSD(sizeMB);

        Serial.println("Verificando SD...");

        if (ok) {
            Serial.print("SD OK - ");
            Serial.print(sizeMB);
            Serial.println(" MB");
        }
        else {
            Serial.println("SD ERROR");
        }

        drawStatus(ok, sizeMB);
    }
}
