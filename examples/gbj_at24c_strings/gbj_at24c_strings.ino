/*
  NAME:
  Storing to and retrieving strings from EEPROM.

  DESCRIPTION:
  The sketch demonstrates technics for storing and retrieving strings in form
  of character array or and String object.
  - Connect chip's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - The sketch resets the string value before retrieving in order to check
    correct storing and retrieving.
  - Storing and retrieving String object is based on converting it to character
    array before at storing or vice versa at retrieving.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_AT24C_STRINGS 1.0.0"

#include "gbj_at24c.h"
// #include "gbj_at24c.cpp"

const unsigned int POSITION = 0;
const unsigned int POSITION_OFFSET = 4;

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

  // Write and read text as character array
  char textArray[] = "123456789012345";
  Serial.print("Stored text = ");
  Serial.println(textArray);
  if (Eeprom.storeStream(POSITION, (uint8_t *)textArray, sizeof(textArray) / sizeof(textArray[0])))
  {
    errorHandler("Store char[]");
    return;
  }
  textArray[0] = '\0';
  if (Eeprom.retrieveStream(POSITION, (uint8_t *)textArray, sizeof(textArray) / sizeof(textArray[0])))
  {
    errorHandler("Retrieved char[]");
    return;
  }
  Serial.print("Retrieved text = ");
  Serial.println(textArray);
  // Text offset
  textArray[0] = '\0';
  if (Eeprom.retrieveStream(POSITION + POSITION_OFFSET, (uint8_t *)textArray, \
    sizeof(textArray) / sizeof(textArray[0]) - POSITION_OFFSET))
  {
    errorHandler("Retrieved partial char[]");
    return;
  }
  Serial.print("Retrieved offset text (" + String(POSITION_OFFSET) + ") = ");
  Serial.println(textArray);
  Serial.println("---");

  // Write and read text as String object (by converting it to the byte stream)
  String textString = "987654321098765";
  byte textLen = textString.length();
  Serial.println("Stored string = " + textString);
  char textBuffer[textLen];
  textString.toCharArray(textBuffer, textString.length());
  if (Eeprom.storeStream(POSITION, (uint8_t *)textBuffer, textLen))
  {
    errorHandler("Store String");
    return;
  }
  textString = "";
  textBuffer[0] = '\0';
  if (Eeprom.retrieveStream(POSITION, (uint8_t *)textBuffer, textLen))
  {
    errorHandler("Retrieved String");
    return;
  }
  textString = String(textBuffer);
  Serial.println("Retrieved string = " + textString);
  Serial.println("---");
  Serial.println("END");
}


void loop() {}
