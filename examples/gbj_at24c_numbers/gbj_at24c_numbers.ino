/*
  NAME:
  Storing to and retrieving from EEPROM numbers of various data types.

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
#define SKETCH "GBJ_AT24C_NUMBERS 1.0.0"

#include "gbj_at24c.h"
// #include "gbj_at24c.cpp"

const unsigned int POSITION = 0;

// Software configuration
gbj_at24c Eeprom = gbj_at24c();
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_100KHZ, true, D2, D1);
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_400KHZ);
byte valueByte = 0xAA;
int valueInt = 0x5555;
float valueFloat = 123.45;


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

  // Write and read byte
  Serial.println("Stored byte = 0x" + String(valueByte, HEX));
  if (Eeprom.store(POSITION, valueByte))
  {
    errorHandler("Store byte");
    return;
  }
  valueByte = 0;
  if (Eeprom.retrieve(POSITION, valueByte))
  {
    errorHandler("Retrieved byte");
    return;
  }
  Serial.println("Retrieved byte = 0x" + String(valueByte, HEX));
  Serial.println("---");

  // Write and read integer
  Serial.println("Stored integer = 0x" + String(valueInt, HEX));
  if (Eeprom.store(POSITION, valueInt))
  {
    errorHandler("Store integer");
    return;
  }
  valueInt = 0;
  if (Eeprom.retrieve(POSITION, valueInt))
  {
    errorHandler("Retrieved integer");
    return;
  }
  Serial.println("Retrieved integer = 0x" + String(valueInt, HEX));
  Serial.println("---");

  // Write and read float
  Serial.println("Stored float = " + String(valueFloat));
  if (Eeprom.store(POSITION, valueFloat))
  {
    errorHandler("Store float");
    return;
  }
  valueFloat = 0.0;
  if (Eeprom.retrieve(POSITION, valueFloat))
  {
    errorHandler("Retrieved float");
    return;
  }
  Serial.println("Retrieved float = " + String(valueFloat));
  Serial.println("---");
  Serial.println("END");
}


void loop() {}
