#include "storage_manager.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 15

bool initSD() {

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);

    if (!SD.begin(SD_CS)) {

        Serial.println("Error iniciando SD");
        return false;

    }

    Serial.println("SD inicializada");

    return true;

}

bool ensureFileExists(const char* filename) {

    if (SD.exists(filename)) {

        return true;

    }

    File file = SD.open(filename, FILE_WRITE);

    if (!file) {

        Serial.println("No se pudo crear archivo");
        return false;

    }

    file.close();

    Serial.println("Archivo creado");

    return true;

}

bool appendLine(const char* filename, const char* line) {

    File file = SD.open(filename, FILE_APPEND);

    if (!file) {

        Serial.println("Error abriendo archivo");
        return false;

    }

    file.println(line);

    file.close();

    return true;

}

String readFile(const char* filename) {

    File file = SD.open(filename);

    if (!file) {

        Serial.println("Error leyendo archivo");
        return "";

    }

    String content = "";

    while (file.available()) {

        content += (char)file.read();

    }

    file.close();

    return content;

}

bool deleteFile(const char* filename) {

    if (!SD.exists(filename)) {

        return false;

    }

    return SD.remove(filename);

}