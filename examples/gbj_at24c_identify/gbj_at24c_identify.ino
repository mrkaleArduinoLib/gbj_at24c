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
#define SKETCH "GBJ_AT24C_IDENTIFY 1.0.0"

#include "gbj_at24c.h"

// Software configuration
gbj_at24c Eeprom = gbj_at24c();
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_100KHZ, true, D2, D1);
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_400KHZ);


void errorHandler(String location)
{
  if (Eeprom.isSuccess()) return;
  Serial.print(location);
  Serial.print(" - Error: ");
  Serial.print(Eeprom.getLastResult());
  Serial.print(" - ");
  switch (Eeprom.getLastResult())
  {
    // General
    case gbj_at24c::ERROR_ADDRESS:
      Serial.println("ERROR_ADDRESS");
      break;

    case gbj_at24c::ERROR_PINS:
      Serial.println("ERROR_PINS");
      break;

    // Arduino, Esspressif specific
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    case gbj_at24c::ERROR_BUFFER:
      Serial.println("ERROR_BUFFER");
      break;

    case gbj_at24c::ERROR_NACK_DATA:
      Serial.println("ERROR_PINS");
      break;

    case gbj_at24c::ERROR_NACK_OTHER:
      Serial.println("ERROR_NACK_OTHER");
      break;

    // Particle specific
#elif defined(PARTICLE)
    case gbj_at24c::ERROR_BUSY:
      Serial.println("ERROR_BUSY");
      break;

    case gbj_at24c::ERROR_END:
      Serial.println("ERROR_END");
      break;

    case gbj_at24c::ERROR_TRANSFER:
      Serial.println("ERROR_TRANSFER");
      break;

    case gbj_at24c::ERROR_TIMEOUT:
      Serial.println("ERROR_TIMEOUT");
      break;
#endif

    // Eeprom specific
    case gbj_at24c::ERROR_POSITION:
      Serial.println("ERROR_POSITION");
      break;

    default:
      Serial.println("Uknown error");
      break;
  }
}


void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_twowire::VERSION);
  Serial.println(gbj_at24c::VERSION);
  Serial.println("---");

  // Initialize EEPROM for default address.
  // Adjust the type and address to your chip and address configuration.
  if (Eeprom.begin(gbj_at24c::AT24C256))
  {
    errorHandler("Begin");
    return;
  }
  Serial.println("Clock: " + String(Eeprom.getBusClock() / 1000) + " kHz");
  Serial.println("Address: 0x" + String(Eeprom.getAddress(), HEX));
  Serial.println("Type: AT24C" + String(Eeprom.getCapacityKiBit()));
  Serial.println("Capacity: " + String(Eeprom.getCapacityKiBit()) + " Kib (" + String(Eeprom.getCapacityBit()) + " b)");
  Serial.println("Capacity: " + String(Eeprom.getCapacityKiByte()) + " KiB (" + String(Eeprom.getCapacityByte()) + " B)");
  Serial.println("Page: " + String(Eeprom.getPageSize()) + " B");
  Serial.println("Pages: " + String(Eeprom.getPages()));
  Serial.println("---");
  Serial.println("END");
}


void loop() {}
