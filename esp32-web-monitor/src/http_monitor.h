#include<Arduino.h>

#ifndef HTTP_MONITOR_H
#define HTTP_MONITOR_H

struct HTTPResult {

    bool success;
    int statusCode;
    unsigned long responseTime;
    String errorMessage;

};

HTTPResult makeHTTPRequest(const char* url);

#endif

/*
EJEMPLO:
HTTPResult result = makeHTTPRequest("https://google.com");
Serial.println(result.success);
Serial.println(result.statusCode);
Serial.println(result.responseTime);
Serial.println(result.errorMessage);
*/