/*
  NAME:
  gbjAT24C

  DESCRIPTION:
  Library for ATMEL EEPROM chips AT24C01 ~ AT24C512 on two-wire (I2C) bus.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_at24c.git
 */
#ifndef GBJ_AT24C_H
#define GBJ_AT24C_H

#include "gbj_memory.h"

class gbj_at24c : public gbj_memory
{
public:
  enum Addresses : uint8_t
  {
    // Minimal address (all address pins grounded)
    ADDRESS_MIN = 0x50,
    // Connection combination of the address pins A0 ~ A2
    ADDRESS_0 = 0 + ADDRESS_MIN,
    ADDRESS_1 = 1 + ADDRESS_MIN,
    ADDRESS_2 = 2 + ADDRESS_MIN,
    ADDRESS_3 = 3 + ADDRESS_MIN,
    ADDRESS_4 = 4 + ADDRESS_MIN,
    ADDRESS_5 = 5 + ADDRESS_MIN,
    ADDRESS_6 = 6 + ADDRESS_MIN,
    ADDRESS_7 = 7 + ADDRESS_MIN,
    // Maximal address (all address pins at Vcc)
    ADDRESS_MAX = ADDRESS_7,
  };
  // Index bit in value, kibibits in name
  enum Capacities : uint8_t
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
    AT24CXXX = 0xFF,
  };

  gbj_at24c(ClockSpeeds clockSpeed = CLOCK_100KHZ,
            uint8_t pinSDA = 4,
            uint8_t pinSCL = 5)
    : gbj_memory(clockSpeed, pinSDA, pinSCL){};

  /*
    Initialize two-wire bus and chip with parameters stored by constructor.

    DESCRIPTION:
    The method sanitizes and stores input parameters to the class instance
    object, which determines the operation modus of the chip.

    PARAMETERS:
    type - EEPROM chip typ determining a capacity bit order. The binary value
    just with capacity bit set determines the memory capacity in kibibits
    (multiples of 1024). The library supports EEPROM chips with capacities from
    1 to 512 Kib. The parameter is constraint to the expected range.
      - Data type: non-negative integer
      - Default value: none
      - Limited range: AT24C01 ~ AT24C512

    address - One of 8 possible 7 bit addresses of the chip or address offset
    determined by the connection of address pins.
      - If address is greater than maximal allowed one, it is defaulted to this
    maximal value.
      - If address is lower than minimal allowed one, it is defaulted to this
    minimal value.
      - Data type: non-negative integer
      - Default value: ADDRESS_MIN
      - Limited range: ADDRESS_MIN ~ ADDRESS_MAX

    RETURN: Result code
  */
  ResultCodes begin(Capacities type,
                    Addresses address = Addresses::ADDRESS_MIN);

  /*
    Detect type of an EEPROM chip.

    DESCRIPTION:
    The method detects the type of the EEPROM chip by detecting its capacity.
    - The method tests type from the highest supported capacity.
    - The test is based on writing specific value to 0 position of the EEPROM
    and another specific value to the first position beyond the capacity of the
    previous supported type. If the tested type is not correct, the EEPROM
    rewrites the value in 0 position with tested value, which is different from
    the reference value written directly to 0 position. The methods decreases
    tested types until the 0 position is not rewritten.
    - The method really rewrites just 0 position of EEPROM and the position in
    the middle of detected type capacity, e.g., for AT24C256 chip it is position
    16384.

    PARAMETERS:
    type - Referenced variable for placing detected EEPROM type.
      - Data type: non-negative integer
      - Default value: none
      - Limited range: 0 ~ 255

    RETURN: Result code
  */
  ResultCodes detectType(Capacities &type);

  // Getters
  inline Capacities getType() { return status_.capacityBit; }

private:
  // Delays of writing to the memory
  enum SendDelays
  {
    SEND_DELAY_AT24C01_16 = 5,
    SEND_DELAY_AT24C32_512 = 10,
  };
  // Memory page sizes
  enum PageSizes
  {
    PAGESIZE_AT24C01_02 = 8,
    PAGESIZE_AT24C04_16 = 16,
    PAGESIZE_AT24C32_64 = 32,
    PAGESIZE_AT24C128_256 = 64,
    PAGESIZE_AT24C512 = 128,
  };
  struct Status
  {
    Capacities capacityBit = Capacities::AT24CXXX; // Memory type
  } status_;
};

#endif
