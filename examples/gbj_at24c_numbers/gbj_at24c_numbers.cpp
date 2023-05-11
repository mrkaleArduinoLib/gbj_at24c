/*
  NAME:
  Storing to and retrieving numbers of various data types from EEPROM.

  DESCRIPTION:
  The sketch demonstrates templated methods of the library by storing values
  to 0 position of the EEPROM.
  - Connect chip's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - The sketch resets the stored value before retrieving in order to check
    correct storing and retrieving.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_at24c.h"

const unsigned int POSITION = 0;

// Software configuration
gbj_at24c eeprom = gbj_at24c();
// gbj_at24c eeprom = gbj_at24c(gbj_at24c::CLOCK_400KHZ);
// gbj_at24c eeprom = gbj_at24c(gbj_at24c::CLOCK_100KHZ, D2, D1);
byte valueByte = 0xA5;
int valueInt = 0xAA55;
float valueFloat = 123.45;

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
  if (eeprom.isError(eeprom.begin(gbj_at24c::AT24C256)))
  {
    errorHandler("Begin");
    return;
  }

  // Write and read byte
  Serial.println("Stored byte: 0x" + String(valueByte, HEX));
  if (eeprom.isError(eeprom.store(POSITION, valueByte)))
  {
    errorHandler("Store byte");
    return;
  }
  valueByte = 0;
  if (eeprom.isError(eeprom.retrieve(POSITION, valueByte)))
  {
    errorHandler("Retrieved byte");
    return;
  }
  Serial.println("Retrieved byte: 0x" + String(valueByte, HEX));
  Serial.println("---");

  // Write and read integer
  Serial.println("Stored integer: 0x" + String(valueInt, HEX));
  if (eeprom.isError(eeprom.store(POSITION, valueInt)))
  {
    errorHandler("Store integer");
    return;
  }
  valueInt = 0;
  if (eeprom.isError(eeprom.retrieve(POSITION, valueInt)))
  {
    errorHandler("Retrieved integer");
    return;
  }
  Serial.println("Retrieved integer: 0x" + String(valueInt, HEX));
  Serial.println("---");

  // Write and read float
  Serial.println("Stored float: " + String(valueFloat));
  if (eeprom.isError(eeprom.store(POSITION, valueFloat)))
  {
    errorHandler("Store float");
    return;
  }
  valueFloat = 0.0;
  if (eeprom.isError(eeprom.retrieve(POSITION, valueFloat)))
  {
    errorHandler("Retrieved float");
    return;
  }
  Serial.println("Retrieved float: " + String(valueFloat));
  Serial.println("---");

  // Read recent position
  valueByte = 0xFF;
  if (eeprom.isError(eeprom.retrieveCurrent(valueByte)))
  {
    errorHandler("Retrieved current");
    return;
  }
  Serial.println("Retrieved current: 0x" + String(valueByte, HEX));
  Serial.println("---");
}

void loop() {}
