#include <Arduino.h>

#include "wifi_manager.h"
#include "http_monitor.h"
#include "storage_manager.h"
#include <TFT_eSPI.h>

#include <time.h>

TFT_eSPI tft = TFT_eSPI();

bool lastWiFiState = false;
bool consultasIniciadas = false;

unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 15000;

const int CANT_URLS = 3;

String URLS[CANT_URLS][2] = {
    {"https://casivar.cl", "casivar"},
    {"https://aconcaguacinetv.cl", "aconcagua"},
    {"https://barrioplazauruguay.cl", "Barrio PU"}
};

int POS = 0;


String getCurrentDateTime();
void transitionScreen(uint16_t targetColor);

void setup() {

    Serial.begin(115200);

    tft.init();
    tft.setRotation(0);

    tft.setTextSize(3);
    tft.setTextColor(TFT_BLACK);

    // ---------------- SD ----------------

    tft.fillScreen(TFT_YELLOW);

    tft.setCursor(30, 100);
    tft.println("SD");

    if (!initSD()) {

        tft.fillScreen(TFT_RED);

        tft.setCursor(20, 100);
        tft.println("SD ERROR");

        while (true);

    }

    delay(2000);

    // ---------------- WIFI ----------------

    tft.fillScreen(TFT_ORANGE);

    tft.setCursor(20, 100);
    tft.println("WiFi");

    initWiFi();

}

void loop() {

    updateWiFi();

    bool currentWiFiState = isWiFiConnected();

    // Cambio estado WiFi
    if (currentWiFiState != lastWiFiState) {

        lastWiFiState = currentWiFiState;

        tft.fillScreen(currentWiFiState ? TFT_GREEN : TFT_RED);

        if (currentWiFiState) {

            tft.setCursor(25, 100);
            tft.println("Conectado");
            configTime(-3 * 3600, 0, "pool.ntp.org");
            delay(2000);

            tft.fillScreen(TFT_BLUE);

            tft.setCursor(10, 100);
            tft.println("Iniciando \n consultas");

            delay(2000);

            consultasIniciadas = true;

        }
        else {

            tft.fillScreen(TFT_RED);

            tft.setCursor(35, 100);
            tft.println("Sin WiFi");

            consultasIniciadas = false;

        }

    }

    // Requests HTTP
    if (currentWiFiState && consultasIniciadas) {

        unsigned long currentMillis = millis();

        if (currentMillis - lastRequestTime >= requestInterval) {

            lastRequestTime = currentMillis;

            if (POS == CANT_URLS) {
                POS = 0;
            }

            String url = URLS[POS][0];
            String url_name = URLS[POS][1];

            POS++;

            HTTPResult result = makeHTTPRequest(url.c_str());

            // ---------------- SERIAL ----------------

            Serial.println("------");
            Serial.println(url);
            Serial.println(result.statusCode);
            Serial.println(result.responseTime);
            Serial.println(result.errorMessage);

            // ---------------- DISPLAY ----------------

            transitionScreen(
                result.success ? TFT_GREEN : TFT_RED
            );

            tft.setCursor(45, 60);
            tft.println(url_name);

            tft.setCursor(45, 90);
            tft.println(result.success ? "OK" : "ERROR");

            tft.setCursor(45, 115);
            tft.print("Code: ");
            tft.println(result.statusCode);

            tft.setCursor(45, 150);
            tft.print(result.responseTime);

            tft.println(" ms");

            // ---------------- CSV ----------------

            String filename = "/" + url_name + ".csv";

            ensureFileExists(filename.c_str());

            String csvLine =
                getCurrentDateTime() + "," +
                String(result.statusCode) + "," +
                String(result.responseTime) + "," +
                String(result.success) + "," +
                String(result.errorMessage);

            appendLine(filename.c_str(), csvLine.c_str());

        }

    }

}


String getCurrentDateTime() {

    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        return "NO_TIME";
    }

    char buffer[30];

    strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d %H:%M:%S",
        &timeinfo
    );

    return String(buffer);

}


void transitionScreen(uint16_t targetColor) {

    tft.fillScreen(TFT_DARKGREY);
    delay(100);

    tft.fillScreen(TFT_BLACK);
    delay(100);

    tft.fillScreen(targetColor);
    delay(100);

}