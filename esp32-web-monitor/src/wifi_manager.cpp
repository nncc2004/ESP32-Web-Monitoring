#include "wifi_manager.h"

#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "Depto 501";
const char* password = "*****";

bool wasConnected = false;

unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 5000;

void initWiFi() {

    Serial.println("Iniciando WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

}

void updateWiFi() {

    wl_status_t status = WiFi.status();
    // Conexion exitosa
    if (status == WL_CONNECTED) {

        if (!wasConnected) {

            wasConnected = true;

            Serial.println("WiFi conectado");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());

        }

    }

    // Conexion perdida
    else {

        if (wasConnected) {

            wasConnected = false;

            Serial.println("WiFi desconectado");

        }

        unsigned long currentMillis = millis();

        if (currentMillis - lastReconnectAttempt >= reconnectInterval) {

            lastReconnectAttempt = currentMillis;

            Serial.println("Reintentando conexion WiFi...");

            WiFi.disconnect();
            WiFi.begin(ssid, password);

        }

    }

}

bool isWiFiConnected() {

    return WiFi.status() == WL_CONNECTED;

}