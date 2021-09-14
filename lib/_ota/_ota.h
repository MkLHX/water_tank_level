#ifndef _OTA_H
#define _OTA_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#ifndef tools
#include "tools.h"
#endif

void _ota_start();
void _ota_handle();

#endif