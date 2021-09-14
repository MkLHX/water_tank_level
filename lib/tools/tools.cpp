/**
 * @file tools.cpp
 * @author Mickael Lehoux (mickael@lehoux.net)
 * @brief this is the tools module
 * @version 0.1
 * @date 2021-01-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "tools.h"

// Adding this line before the library is included will cause a warning in the compile output window containing the identified board
#define BOARD_IDENTIFY_WARNING
// If warnings still don't display, ensure "File->Preferences->compiler warnings" is set to "Default".
// Even doing this, some boards still won't display the warning in the compile window but the constants will still be created.

// Include the library
#include <Board_Identify.h>

/**
 * @brief restart the device
 * 
 */
void (*resetFunc)(void) = 0; //declare reset function at address 0

/**
 * @brief get the current pin mode OUTPUT / INPUT / INPUT_PULLUP
 * 
 * @param pin 
 * @return int 
 */
int getPinMode(uint8_t pin)
{
    if (pin >= NUM_DIGITAL_PINS)
        return (-1);

    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint32_t *reg = portModeRegister(port);
    if (*reg & bit)
        return (OUTPUT);

    volatile uint32_t *out = portOutputRegister(port);
    return ((*out & bit) ? INPUT_PULLUP : INPUT);
}

/**
 * @brief Get the Board Infos object
 * 
 */
void getBoardInfos()
{
    // Print the board information to the serial using the defined terms
    Serial.print(F("Board Type: "));
    Serial.println(BoardIdentify::type);
    Serial.print(F("Board Make: "));
    Serial.println(BoardIdentify::make);
    Serial.print(F("Board Model: "));
    Serial.println(BoardIdentify::model);
    Serial.print(F("Board MCU: "));
    Serial.println(BoardIdentify::mcu);
}
