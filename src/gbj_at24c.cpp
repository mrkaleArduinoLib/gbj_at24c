#include "gbj_at24c.h"
const String gbj_at24c::VERSION = "GBJ_AT24C 1.0.0";


uint8_t gbj_at24c::begin(uint8_t type, uint8_t address)
{
  _status.capacityBit = constrain(type, AT24C01, AT24C512);
  // Send delay
  switch (_status.capacityBit)
  {
    case AT24C01:
    case AT24C02:
    case AT24C04:
    case AT24C08:
    case AT24C16:
      setDelaySend(SEND_DELAY_AT24C01_16);
      break;
    case AT24C32:
    case AT24C64:
    case AT24C128:
    case AT24C256:
    case AT24C512:
      setDelaySend(SEND_DELAY_AT24C32_512);
      break;
  }
  // Page size
  uint8_t pageSize;
  switch (_status.capacityBit)
  {
    case AT24C01:
    case AT24C02:
      pageSize = PAGESIZE_AT24C01_02;
      break;
    case AT24C04:
    case AT24C08:
    case AT24C16:
      pageSize = PAGESIZE_AT24C04_16;
      break;
    case AT24C32:
    case AT24C64:
      pageSize = PAGESIZE_AT24C32_64;
      break;
    case AT24C128:
    case AT24C256:
      pageSize = PAGESIZE_AT24C128_256;
      break;
    case AT24C512:
      pageSize = PAGESIZE_AT24C512;
      break;
  }
  if (gbj_memory::begin((1 << (_status.capacityBit + 7)) - 1, pageSize)) \
    return getLastResult();
  if (setAddress(address)) return getLastResult();
  return getLastResult();
}


uint8_t gbj_at24c::detectType(uint8_t &type)
{
  const uint16_t refPosition = 0;
  const uint8_t refValue = 0x55;
  const uint8_t testValue = 0xAA;
  const uint8_t badType = 0xFF;
  type = badType;
  for (uint8_t capacityBit = AT24C512; capacityBit; capacityBit--)
  {
    // Write reference value to the very first position
    if (store(refPosition, refValue)) return getLastResult();
    // Write test value to the very last position of testing capacity.
    // Do not use standard method in order to avoid capacity check. Real position
    // is the same to logical one, so that no mapping is required.
    uint16_t testPosition = 1 << (capacityBit + 6);
    if (busSendStreamPrefixed((uint8_t *)(void *)&testValue, sizeof(testValue), false, \
      (uint8_t *)&testPosition, sizeof(testPosition), true, true)) return getLastResult();
    // Read reference position and compare it to the reference value.
    // At wrong capacity the storing rollovers.
    uint8_t readValue;
    if (retrieve(refPosition, readValue)) return getLastResult();
    if (readValue == refValue)
    {
      type = capacityBit;
      return getLastResult();
    }
  }
  return setLastResult(ERROR_POSITION);
}


//-------------------------------------------------------------------------
// Setters
//-------------------------------------------------------------------------
uint8_t gbj_at24c::setAddress(uint8_t address)
{
  // Detect address form and finalize address
  if (address < ADDRESS_MIN)
  {
    address &= 0x07;
    address |= ADDRESS_MIN;
  }
  // Sanitize address
  address = constrain(address, ADDRESS_MIN, ADDRESS_MAX);
  return gbj_twowire::setAddress(address);
}
