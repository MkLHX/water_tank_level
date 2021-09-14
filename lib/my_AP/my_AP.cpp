/**
 * @file my_AP.cpp
 * @author Mickael Lehoux (mickael@lehoux.net)
 * @brief this is the Access Point module
 * @version 0.1
 * @date 2021-01-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "my_AP.h"


void ap_start()
{
    extern char apSsid[127]; // from main.cpp

    int timeout = 0;
    while (!WiFi.softAP(apSsid, "watertanklevel"))
    {
        Serial.println(F("."));
        delay(250);
        if (timeout > 40) //delay 250ms so 10s = 40*250ms
        {
            Serial.println(F("AP not start"));
        }
        timeout++;
    };
    Serial.println("AP ip : " + WiFi.softAPIP().toString());
}