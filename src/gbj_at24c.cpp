#include "gbj_at24c.h"

gbj_at24c::ResultCodes gbj_at24c::begin(Capacities type, Addresses address)
{
  _status.capacityBit = type;
  // Send delay
  switch (_status.capacityBit)
  {
    case Capacities::AT24C01:
    case Capacities::AT24C02:
    case Capacities::AT24C04:
    case Capacities::AT24C08:
    case Capacities::AT24C16:
      setDelaySend(SendDelays::SEND_DELAY_AT24C01_16);
      break;
    case Capacities::AT24C32:
    case Capacities::AT24C64:
    case Capacities::AT24C128:
    case Capacities::AT24C256:
    case Capacities::AT24C512:
      setDelaySend(SendDelays::SEND_DELAY_AT24C32_512);
      break;
    default:
      break;
  }
  // Page size
  uint8_t pageSize;
  switch (_status.capacityBit)
  {
    case Capacities::AT24C01:
    case Capacities::AT24C02:
      pageSize = PAGESIZE_AT24C01_02;
      break;
    case Capacities::AT24C04:
    case Capacities::AT24C08:
    case Capacities::AT24C16:
      pageSize = PageSizes::PAGESIZE_AT24C04_16;
      break;
    case Capacities::AT24C32:
    case Capacities::AT24C64:
      pageSize = PageSizes::PAGESIZE_AT24C32_64;
      break;
    case Capacities::AT24C128:
    case Capacities::AT24C256:
      pageSize = PageSizes::PAGESIZE_AT24C128_256;
      break;
    case Capacities::AT24C512:
      pageSize = PageSizes::PAGESIZE_AT24C512;
      break;
    default:
      break;
  }
  if (isError(
        gbj_memory::begin((1 << (_status.capacityBit + 7)) - 1, pageSize)))
  {
    return getLastResult();
  }
  return setAddress(static_cast<uint8_t>(address));
}

gbj_at24c::ResultCodes gbj_at24c::detectType(Capacities &type)
{
  const uint16_t refPosition = 0;
  const uint8_t refValue = 0x55;
  const uint8_t testValue = 0xAA;
  type = Capacities::AT24CXXX;
  for (uint8_t capacityBit = Capacities::AT24C512; capacityBit; capacityBit--)
  {
    // Write reference value to the very first position
    if (store(refPosition, refValue))
    {
      return getLastResult();
    }
    // Write test value to the very last position of testing capacity.
    // Do not use standard method in order to avoid capacity check. Real
    // position is the same to logical one, so that no mapping is required.
    uint16_t testPosition = 1 << (capacityBit + 6);
    if (busSendStreamPrefixed(const_cast<uint8_t *>(&testValue),
                              sizeof(testValue),
                              false,
                              reinterpret_cast<uint8_t *>(&testPosition),
                              sizeof(testPosition),
                              true,
                              true))
    {
      return getLastResult();
    }
    // Read reference position and compare it to the reference value.
    // At wrong capacity the storing rollovers.
    uint8_t readValue;
    if (retrieve(refPosition, readValue))
    {
      return getLastResult();
    }
    if (readValue == refValue)
    {
      type = static_cast<Capacities>(capacityBit);
      return getLastResult();
    }
  }
  return setLastResult(ResultCodes::ERROR_POSITION);
}
