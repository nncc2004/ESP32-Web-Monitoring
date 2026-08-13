#include <Arduino.h>
#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

bool initSD();

bool ensureFileExists(const char* filename);

bool appendLine(const char* filename, const char* line);

String readFile(const char* filename);

bool deleteFile(const char* filename);

#endif