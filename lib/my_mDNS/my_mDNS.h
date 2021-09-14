#ifndef MY_MDNS_H
#define MY_MDNS_H

#include <Arduino.h>
#include <ESPmDNS.h>
#ifndef tools
#include "tools.h"
#endif

void mdns_start();

#endif