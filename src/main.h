#ifndef MAIN_H
#define MAIN_H

/**
 * @file main.h
 * @author Mickael Lehoux (mickael@lehoux.net)
 * @brief this is the Water Tank Level firmware
 * @version 0.1
 * @date 2021-09-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <Arduino.h>
#include <ArduinoJson.h>
#ifndef EEPROM
#include <EEPROM.h>
#endif
#ifndef _webserver
#include "_webserver.h"
#endif
#ifndef tools
#include "tools.h"
#endif
#ifndef eeprom_mgmt
#include "eeprom_mgmt.h"
#endif
#ifndef my_AP
#include "my_AP.h"
#endif
#ifndef _ota
#include "_ota.h"
#endif
#ifndef my_mDNS
#include "my_mDNS.h"
#endif

#endif // MAIN_H