#include "gbj_at24c.h"
const String gbj_at24c::VERSION = "GBJ_AT24C 1.0.0";


uint8_t gbj_at24c::begin(uint8_t capacity, uint8_t address)
{
  _status.capacityBit = constraint(capacity, AT24C01, AT24C512);
  if (setAddress(address)) return getLastResult();
  return getLastResult();
}


template<class T>
uint8_t store(T *position, T data)
{
  uint8_t bytes = sizeof(T);
  uint8_t *ptr = (uint8_t *)(void *)&data;  // Pointer to internal copy of parameter
  while (bytes--)
  {
    eeprom_write_byte (((uint8_t *)(position) + bytes), *(ptr + bytes));
  }
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
uint8_t gbj_at24c::busSendBuffer(uint8_t* buffer, uint32_t bufferBytes)
{
  uint8_t packetSize = 32;
  initLastResult();
  while (bufferBytes)
  {
    beginTransmission(getAddress());
    busWrite(CMD_CONTROL_BYTE_DATA_CONTINUE);
    uint8_t packetBytes = packetSize - sizeof(CMD_CONTROL_BYTE_DATA_CONTINUE);
    while (packetBytes > 0 && bufferBytes > 0)
    {
      busWrite(*buffer++);
      // busWrite(0);
      packetBytes--;
      bufferBytes--;
    }
    if (setLastResult(endTransmission(getBusStop()))) return getLastResult();
  }
  return getLastResult();
}
