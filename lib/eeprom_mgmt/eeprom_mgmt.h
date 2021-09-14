#ifndef EEPROM_MGMT_H
#define EEPROM_MGMT_H

#include <Arduino.h>

#ifndef EEPROM
#include <EEPROM.h>
#endif

/**
  * @brief Eeprom addresses to store state
  * 
  */
struct eepromAddresses
{
  const int EEPROM_STATE = 1;
  const int LOW_LEVEL = 5;
  const int HIGH_LEVEL = 10;
};

/**
 * @brief create instance of eeprom addresses
 *
 */
constexpr eepromAddresses EEPROM_ADDRESSES;

void initEeprom(bool = false);
void readEepromValues();
void updateEeprom(int, int);

#endif