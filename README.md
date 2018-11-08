<a id="library"></a>
# gbjAT24C
Library for Atmel *AT24Cxxx* EEPROM chips communicating on two-wire (I2C) bus.
- Sensor can have following addresses, which allows up to 4 sensors present on the same two-wire bus:
  - `0x48` for ADD0 pin connected to GND (ground)
  - `0x49` for ADD0 pin connected to V+ (power supply positive rail)
  - `0x4A` for ADD0 pin connected to SDA (serial data rail of the two-wire bus)
  - `0x4B` for ADD0 pin connected to SCL (serial clock rail of the two-wire bus)
- The library provides measured temperature in degrees of Celsius.
- For conversion among various temperature unit scales and for calculating dew point temperature use library *gbjAppHelpers*.
- The sensor in normal mode has 12 bit resolution with sensitivity 0.0625 centigrade with measurement range -55 to +128 centigrade.
- The extended mode has 13 bit resolution, but the same sensitivity and it just extends the upper temperature measurement range up to +150 centigrade.
  - Switching (reconfiguration) to extended mode from normal mode or vice-versa needs a time delay cca 350 milliseconds after it in order to settle the sensor and stabilize the temperature conversion.
  - Without the delay after switching to extended mode the reading is doubled to real temperature at first reading after switching mode.
  - Without the delay after switching to normal mode the reading is halved to real temperature at first reading after switching mode.
  - Library does not have implemented such specific delay after mode switching due to small usefulness of the extended mode.


#### Particle hardware configuration
- Connect microcontroller's pin `D0` to sensor's pin **SDA** (Serial Data).
- Connect microcontroller's pin `D1` to sensor's pin **SCL** (Serial Clock).

#### Arduino UNO hardware configuration
- Connect microcontroller's pin `A4` to sensor's pin **SDA** (Serial Data).
- Connect microcontroller's pin `A5` to sensor's pin **SCL** (Serial Clock).

#### Espressif - ESP8266, ESP32 default hardware configuration
- Connect microcontroller's pin `D2` to sensor's pin **SDA** (Serial Data).
- Connect microcontroller's pin `D1` to sensor's pin **SCL** (Serial Clock).


<a id="dependency"></a>
## Dependency

#### Particle platform
- **Particle.h**: Includes alternative (C++) data type definitions.

#### Arduino platform
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.
- **TwoWire**: I2C system library loaded from the file *Wire.h*.

