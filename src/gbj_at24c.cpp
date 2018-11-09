#include "gbj_at24c.h"
const String gbj_at24c::VERSION = "GBJ_AT24C 1.0.0";


uint8_t gbj_at24c::begin(uint8_t type, uint8_t address)
{
  _status.capacityBit = constrain(type, AT24C01, AT24C512);
  if (setAddress(address)) return getLastResult();
  setDelaySend(SEND_DELAY);
  //
  return getLastResult();
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
  address = max(address, ADDRESS_MIN);
  address = min(address, ADDRESS_MAX);
  return gbj_twowire::setAddress(address);
}


//------------------------------------------------------------------------------
// Getters
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------
