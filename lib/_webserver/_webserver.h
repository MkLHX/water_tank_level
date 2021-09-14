#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <ArduinoJson.h>

void webserver_start();
void webserver_update();

#endif