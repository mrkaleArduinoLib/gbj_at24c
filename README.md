<a id="library"></a>
# gbjAT24C
Library for Atmel *AT24Cxxx* EEPROM chips communicating on two-wire (I2C) bus.
- Chip can have up to 8 addresses from `0x50` to `0x57` according to hardware configuration of its address pins.


#### Particle hardware configuration
- Connect microcontroller's pin `D0` to EEPROM's pin **SDA** (Serial Data).
- Connect microcontroller's pin `D1` to EEPROM's pin **SCL** (Serial Clock).

#### Arduino UNO hardware configuration
- Connect microcontroller's pin `A4` to EEPROM's pin **SDA** (Serial Data).
- Connect microcontroller's pin `A5` to EEPROM's pin **SCL** (Serial Clock).

#### Espressif - ESP8266, ESP32 default hardware configuration
- Connect microcontroller's pin `D2` to EEPROM's pin **SDA** (Serial Data).
- Connect microcontroller's pin `D1` to EEPROM's pin **SCL** (Serial Clock).


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
- **gbjMemory**: Memory custom library loaded from the file *gbj_memory.h*. The library [gbjat24c](#library) inherits common memory processing functionality from this library.
- **gbjTwoWire**: I2C custom library loaded from the file *gbj_twowire.h*. The library `gbjMemory` inherits common two-wire bus functionality from this library.


<a id="constants"></a>
## Constants
- **gbj\_at24c::VERSION**: Name and semantic version of the library.


<a id="addresses"></a>
#### Addresses
- **gbj\_at24c::ADDRESS\_MIN**: Minimal address (all address pins `LOW`).
- **gbj\_at24c::ADDRESS\_MAX**: Maximal address value (all address pins `HIGH`).


<a id="capacity"></a>
#### Supported EEPROM types and capacities
The last number in the type mark is the capacity of a chip in kibibit (1 Kib = 1024 bits). So that the usual byte capacity is 8 time lower and expressed in kibibytes (1 KiB = 1024 bytes).

- **gbj\_at24c::AT24C01**: 1 Kib, 1024 bits, 128 bytes.
- **gbj\_at24c::AT24C02**: 2 Kib, 2048 bits, 256 bytes.
- **gbj\_at24c::AT24C04**: 4 Kib, 4096 bits, 512 bytes.
- **gbj\_at24c::AT24C08**: 8 Kib, 8192 bits, 1024 bytes, 1 KiB.
- **gbj\_at24c::AT24C16**: 16 Kib, 16384 bits, 2048 bytes, 2 KiB.
- **gbj\_at24c::AT24C32**: 32 Kib, 32768 bits, 4096 bytes, 4 KiB.
- **gbj\_at24c::AT24C64**: 64 Kib, 65536 bits, 8192 bytes, 8 KiB.
- **gbj\_at24c::AT24C128**: 128 Kib, 131072 bits, 16384 bytes, 16 KiB.
- **gbj\_at24c::AT24C256**: 256 Kib, 262144 bits, 32768 bytes, 32 KiB.
- **gbj\_at24c::AT24C512**: 512 Kib, 524788 bits, 65536 bytes, 64 KiB.

 In fact, each of those class constant expresses an exponent of power of 2 determining the chip capacity in kibibits. For instance, the value of `gbj_at24c::AT24C64` constant is `6`, so that `2^^6 = 64 Kib`.

<a id="errors"></a>
#### Error codes
The library does not have its own error codes. There are only error codes defined by its parent classes [gbjMemory](#dependency) and its predecessors.


<a id="interface"></a>
## Interface

#### Main
- [gbj_at24c()](#gbj_at24c)
- [begin()](#begin)
- [detectType()](#detectType)

#### Getters
- [getType()](#getType)

Other possible setters and getters are inherited from the predecessor libraries `gbjMemory` and `gbjTwoWire`, and described there.


<a id="gbj_at24c"></a>
## gbj_at24c()
#### Description
The library does not need special constructor and destructor, so that the inherited ones are good enough and there is no need to define them in the library, just use it with default or specific parameters as defined at constructor of parent library [gbjTwoWire](#dependency).
- Constructor sets parameters specific to the two-wire bus in general.
- All the constructor parameters can be changed dynamically with corresponding setters later in a sketch.

#### Syntax
    gbj_at24c(uint32_t clockSpeed, uint8_t pinSDA, uint8_t pinSCL);

#### Parameters
<a id="prm_busClock"></a>
- **clockSpeed**: Two-wire bus clock frequency in Hertz. If the clock is not from enumeration, it fallbacks to 100 kHz.
  - *Valid values*: gbj\_at24c::CLOCK\_100KHZ, gbj\_at24c::CLOCK\_400KHZ
  - *Default value*: gbj\_at24c::CLOCK\_100KHZ


<a id="prm_pinSDA"></a>
- **pinSDA**: Microcontroller's pin for serial data. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms for communication on the bus. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  - *Valid values*: positive integer
  - *Default value*: 4 (GPIO4, D2)


<a id="prm_pinSCL"></a>
- **pinSCL**: Microcontroller's pin for serial clock. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  - *Valid values*: positive integer
  - *Default value*: 5 (GPIO5, D1)

#### Returns
Object performing the EEPROM management.
The constructor cannot return [a result or error code](#constants) directly, however, it stores them in the instance object. The result can be tested in the operational code with the inhereted method `getLastResult()`, `isError()`, or `isSuccess()`.

[Back to interface](#interface)


<a id="begin"></a>
## begin()
#### Description
The method takes, sanitizes, and stores EEPROM parameters to a class instance object and initiates two-wire bus.
- The method sets parameters specific to the EEPROM itself.

#### Syntax
    uint8_t begin(nt8_t type, uint8_t address);

#### Parameters
<a id="prm_type"></a>
- **type**: One of 10 supported Atmel EEPROM chip types. The input value is limited to maximal supported capacity.
  - *Valid values*: [gbj\_at24c::AT24C01 ~ gbj\_at24c::AT24C512](#capacity).
  - *Default value*: None


<a id="prm_address"></a>
- **address**: One of 8 possible 7 bit addresses of the EEPROM chip or binary representation of the address pins configuration, i.e., increment to the basic (minimal) address.
  - *Valid values*: [gbj\_at24c::ADDRESS\_MIN ~ gbj\_at24c::ADDRESS\_MAX](#addresses).
  - *Default value*: [gbj\_at24c::ADDRESS\_MIN](#addresses)
    - The default values is set to address corresponding to all address pins grounded.
    - The input value is limited to maximal address.
    - Implementing addressing allows up to 8 EEPROMs present on the same two-wire bus.

#### Returns
Some of [result or error codes](#constants).

[Back to interface](#interface)


<a id="detectType"></a>
## detectType()
#### Description
The method detects the type of the EEPROM chip by detecting its capacity.
- The method tests type from the highest supported capacity.
- The test is based on writing specific value to 0 position of the EEPROM
  and another specific value to the first position beyond the capacity of the
  previous supported type. If the tested type is not correct, the EEPROM
  rewrites the value in 0 position with tested value, which is different from
  the reference value written directly to 0 position. The methods decreases
  tested types until the 0 position is not rewritten.
- The method really rewrites just 0 position of EEPROM and the position in the
  middle of detected type capacity, e.g., for AT24C256 chip it is position
  16384.

#### Syntax
    uint8_t detectType(uint8_t &type);

#### Parameters
- **type**: Referenced variable for placing detected EEPROM type.
  - *Valid values*: [gbj\_at24c::AT24C01 ~ gbj\_at24c::AT24C512](#capacity)
  - *Default value*: None

#### Returns
Some of [result or error codes](#constants).

[Back to interface](#interface)


<a id="getType"></a>
## getType()
#### Description
The method provides current type of the EEPROM chip set by the [begin()](#begin) method.

#### Syntax
    uint8_t getType();

#### Parameters
None

#### Returns
EEPROM type defined by one from class constants [gbj\_at24c::AT24C01 ~ gbj\_at24c::AT24C512](#capacity).

#### See also
[detectType()](#detectType)

[begin()](#begin)

[Back to interface](#interface)
