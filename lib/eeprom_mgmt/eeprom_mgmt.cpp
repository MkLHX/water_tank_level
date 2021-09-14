/**
 * @file eeprom_mgmt.cpp
 * @author Mickael Lehoux (mickael@lehoux.net)
 * @brief this is the eeprom storing management module
 * @version 0.1
 * @date 2021-01-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "eeprom_mgmt.h"

/**
 * @brief set all EEPROM addresses to 127 and store the reset status on address 1
 * 
 */
void initEeprom(bool forceInit)
{
#define EEPROM_SIZE 128
    EEPROM.begin(EEPROM_SIZE);
    // check EEPROM status to be sure EEPROM is init
    if (127 != EEPROM.read(EEPROM_ADDRESSES.EEPROM_STATE) || forceInit)
    {
        // write every EEPROM addresses to 127
        for (int i = 0; i <= EEPROM_SIZE; i++)
        {
            EEPROM.write(i, 127);
            EEPROM.commit();
        }
    }
}

/**
 * @brief read all EEPROM values
 * 
 */
void readEepromValues()
{
    // read every EEPROM addresses values
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        Serial.print(F("on EEPROM address: "));
        Serial.print(String(i));
        Serial.print(F(" the value is: "));
        Serial.println(String(EEPROM.read(i)));
    }
}

/**
 * @brief update EEPROM value if is not already the same
 * 
 */
void updateEeprom(int addr, int value)
{
    if (value == EEPROM.read(addr))
    {
        return; // no update because same value
    }
    EEPROM.write(addr, value);
    EEPROM.commit();
}