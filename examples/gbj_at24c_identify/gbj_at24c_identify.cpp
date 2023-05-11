/*
  NAME:
  Fundamental parameters of EEPROM chip using gbjAT24C library.

  DESCRIPTION:
  The sketch displays all capacity parameters of the EEPROM.
  - Connect chip's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_at24c.h"

// Software configuration
gbj_at24c eeprom = gbj_at24c();
// gbj_at24c eeprom = gbj_at24c(eeprom.CLOCK_400KHZ);
// gbj_at24c eeprom = gbj_at24c(eeprom.CLOCK_100KHZ, D2, D1);

void errorHandler(String location)
{
  Serial.println(eeprom.getLastErrorTxt(location));
  Serial.println("---");
  return;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("---");

  // Initialize EEPROM for default address.
  // Adjust the type and address to your chip and address configuration.
  if (eeprom.isError(eeprom.begin(eeprom.AT24C512)))
  {
    errorHandler("Begin");
    return;
  }
  Serial.println("Clock: " + String(eeprom.getBusClock() / 1000) + " kHz");
  Serial.println("Address: 0x" + String(eeprom.getAddress(), HEX));
  Serial.println("Type: AT24C" + String(eeprom.getCapacityKiBit()));
  Serial.println("Capacity: " + String(eeprom.getCapacityKiBit()) + " Kib (" +
                 String(eeprom.getCapacityBit()) + " b)");
  Serial.println("Capacity: " + String(eeprom.getCapacityKiByte()) + " KiB (" +
                 String(eeprom.getCapacityByte()) + " B)");
  Serial.println("Page: " + String(eeprom.getPageSize()) + " B");
  Serial.println("Pages: " + String(eeprom.getPages()));
  Serial.println("---");
}

void loop() {}
