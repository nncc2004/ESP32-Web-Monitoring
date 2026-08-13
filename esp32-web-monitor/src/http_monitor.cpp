#include "http_monitor.h"

#include <Arduino.h>
#include <HTTPClient.h>

HTTPResult makeHTTPRequest(const char* url) {

    HTTPResult result;

    result.success = false;
    result.statusCode = 0;
    result.responseTime = 0;
    result.errorMessage = "";

    HTTPClient http;

    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000);

    unsigned long startTime = millis();

    http.begin(url);

    int httpCode = http.GET();

    unsigned long endTime = millis();

    result.responseTime = endTime - startTime;

    if (httpCode > 0) {

        result.success = true;
        result.statusCode = httpCode;
        result.errorMessage = "OK";

    }
    else {

        result.success = false;
        result.statusCode = httpCode;
        result.errorMessage = http.errorToString(httpCode);

    }

    http.end();

    return result;

}