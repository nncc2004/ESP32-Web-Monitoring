/*
Este módulo se encarga de:
conectarse
reconectarse
informar estado
entregar IP
*/


#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H



void initWiFi();
void updateWiFi();

bool isWiFiConnected();

#endif