#### Custom Libraries
- **gbjTwoWire**: I2C custom library loaded from the file *gbj_twowire.h*. The library [gbjTMP102](#library) inherits common bus functionality from this library.


<a id="constants"></a>
## Constants
- **gbj\_tmp102::VERSION**: Name and semantic version of the library.


<a id="addresses"></a>
#### Addresses
- **gbj\_tmp102::ADDRESS\_GND**: ADD0 pin connected to GND pin (default).
- **gbj\_tmp102::ADDRESS\_VCC**: ADD0 pin connected to positive power supply rail.
- **gbj\_tmp102::ADDRESS\_SDA**: ADD0 pin connected to serial data rail of two-wire bus.
- **gbj\_tmp102::ADDRESS\_SCL**: ADD0 pin connected to serial clock rail of two-wire bus.


<a id="conversions"></a>
#### Conversion rates
- **gbj\_tmp102::CONVERSION\_PERIOD\_4000MS**: Conversion period in milliseconds for conversion frequency 0.25 Hz.
- **gbj\_tmp102::CONVERSION\_PERIOD\_1000MS**: Conversion period in milliseconds for conversion frequency 1 Hz.
- **gbj\_tmp102::CONVERSION\_PERIOD\_250MS**: Conversion period in milliseconds for conversion frequency 4 Hz (default).
- **gbj\_tmp102::CONVERSION\_PERIOD\_125MS**: Conversion period in milliseconds for conversion frequency 8 Hz.
- **gbj\_tmp102::CONVERSION\_RATE\_025HZ**: Conversion frequency 0.25 Hz.
- **gbj\_tmp102::CONVERSION\_RATE\_1HZ**: Conversion frequency 1 Hz.
- **gbj\_tmp102::CONVERSION\_RATE\_4HZ**: Conversion frequency 4 Hz (default).
- **gbj\_tmp102::CONVERSION\_RATE\_8HZ**: Conversion frequency 8 Hz.


<a id="faults"></a>
#### Fault queues
- **gbj\_tmp102::FAULT\_QUEUE\_1**: 1 consecutive fault for changing ALERT pin state (default).
- **gbj\_tmp102::FAULT\_QUEUE\_2**: 2 consecutive faults for changing ALERT pin state.
- **gbj\_tmp102::FAULT\_QUEUE\_4**: 4 consecutive faults for changing ALERT pin state.
- **gbj\_tmp102::FAULT\_QUEUE\_6**: 8 consecutive faults for changing ALERT pin state.


<a id="errors"></a>
#### Error codes
- **gbj\_tmp102::ERROR\_RESET**: Resetting failure.
- **gbj\_tmp102::ERROR\_MEASURE\_TEMP**: Measuring temperature failure.
- **gbj\_tmp102::ERROR\_SETUP\_TEMP**: Temperature limits failure.

Other error codes as well as result code are inherited from the parent library [gbjTwoWire](#dependency).


<a id="configuration"></a>
## Configuration
The configuration of the sensor is realized by the configuration register, which consist of several configuration bits determining its behaviour. The library stores (caches) the value of the configuration register in its instance object.

The sensor configuration implemented in the library is based on updating cached configuration value in advanced and finally to send that value to the sensor and write all configuration bits to configuration register at once in order to reduce communication on the two-wire bus in contrast to sending configuration bits to the sensor individually.


<a id="interface"></a>
## Interface

#### Main
- [gbj_tmp102()](#gbj_tmp102)
- [begin()](#begin)
- [reset()](#reset)
- [measureTemperature()](#measureTemperature)
- [measureTemperatureOneshot()](#measureTemperatureOneshot)
- [calculateTemperature()](#calculateTemperature)

#### Setters
- [setConfiguration()](#setConfiguration)
- [setAlertLow()](#setAlertValue)
- [setAlertHigh()](#setAlertValue)
- [setAlerts()](#setAlertValue)
- [configAlertActiveLow()](#configAlertMode)
- [configAlertActiveHigh()](#configAlertMode)
- [configExtendedMode()](#configResolutionMode)
- [configNormalMode()](#configResolutionMode)
- [configShutdownMode()](#configPowerMode)
- [configContinuousMode()](#configPowerMode)
- [configInterruptMode()](#configActionMode)
- [configThermostatMode()](#configActionMode)
- [configConversionRate()](#configConversionRate)
- [configFaultQueue()](#configFaultQueue)
- [configOneshotMode()](#configOneshotMode)

#### Getters
- [getConfiguration()](#getConfiguration)
- [getAlertActiveLow()](#getAlertMode)
- [getAlertActiveHigh()](#getAlertMode)
- [getAlertLow()](#getAlertValue)
- [getAlertHigh()](#getAlertValue)
- [getAlert()](#getAlert)
- [getExtendedMode()](#getResolutionMode)
- [getNormalMode()](#getResolutionMode)
- [getShutdownMode()](#getPowerMode)
- [getContinuousMode()](#getPowerMode)
- [getInterruptMode()](#getActionMode)
- [getThermostatMode()](#getActionMode)
- [getConversionRate()](#getConversionRate)
- [getFaultQueue()](#getFaultQueue)
- [getOneshotMode()](#getOneshotMode)

Other possible setters and getters are inherited from the parent library [gbjTwoWire](#dependency) and described there.


<a id="gbj_tmp102"></a>
## gbj_tmp102()
#### Description
The library does not need special constructor and destructor, so that the inherited ones are good enough and there is no need to define them in the library, just use it with default or specific parameters as defined at constructor of parent library [gbjTwoWire](#dependency).
- Constructor sets parameters specific to the two-wire bus in general.
- All the constructor parameters can be changed dynamically with corresponding setters later in a sketch.

#### Syntax
    gbj_tmp102(uint32_t clockSpeed, bool busStop, uint8_t pinSDA, uint8_t pinSCL);

#### Parameters
<a id="prm_busClock"></a>
- **clockSpeed**: Two-wire bus clock frequency in Hertz. If the clock is not from enumeration, it fallbacks to 100 kHz.
  - *Valid values*: gbj\_tmp102::CLOCK\_100KHZ, gbj\_tmp102::CLOCK\_400KHZ
  - *Default value*: gbj\_tmp102::CLOCK\_100KHZ


<a id="prm_busStop"></a>
- **busStop**: Logical flag about releasing bus after end of transmission.
  - *Valid values*: true, false
    - **true**: Releases the bus after data transmission and enables other master devices to control the bus.
    - **false**: Keeps connection to the bus and enables to begin further data transmission immediately.
  - *Default value*: true


<a id="prm_pinSDA"></a>
- **pinSDA**: Microcontroller's pin for serial data. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms for communication on the bus. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  - *Valid values*: positive integer
  - *Default value*: 4 (GPIO4, D2)


<a id="prm_pinSCL"></a>
- **pinSCL**: Microcontroller's pin for serial clock. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  - *Valid values*: positive integer
  - *Default value*: 5 (GPIO5, D1)

#### Returns
Object performing the sensor management.
The constructor cannot return [a result or error code](#constants) directly, however, it stores them in the instance object. The result can be tested in the operational code with the method [getLastResult()](#getLastResult), [isError()](#isError), or [isSuccess()](#isSuccess).

#### Example
The method has all arguments defaulted and calling without any parameters is equivalent to the calling with all arguments set by corresponding constant with default value:

```cpp
  gbj_tmp102 Sensor = gbj_tmp102(); // It is equivalent to
  gbj_tmp102 Sensor = gbj_tmp102(gbj_tmp102::CLOCK_100KHZ, true, D2, D1);
```

[Back to interface](#interface)


<a id="begin"></a>
## begin()
#### Description
The method takes, sanitizes, and stores sensor parameters to a class instance object and initiates two-wire bus.
- The method sets parameters specific to the sensor itself.
- The method resets the sensor to its power-up state (see details in method [reset](#reset)).
- All the method parameters can be changed dynamically with corresponding [setters](#interface) later in a sketch.

#### Syntax
    uint8_t begin(uint8_t address);

#### Parameters
<a id="prm_address"></a>
- **address**: One of four possible 7 bit addresses of the sensor.
  - *Valid values*: [gbj\_tmp102::ADDRESS\_GND](#addresses), [gbj\_tmp102::ADDRESS\_VCC](#addresses), [gbj\_tmp102::ADDRESS\_SDA](#addresses), [gbj\_tmp102::ADDRESS\_SCL](#addresses).
  - *Default value*: [gbj\_tmp102::ADDRESS\_GND](#addresses)
    - The default values is set to address corresponding to grounded ADD0 pin.
    - If input value is none of expected ones, the method fallbacks it to default address.
    - Implementing addressing allows up to 4 sensors present on the same two-wire bus.
    - Module boards with the sensor have usually ADD0 grounded and are equipped with soldering pad for reconnecting that pin to V+ rail.

#### Returns
Some of [result or error codes](#constants).

#### See also
[reset()](#reset)

[Back to interface](#interface)


<a id="reset"></a>
## reset()
#### Description
The method resets the sensor by the general call software reset sending the code `0x06` to the two-wire bus at address `0x00` and reads the content of the configuration register to the library instance object. Software reset causes resetting all internal registers to their power-up values, which determine following configuration and values:
- Upper alert temperature limit 80 centigrade.
- Lower alert temperature limit 75 centigrade.
- Normal mode (12 bit).
- Conversion rate 4 Hz.
- Continuous power mode (shutdown mode off).
- Thermostat (comparator) mode.
- Alert pin active low.
- Alert active.
- One fault for fault queue.
- One-shot conversion off.

#### Syntax
    uint8_t reset();

#### Parameters
None

#### Returns
Some of [result or error codes](#constants).

#### See also
[begin()](#begin)

[Back to interface](#interface)


<a id="measureTemperature"></a>
## measureTemperature()
#### Description
The method measures temperature.
- After each temperature reading the method reads the configuration register with the method [getConfiguration()](#getConfiguration) as well and its content stores in the instance object. It is essential for reading alert status with the method [getAlert()](#getAlert).

#### Syntax
    float measureTemperature();

#### Parameters
None

#### Returns
Temperature in centigrade or the error value [gbj\_tmp102::ERROR\_MEASURE\_TEMP](#errors) with corresponding error code in the library object.

#### See also
[measureTemperatureOneshot()](#measureTemperatureOneshot)

[Back to interface](#interface)


<a id="measureTemperatureOneshot"></a>
## measureTemperatureOneshot()
#### Description
The method configures shutdown mode and one-shot conversion of the sensor. It waits until conversion finishes and returns ambient temperature in centigrade.
- The method is useful at very long periods (couple of minutes and hours) between measurements in order to save power consumption.
- After each temperature reading the method reads the configuration register with the method [getConfiguration()](#getConfiguration) as well and its content stores in the instance object. It is essential for reading alert status with the method [getAlert()](#getAlert).

#### Syntax
    float measureTemperatureOneshot();

#### Parameters
None

#### Returns
Temperature in centigrade or the error value [gbj\_tmp102::ERROR\_MEASURE\_TEMP](#errors) with corresponding error code in the library object.

#### See also
[measureTemperature()](#measureTemperature)

[Back to interface](#interface)


<a id="calculateTemperature"></a>
## calculateTemperature()
#### Description
The particular method wraps a formula for calculating temperature in centigrades from 16-bit word from temperature register or vice-versa.
- The methods are suitable for storing temperatures in EEPROM as binary word instead of as float number.

#### Syntax
    float calculateTemperature(int16_t wordMeasure);
    int16_t calculateTemperature(float temperature);

#### Parameters
- **wordMeasure**: Temperature binary word of temperature register. If the least significant bit is set and there is extended mode bit set in configuration register, the value is considered in 13-bit resolution.
  - *Valid values*: integer
  - *Default value*: none

- **temperature**: Temperature in centigrade.
  - *Valid values*: -55.0 ~ 150.0
  - *Default value*: none

#### Returns
Temperature in centigrade or binary word representing temperature.

#### See also
[setAlertLow(), setAlertHigh(), setAlerts()](#setAlertValue)

[getAlertLow(), getAlertHigh()](#getAlertValue)

[Back to interface](#interface)


<a id="setConfiguration"></a>
## setConfiguration()
#### Description
The method writes the new content of the configuration register stored in the instance object (configuration cache) to the sensor. This content should has been prepared by methods of type `configXXX` right before.

#### Syntax
    uint8_t setConfiguration(bool flagWait);

#### Parameters
- **flagWait**: Flag about waiting after writing to the configuration register. The waiting delay is defined in the library internally. No delay is used in the method [measureTemperatureOneshot()](#measureTemperatureOneshot).
  - *Valid values*: true, false
  - *Default value*: true

#### Returns
Some of [result or error codes](#constants).

#### See also
[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="getConfiguration"></a>
## getConfiguration()
#### Description
The method reads configuration register and its value stores in the instance object, so that it caches it and enables it for corresponding getters.

#### Syntax
    uint8_t getConfiguration();

#### Parameters
None

#### Returns
Some of [result or error codes](#constants).

#### See also
[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="setAlertValue"></a>
## setAlertLow(), setAlertHigh(), setAlerts()
#### Description
The particular method writes either lower or upper temperature limit, or both at once to the sensor.
- If an illogical limit value in comparison to its counterpart is provided, the error [gbj\_tmp102::ERROR\_SETUP\_TEMP](#errors)  is raised, e.g., than lower limit is greater than upper limit.
- If both limits are set at once, they are sorted ascending at first.

#### Syntax
    uint8_t setAlertLow(float temperatureLow);
    uint8_t setAlertHigh(float temperatureHigh);
    uint8_t setAlerts(float temperatureLow, float temperatureHigh);

#### Parameters
- **temperatureLow**, **temperatureHigh**: Particular temperature limit in centigrade.
  - *Valid values*: -55.0 ~ 150.0
  - *Default value*: none

#### Returns
Some of [result or error codes](#constants).

#### See also
[getAlertLow(), getAlertHigh()](#getAlertValue)

[Back to interface](#interface)


<a id="getAlertValue"></a>
## getAlertLow(), getAlertHigh()
#### Description
The particular method reads upper or lower temperature limit from the sensor.

#### Syntax
    float getAlertLow();
    float getAlertHigh();

#### Parameters
None

#### Returns
Lower or upper temperature limit or an [error code](#errors) cached in the library object.

#### See also
[setAlertLow(), setAlertHigh(), setAlerts()](#setAlertValue)

[Back to interface](#interface)


<a id="configAlertMode"></a>
## configAlertActiveLow(), configAlertActiveHigh()
#### Description
The particular method updates alert activity bit state in the cached configuration value before its sending to the sensor by the method [setConfiguration()](#setConfiguration).

#### Syntax
    void configAlertActiveLow();
    void configAlertActiveHigh();

#### Parameters
None

#### Returns
None

#### See also
[getAlertActiveLow(), getAlertActiveHigh()](#getAlertMode)

[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="getAlertMode"></a>
## getAlertActiveLow(), getAlertActiveHigh()
#### Description
The particular method determines flag about alert activity mode from the cached configuration value.

#### Syntax
    bool getAlertActiveLow();
    bool getAlertActiveHigh();

#### Parameters
None

#### Returns
Flag about set particular alert activity mode.

#### See also
[configAlertActiveLow(), configAlertActiveHigh()](#configAlertMode)

[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="getAlert"></a>
## getAlert()
#### Description
The method provides flag about state of alert pin from cached configuration value.
- It is suitable for detecting the alert by software without need of hardware sensing the ALERT pin of the sensor.

#### Syntax
    bool getAlert();

#### Parameters
None

#### Returns
Flag about ALERT pin state.

#### See also
[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="configResolutionMode"></a>
## configExtendedMode(), configNormalMode()
#### Description
The particular method turns on corresponding resolution mode in the cached configuration value before its sending to the sensor by the method [setConfiguration()](#setConfiguration).
- At *normal* mode the resolution is the 12-bit resolution.
- At *extended* mode the resolution is the 13-bit resolution.
- Extended mode does not increase sensitivity, just extents the upper temperature measurement range from +128 to +150 centigrades. So that in normal working conditions it is not very useful.
- After changing resolution mode and writing it to the sensor it is needed to wait cca 350 milliseconds in order to settle the sensor and provide conversion. Otherwise the first conversion after changing resolution to extended mode from normal one doubles the measured temperature and after changing to normal mode from extended one halves the temperature, which might confuse follow-up logic or controlling mechanizm.
- The library does not have extra delay after resolution change implemented, so that it must be enforced in a sketch.

#### Syntax
    void configExtendedMode();
    void configNormalMode();

#### Parameters
None

#### Returns
None

#### See also
[getExtendedMode(), getNormalMode()](#getResolutionMode)

[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="getResolutionMode"></a>
## getExtendedMode(), getNormalMode()
#### Description
The particular method determines flag about resolution mode state from the cached configuration value.

#### Syntax
    bool getExtendedMode();
    bool getNormalMode();

#### Parameters
None

#### Returns
Flag about set particular resolution mode.

#### See also
[configExtendedMode(), configNormalMode()](#configResolutionMode)

[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="configPowerMode"></a>
## configShutdownMode(), configContinuousMode()
#### Description
The particular method turns on corresponding power mode in the cached configuration value before its sending to the sensor by the method [setConfiguration()](#setConfiguration).
- At *shutdown* mode the sensor turns on all its system except the serial interface and reduces power consumption. This mode is utilized by the method [measurementTemperatureOneshot()](#measurementTemperatureOneshot) for one-shot temperature measurement.
- At *continuous* mode the sensor performs continuous temperature conversion according to its [current conversion rate](#getConversionRate).

#### Syntax
    void configShutdownMode();
    void configContinuousMode();

#### Parameters
None

#### Returns
None

#### See also
[getShutdownMode(), getContinuousMode()](#getPowerMode)

[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="getPowerMode"></a>
## getShutdownMode(), getContinuousMode()
#### Description
The particular method determines flag about power mode state from the cached configuration value.

#### Syntax
    bool getShutdownMode();
    bool getContinuousMode();

#### Parameters
None

#### Returns
Flag about set particular power mode.

#### See also
[configShutdownMode(), configContinuousMode()](#configPowerMode)

[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="configActionMode"></a>
## configInterruptMode(), configThermostatMode()
#### Description
The particular method turns on corresponding action mode in the cached configuration value before its sending to the sensor by the method [setConfiguration()](#setConfiguration).
- At *interruption* mode the sensor generates a short impulse on ALERT pin at reaching particular temperature limit with particular polarity according to the [alert activity mode](#configAlertActivityMode).
- At *termostat* mode the sensor changes state of ALERT pin at reaching a temperature limit with particular polarity according to the [alert activity mode](#configAlertActivityMode) and keeps it until reaching another temperature limit.

#### Syntax
    void configInterruptMode();
    void configThermostatMode();

#### Parameters
None

#### Returns
None

#### See also
[getInterruptMode(), getThermostatMode()](#getActionMode)

[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="getActionMode"></a>
## getInterruptMode(), getThermostatMode()
#### Description
The particular method determines flag about action mode state from the cached configuration value.

#### Syntax
    bool getInterruptMode();
    bool getThermostatMode();

#### Parameters
None

#### Returns
Flag about set particular action mode.

#### See also
[configInterruptMode(), configThermostatMode()](#configActionMode)

[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="configConversionRate"></a>
## configConversionRate()
#### Description
The method sets conversion rate bits in the cached configuration value before its sending to the sensor by the method [setConfiguration()](#setConfiguration). The rate is determined with corresponding library class constant.

#### Syntax
    void configConversionRate(uint8_t conversionRate);

#### Parameters
- **conversionRate**: Value determining conversion rate. It fallbacks to least significant 2 bits.
  - *Valid values*: [gbj\_tmp102::CONVERSION\_RATE\_025HZ](#conversions) ~ [gbj\_tmp102::CONVERSION\_RATE\_8HZ](#conversions) or [gbj\_tmp102::CONVERSION\_PERIOD\_4000MS](#conversions) ~ [gbj\_tmp102::CONVERSION\_PERIOD\_125MS](#conversions)
  - *Default value*: none

#### Returns
None

#### See also
[getConversionRate()](#getConversionRate)

[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="getConversionRate"></a>
## getConversionRate()
#### Description
The method provides current conversion rate in form of value of pair of conversion rate bits from the cached configuration value. That value can be compared to corresponding library class constants in order to determine conversion frequency or period.

#### Syntax
    uint8_t getConversionRate();

#### Parameters
None

#### Returns
One of constants from ranges [gbj\_tmp102::CONVERSION\_RATE\_025HZ](#conversions) ~ [gbj\_tmp102::CONVERSION\_RATE\_8HZ](#conversions) or [gbj\_tmp102::CONVERSION\_PERIOD\_4000MS](#conversions) ~ [gbj\_tmp102::CONVERSION\_PERIOD\_125MS](#conversions).

#### See also
[configConversionRate()](#configConversionRate)

[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="configFaultQueue"></a>
## configFaultQueue()
#### Description
The method sets fault queue bits in the cached configuration value before its sending to the sensor by the method [setConfiguration()](#setConfiguration). The queue is determined with corresponding library class constant and states the number of consecutive faults for triggering ALERT pin change.

#### Syntax
    void configFaultQueue(uint8_t faults);

#### Parameters
- **faults**: Value determining consecutive faults for alerting. It fallbacks to least significant 2 bits.
  - *Valid values*: [gbj\_tmp102::FAULT\_QUEUE\_1](#conversions) ~ [gbj\_tmp102::FAULT\_QUEUE\_6](#conversions)
  - *Default value*: none

#### Returns
None

#### See also
[getFaultQueue()](#getFaultQueue)

[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="getFaultQueue"></a>
## getFaultQueue()
#### Description
The method provides current consecutive faults in form of value of pair of fault queue bits from the cached configuration value. That value can be compared to corresponding library class constants in order to determine number of consecutive faults.

#### Syntax
    uint8_t getFaultQueue();

#### Parameters
None

#### Returns
One of constants from range [gbj\_tmp102::FAULT\_QUEUE\_1](#conversions) ~ [gbj\_tmp102::FAULT\_QUEUE\_6](#conversions).

#### See also
[configFaultQueue()](#configFaultQueue)

[getConfiguration()](#getConfiguration)

[Back to interface](#interface)


<a id="configOneshotMode"></a>
## configOneshotMode()
#### Description
The method turns on one-shot temperature measurement mode in the cached configuration value before its sending to the sensor by the method [setConfiguration()](#setConfiguration).
- This mode is utilized by the method [measurementTemperatureOneshot()](#measurementTemperatureOneshot) for one-shot temperature measurement.

#### Syntax
    void configOneshotMode();

#### Parameters
None

#### Returns
None

#### See also
[getOneshotMode()](#getOneshotMode)

[setConfiguration()](#setConfiguration)

[Back to interface](#interface)


<a id="getOneshotMode"></a>
## getOneshotMode()
#### Description
The method provides current consecutive faults in form of value of pair of fault queue bits from the cached configuration value. That value can be compared to corresponding library class constants in order to determine number of consecutive faults.

#### Syntax
    bool getOneshotMode();

#### Parameters
None

#### Returns
Flag about set one-shot measurement mode.

#### See also
[configOneshotMode()](#configOneshotMode)

[getConfiguration()](#getConfiguration)

[Back to interface](#interface)
