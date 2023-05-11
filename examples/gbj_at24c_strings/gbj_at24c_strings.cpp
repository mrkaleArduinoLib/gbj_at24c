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
#include "gbj_at24c.h"

const unsigned int POSITION = 0;
const unsigned int POSITION_OFFSET = 4;

// Software configuration
gbj_at24c eeprom = gbj_at24c();
// gbj_at24c eeprom = gbj_at24c(gbj_at24c::CLOCK_400KHZ);
// gbj_at24c eeprom = gbj_at24c(gbj_at24c::CLOCK_100KHZ, D2, D1);

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

  // Write and read text as character array
  char textArray[] = "123456789012345";
  Serial.print("Stored text = ");
  Serial.println(textArray);
  if (eeprom.isError(
        eeprom.storeStream(POSITION,
                           reinterpret_cast<uint8_t *>(textArray),
                           sizeof(textArray) / sizeof(textArray[0]))))
  {
    errorHandler("Store char[]");
    return;
  }
  textArray[0] = '\0';
  if (eeprom.isError(
        eeprom.retrieveStream(POSITION,
                              reinterpret_cast<uint8_t *>(textArray),
                              sizeof(textArray) / sizeof(textArray[0]))))
  {
    errorHandler("Retrieved char[]");
    return;
  }
  Serial.print("Retrieved text = ");
  Serial.println(textArray);
  // Text offset
  textArray[0] = '\0';
  if (eeprom.isError(eeprom.retrieveStream(
        POSITION + POSITION_OFFSET,
        reinterpret_cast<uint8_t *>(textArray),
        sizeof(textArray) / sizeof(textArray[0]) - POSITION_OFFSET)))
  {
    errorHandler("Retrieved partial char[]");
    return;
  }
  Serial.print("Retrieved offset text (" + String(POSITION_OFFSET) + ") = ");
  Serial.println(textArray);
  Serial.println("---");

  // Write and read text as String object (by converting it to the byte stream)
  String textString = "987654321098765";
  byte textLen = textString.length() + 1; // Including trailing '\0'
  Serial.println("Stored string = " + textString);
  char textBuffer[textLen];
  textString.toCharArray(textBuffer, textLen);
  if (eeprom.isError(eeprom.storeStream(
        POSITION, reinterpret_cast<uint8_t *>(textBuffer), textLen)))
  {
    errorHandler("Store String");
    return;
  }
  textString = "";
  textBuffer[0] = '\0';
  if (eeprom.isError(eeprom.retrieveStream(
        POSITION, reinterpret_cast<uint8_t *>(textBuffer), textLen)))
  {
    errorHandler("Retrieved String");
    return;
  }
  textString = String(textBuffer);
  Serial.println("Retrieved string = " + textString);
  Serial.println("---");
}

void loop() {}
