/*
  NAME:
  Using TMP102 sensor for basic temperature measurement.

  DESCRIPTION:
  The sketch measures temperature with TMP102 sensor.
  - Connect sensor's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - Connect ADD0 pin to ground in order to use default I2C address. For other
    connection determine the appropriate address in the begin() method calling.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_AT24C_DEMO 1.0.0"

#include "gbj_at24c.h"

const unsigned int PERIOD_MEASURE = 3000;  // Time in miliseconds between measurements
const unsigned int POSITION = 0x05;

// Software configuration
gbj_at24c Eeprom = gbj_at24c();
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_100KHZ, true, D2, D1);
// gbj_at24c Eeprom = gbj_at24c(gbj_at24c::CLOCK_400KHZ);
float tempValue;


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

  // Initialize sensor
  if (Eeprom.begin(gbj_at24c::AT24C256)) // Use default address
  {
    errorHandler("Begin");
    return;
  }
  Serial.println("Type = AT24C" + String(Eeprom.getCapacityKiBit()));
  Serial.println("Capacity = " + String(Eeprom.getCapacityKiBit()) + " Kib (" + String(Eeprom.getCapacityBit()) + " b)");
  Serial.println("Capacity = " + String(Eeprom.getCapacityKiByte()) + " KiB (" + String(Eeprom.getCapacityByte()) + " B)");
  Serial.println("---");

  // Read position 0
  uint8_t val;
  // Serial.println("BEFORE");
  // val = Eeprom.retrieve(POSITION);
  // if (Eeprom.isError())
  // {
  //   errorHandler("Read");
  //   return;
  // }
  // Serial.println(val);
  // Serial.println(val, HEX);
  // Serial.println(val, BIN);

  // Write position 0
  if (Eeprom.store(POSITION, 0xFC))
  {
    errorHandler("Write");
    return;
  }

  // Read position 0
  Serial.println("AFTER");
  val = Eeprom.retrieve(POSITION);
  if (Eeprom.isError())
  {
    errorHandler("Read");
    return;
  }
  Serial.println(val);
  Serial.println(val, HEX);
  Serial.println(val, BIN);

}


void loop() {}
