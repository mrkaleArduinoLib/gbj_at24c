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
  ERROR_POSITION = 255,  // Wrong position in EEPROM; either 0 or no sufficient
                         // space for data storing or retrieving
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
  type - EEPROM chip typ determining a capacity bit order. The binary value just
         with capacity bit set determines the memory capacity in kibibits
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
uint8_t begin(uint8_t type, uint8_t address = 0);


uint8_t storeStream(uint16_t position, uint8_t *dataBuffer, uint16_t dataLen);
uint8_t retrieveStream(uint16_t position, uint8_t *dataBuffer, uint16_t dataLen);
uint8_t fill(uint16_t position, uint16_t dataLen, uint8_t fillValue);
uint8_t erase();


template<class T>
uint8_t store(uint16_t position, T data)
{
  return storeStream(position, (uint8_t *)(void *)&data, sizeof(T));
}


template<class T>
uint8_t retrieve(uint16_t position, T *dataBuffer)
{
  return retrieveStream(position, (uint8_t *)dataBuffer, sizeof(T));
}


//------------------------------------------------------------------------------
// Public setters - they usually return result code.
//------------------------------------------------------------------------------
inline void setPageSize(uint8_t pageSize) { _status.pageSize = pageSize; };


//------------------------------------------------------------------------------
// Public getters
//------------------------------------------------------------------------------
inline uint8_t getType() { return _status.capacityBit; };
inline uint16_t getCapacityKiBit() { return 1 << _status.capacityBit; }; // In Kibits
inline uint32_t getCapacityBit() { return (uint32_t) getCapacityKiBit() << 10; }; // In bits
inline uint8_t getCapacityKiByte() { return getCapacityKiBit() >> 3; };  // In Kibibytes
inline uint16_t getCapacityByte() { return getCapacityKiBit() << 7; };  // In bytes
inline uint8_t getPageSize() { return _status.pageSize; };  // In bytes
inline uint16_t getPages() { return getCapacityByte() / getPageSize(); };


private:
//------------------------------------------------------------------------------
// Private constants
//------------------------------------------------------------------------------
enum Params
{
  // Writing to the memory delays
  SEND_DELAY_AT24C01_16 = 5,
  SEND_DELAY_AT24C32_512 = 10,
  // Memory page sizes
  PAGESIZE_AT24C01_02 = 8,
  PAGESIZE_AT24C04_16 = 16,
  PAGESIZE_AT24C32_64 = 32,
  PAGESIZE_AT24C128_256 = 64,
  PAGESIZE_AT24C512 = 128,
};

//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
struct
{
  uint8_t capacityBit;  // Order number of the capacity bit counting from 0
  uint8_t pageSize;  // Size of the memory page in bytes
} _status;


//------------------------------------------------------------------------------
// Private methods - they return result code if not stated else
//------------------------------------------------------------------------------
uint8_t setAddress(uint8_t address);
uint8_t checkPosition(uint16_t position, uint16_t dataLen);

};

#endif
