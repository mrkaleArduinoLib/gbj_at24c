/*
  NAME:
  gbjAT24C

  DESCRIPTION:
  Library for ATMEL EEPROM chips AT24C01 ~ AT24C512 on two-wire (I2C) bus.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_at24c.git
 */
#ifndef GBJ_AT24C_H
#define GBJ_AT24C_H

#include "gbj_twowire.h"


class gbj_at24c : public gbj_twowire
{
public:
//------------------------------------------------------------------------------
// Public constants
//------------------------------------------------------------------------------
static const String VERSION;
enum Addresses
{
  ADDRESS_MIN = 0x50,  // Minimal address (all address pins grounded)
  ADDRESS_MAX = 0x57,  // Maximal address (all address pins at Vcc)
};
enum Capacities  // Index bit in value, kilobits in name
{
  AT24C01 = 0,
  AT24C02 = 1,
  AT24C04 = 2,
  AT24C08 = 3,
  AT24C16 = 4,
  AT24C32 = 5,
  AT24C64 = 6,
  AT24C128 = 7,
  AT24C256 = 8,
  AT24C512 = 9,
};
enum ErrorCodes
{
  ERROR_POSITION = 255,  // Wrong position in EEPROM
};


//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------
/*
  Constructor taken from parent class.
*/
gbj_at24c(uint32_t clockSpeed = CLOCK_100KHZ, bool busStop = true, \
  uint8_t pinSDA = 4, uint8_t pinSCL = 5) \
: gbj_twowire(clockSpeed, busStop, pinSDA, pinSCL) {};


/*
  Initialize two-wire bus and chip with parameters stored by constructor.

  DESCRIPTION:
  The method sanitizes and stores input parameters to the class instance object,
  which determines the operation modus of the chip.

  PARAMETERS:
  capacity - Capacity bit order of the chip type. The binary value just with
             capacity bit set determines the memory capacity in kibibits
             (multiples of 1024). The library supports EEPROM chips with
             capacities from 1 to 512 Kib. The parameter is constraint to the
             expected range.
            - Data type: non-negative integer
            - Default value: none
            - Limited range: ADDRESS_MIN ~ AT24C512 or 0 ~ 9

  address - One of 8 possible 7 bit addresses of the chip or address offset
            determined by the connection of address pins.
            - If address is greater than maximal allowed one, it is defaulted
              to this maximal value.
            - If address is lower than minimal allowed one, it is defaulted
              to this minimal value.
            - Data type: non-negative integer
            - Default value: 0
            - Limited range: AT24C01 ~ ADDRESS_MAX or 0 ~ 7

  RETURN:
  Result code.
*/
uint8_t begin(uint8_t capacity, uint8_t address = 0);

template<class T>
T retrieve(T *position);

template<class T>
uint8_t store(T *position, T data);


//------------------------------------------------------------------------------
// Public setters - they usually return result code.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public getters
//------------------------------------------------------------------------------
inline uint32_t getCapacityBits() { return 1 << (_status.configRegister + 10); };
inline uint32_t getCapacity() { return 1 << (_status.configRegister + 7); };  // In bytes


private:
//------------------------------------------------------------------------------
// Private constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
struct
{
  uint8_t capacityBit;  // Order number of the capacity bit counting from 0
} _status;


//------------------------------------------------------------------------------
// Private methods - they return result code if not stated else
//------------------------------------------------------------------------------
uint8_t setAddress(uint8_t address);

};

#endif
