/*
  NAME:
  Detect type of an EEPROM chip.

  DESCRIPTION:
  The sketch detects the type of EEPROM and displays all its capacity parameters.
  - Connect chip's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_AT24C_DETECT 1.0.0"

#include "gbj_at24c.h"

// Software configuration
gbj_at24c Eeprom = gbj_at24c();
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_100KHZ, D2, D1);
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_400KHZ);

unsigned char eepromType;

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

  // Initialize EEPROM for the highest supported capacity in order to setup
  // sufficient send delay.
  // Adjust the address to your address configuration.
  if (Eeprom.begin(gbj_at24c::AT24C512))
  {
    errorHandler("Begin dummy");
    return;
  }
  // Detect EEPROM type
  if (Eeprom.detectType(eepromType))
  {
    errorHandler("Detect");
    return;
  }
  // Initialize detected EEPROM.
  // Adjust the address to your address configuration.
  if (Eeprom.begin(eepromType))
  {
    errorHandler("Begin");
    return;
  }
  // Dislay EEPROM parameters
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
