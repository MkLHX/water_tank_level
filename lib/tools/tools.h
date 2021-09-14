#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>
#if !defined(ARRAY_SIZE)
    #define ARRAY_SIZE(arr) int(sizeof(arr) / sizeof(arr[0]))
#endif

int getPinMode(uint8_t);
void getBoardInfos();

#endif