# Management bus interface

With the default open-source firmware the individual blades can be controlled via a [TWI (I<sup>2</sup>C) communication bus][i2c]. The blades use **5V CMOS logic levels** for the TWI signals (SDA, SCL) on the management bus.

<!--
Some core concepts need to be established in order to understand the design of management interface. The concept of a **subsystem** describes a **distinct functional component**, such as the _power_ or the _thermal_ subsystem. Further within these subsystems there are different **classes of data** available, which are referred to as the **data domains**. The `status` data domain describes the **current state and status** of the system and is read-only, such as the version of the firmware for example. The `spec` data domain in contrast describes **desired state** of the system. These data domains are used to make the system [declarative and eventually consistent as described here](mycelium-overview.md#declarative-and-eventually-consistent). The `telemetry` data domain exposes frequently changing data, that will often be represented in a timeseries.
-->

## Address structure

All registers are addressed via 8-bit addresses, which follow a common scheme as described in the table below.

| Bits  | Name                  | Description                                                                       |
| ----- | --------------------- | --------------------------------------------------------------------------------- |
| [7:4] | Data&nbsp;category    | A 4-bit identifier that corresponds to a [data category][mycelium_data_category]. |
| [3:0] | Register&nbsp;address | A 4-bit address for a register within a [data category][mycelium_data_category].  |

## Register overview

This section describes all available register addresses that are part of the interface to access the blade's BMC[^1]. The _Access_ column specifies the access to the register, which be either **RW** indicating _read-write_ access or **RO** indicating _read-only_ access. The _Datatype_ column indirectly specifies the register length in **bytes** via the datatype expected to be read from or written to the specified register. All registers use most significant byte first byte ordering.

### Metadata registers

| Register | Address | Datatype  | Access | Name            | Description                                               |
| -------- | ------- | --------- | ------ | --------------- | --------------------------------------------------------- |
| ADRPTR   | 0x00    | `uint8_t` | RW     | Address pointer | The register address of the next read or write operation. |

### Status registers

| Register | Address | Datatype   | Access | Name                 | Description                                                                                      |
| -------- | ------- | ---------- | ------ | -------------------- | ------------------------------------------------------------------------------------------------ |
| BMCCPU   | 0x10    | `char[16]` | RO     | BMC CPU[^2]          | A string providing the CPU name of the BMC.                                                      |
| BMCFWN   | 0x11    | `char[16]` | RO     | BMC firmware name    | A string providing the name of the installed firmware.                                           |
| BMCFWV   | 0x12    | `char[16]` | RO     | BMC firmware version | A string providing the semantic version of the firmware.                                         |
| BMCSTA   | 0x13    | `uint8_t`  | RO     | BMC state            | The state of the baseboard management controller enumerated as described [here][mycelium_state]. |
| SBCSTA   | 0x14    | `uint8_t`  | RO     | SBC[^3] state        | The state of the single board controller enumerated as described [here][mycelium_state].         |
| PSUSTA   | 0x15    | `uint8_t`  | RO     | PSU[^4] state        | The state of the external power supply enumerated as described [here][mycelium_state].           |
| FANDCC   | 0x16    | `uint8_t`  | RO     | DC[^5] fan state     | The state of the DC controlled fan enumerated as described [here][mycelium_state].               |
| FANPWM   | 0x17    | `uint8_t`  | RO     | PWM[^6] fan state    | The state of the PWM controlled fan enumerated as described [here][mycelium_state].              |
| FANMIN   | 0x18    | `uint8_t`  | RO     | Minimum fan speed    | The minimum rotational speed of the fan in Hertz before entering the stop band.                  |
| FANMAX   | 0x19    | `uint8_t`  | RO     | Maximum fan speed    | The maximum rotational speed of the fan in Hertz at full power.                                  |
| DUTMIN   | 0x20    | `uint8_t`  | RO     | Minimum fan duty     | The minimum duty cycle of the fan control signal before entering the stop band.                  |
| DUTMAX   | 0x21    | `uint8_t`  | RO     | Maximum fan duty     | The maximum duty cycle of the fan control signal at full power.                                  |

### Specification registers

| Register          | Address | Datatype  | Access | Name                | Description                                                                         |
| ----------------- | ------- | --------- | ------ | ------------------- | ----------------------------------------------------------------------------------- |
| [FANMOD](#fanmod) | 0x30    | `uint8_t` | RW     | Fan control mode    | The mode used to control the fan enumerated as described [here](#fanmod).           |
| [FANFDB](#fanfdb) | 0x31    | `uint8_t` | RW     | Fan feedback source | The feedback source of the automatic fan control mode as described [here](#fanfdb). |
| [FANSET](#fanfdb) | 0x32    | `uint8_t` | RW     | Fan setpoint        | The sensor range percentage at which the fan should run with full speed.            |
| DUTSET            | 0x33    | `uint8_t` | RW     | Fan duty setpoint   | The duty setpoint for the fan in manual mode.                                       |

### Telemetry registers

| Register | Address | Datatype    | Access | Name                | Description                                                                              |
| -------- | ------- | ----------- | ------ | ------------------- | ---------------------------------------------------------------------------------------- |
| BMCVOL   | 0x50    | `float32_t` | R      | BMC voltage         | The current drawn by the baseboard management controller and all other electronics.      |
| BMCCUR   | 0x51    | `float32_t` | R      | BMC current         | The voltage supplied to the baseboard management controller and all other electronics.   |
| SBCVOL   | 0x52    | `float32_t` | R      | SBC voltage         | The current drawn by the single-board computer.                                          |
| SBCCUR   | 0x53    | `float32_t` | R      | SBC current         | The voltage supplied to the single-board computer.                                       |
| TMPAMB   | 0x54    | `float32_t` | R      | Ambient temperature | The ambient temperature.                                                                 |
| TMPPSU   | 0x55    | `float32_t` | R      | PSU temperature     | The temperature of the power supply or the VIN rail trace and is based on the backplane. |
| FANSPD   | 0x56    | `uint8_t`   | R      | Fan speed           | The current fan speed in Hertz.                                                          |
| FANDUT   | 0x57    | `uint8_t`   | R      | Fan duty cycle      | The current fan duty cycle.                                                              |

## Register description

The following section explains more detailed how the register values are to be interpreted.

### FANMOD

The fan control mode of the fan can be automatic, where `x` is the feedback signal as configured by [FANFDB](#fanfdb) and `a` is a preconfigured coefficient.

| Mode        | Value | Description                                                         |
| ----------- | ----- | ------------------------------------------------------------------- |
| Off         | 0     | The fan is disabled.                                                |
| Manual      | 1     | The fan is controlled by writing a value to the `DTYSET` register.  |
| Linear      | 2     | The fan is controlled according to the function `f(x) = a*x`.       |
| SquareRoot  | 3     | The fan is controlled according to the function `f(x) = a*sqrt(x)`. |
| Quadratic   | 4     | The fan is controlled according to the function `f(x) = a*x^2`.     |
| Exponential | 5     | The fan is controlled according to the function `f(x) = a*e^x`.     |

### FANFDB

The fan feedback source that is used to compute the duty for any fan control mode other than _Off_ or _Manual_.

| Feedback source     | Value | Minimum sensor value | Maximum sensor value |
| ------------------- | ----- | -------------------- | -------------------- |
| Ambient temperature | 0     | 0°C                  | 100°C                |
| PSU temperature     | 1     | 0°C                  | 100°C                |
| SBC current         | 2     | 0A                   | 5A                   |
| BMC current         | 3     | 0A                   | 5A                   |

### FANSET

The fan setpoint configures the value at which the fan will run with full speed. This value is used to compute the parameter `a` if the fan control mode ([FANMOD](#fanmod)) is not _Off_ or _Manual_. The fan setpoint is a percentage of the full-range sensor maximum and therefore unitless irrespective of which feedback source ([FANFDB](#fanfdb)) is selected.

Let's consider the following example: We want our fan to run at full speed if the ambient temperature sensor shows a value of 40°C.

Its maximum sensor value is 100°C. To determine the register value, we can perform the following calculation:

```c
float sensor_max = 100;
float setpoint = 40;

float setpoint_percent = setpoint / sensor_max; // 0.4

float setpoint_register_value = floor(setpoint_percent * 255); // 102
```

By writing the value `102` to our [FANSET](#fanset) register, the fan will now run on full speed if the sensor reads 40°C.

[^1]: **BMC:** [baseboard management controller][bmc]
[^2]: **CPU:** [central processing unit][cpu]
[^3]: **SBC:** [single-board computer][sbc]
[^4]: **PSU:** [power supply unit][psu]
[^5]: **DC:** [direct current][dc]
[^6]: **PWM:** [pulse-width modulation][pwm]

<!-- Glossary -->

[i2c]: https://en.wikipedia.org/wiki/I%C2%B2C
[bmc]: https://en.wikipedia.org/wiki/Intelligent_Platform_Management_Interface#Baseboard_management_controller
[cpu]: https://en.wikipedia.org/wiki/Central_processing_unit
[sbc]: https://en.wikipedia.org/wiki/Single-board_computer
[psu]: https://en.wikipedia.org/wiki/Power_supply
[dc]: https://en.wikipedia.org/wiki/Direct_current
[pwm]: https://en.wikipedia.org/wiki/Pulse-width_modulation
[mycelium_state]: mycelium-properties.md#state
[mycelium_data_category]: mycelium-overview.md#data-category
