/**
 * @file my_mDNS.cpp
 * @author Mickael Lehoux (mickael@lehoux.net)
 * @brief this is the mdns/bonjour module
 * @version 0.1
 * @date 2021-01-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "my_mDNS.h"

void mdns_start()
{
    extern char hostname[127]; // from main.cpp

    if (!MDNS.begin(hostname))
    {
        Serial.println(F("mDNS error setting up MDNS responder!"));
    }
}