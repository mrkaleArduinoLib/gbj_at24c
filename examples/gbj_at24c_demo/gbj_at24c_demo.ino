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
#define SKETCH "GBJ_TMP102_MEASURE 1.0.0"

#include "gbj_tmp102.h"

const unsigned int PERIOD_MEASURE = 3000;  // Time in miliseconds between measurements

// Software configuration
gbj_tmp102 Sensor = gbj_tmp102();
// gbj_tmp102 Sensor = gbj_tmp102(gbj_tmp102::CLOCK_100KHZ, true, D2, D1);
// gbj_tmp102 Sensor = gbj_tmp102(gbj_tmp102::CLOCK_400KHZ);
float tempValue;


void errorHandler(String location)
{
  if (Sensor.isSuccess()) return;
  Serial.print(location);
  Serial.print(" - Error: ");
  Serial.print(Sensor.getLastResult());
  Serial.print(" - ");
  switch (Sensor.getLastResult())
  {
    // General
    case gbj_tmp102::ERROR_ADDRESS:
      Serial.println("ERROR_ADDRESS");
      break;

    case gbj_tmp102::ERROR_PINS:
      Serial.println("ERROR_PINS");
      break;

    // Arduino, Esspressif specific
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    case gbj_tmp102::ERROR_BUFFER:
      Serial.println("ERROR_BUFFER");
      break;

    case gbj_tmp102::ERROR_NACK_DATA:
      Serial.println("ERROR_PINS");
      break;

    case gbj_tmp102::ERROR_NACK_OTHER:
      Serial.println("ERROR_NACK_OTHER");
      break;

    // Particle specific
#elif defined(PARTICLE)
    case gbj_tmp102::ERROR_BUSY:
      Serial.println("ERROR_BUSY");
      break;

    case gbj_tmp102::ERROR_END:
      Serial.println("ERROR_END");
      break;

    case gbj_tmp102::ERROR_TRANSFER:
      Serial.println("ERROR_TRANSFER");
      break;

    case gbj_tmp102::ERROR_TIMEOUT:
      Serial.println("ERROR_TIMEOUT");
      break;
#endif

    // Sensor specific
    case gbj_tmp102::ERROR_MEASURE_TEMP:
      Serial.println("ERROR_MEASURE_TEMP");
      break;

    case gbj_tmp102::ERROR_SETUP_TEMP:
      Serial.println("ERROR_SETUP_TEMP");
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
  Serial.println(gbj_tmp102::VERSION);
  Serial.println("---");

  // Initialize sensor
  if (Sensor.begin()) // Use default address
  {
    errorHandler("Begin");
    return;
  }

  // Print header
  Serial.println("Temperature ('C')");
}


void loop()
{
  if (Sensor.isError()) return;

  // Measure
  tempValue = Sensor.measureTemperature();
  // tempValue = Sensor.measureTemperatureOneshot();
  if (Sensor.isSuccess())
  {
    Serial.println(tempValue, 4);
  }
  else
  {
    errorHandler("Temperature");
  }
  delay(PERIOD_MEASURE);
}
