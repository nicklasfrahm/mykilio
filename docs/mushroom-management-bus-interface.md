# Management bus interface

With the default open-source firmware the individual blades can be controlled via the [TWI (I<sup>2</sup>C) protocol][i2c]. The blades use **5V CMOS logic levels** for the TWI signals (SDA, SCL) on the management bus.

<!--
Some core concepts need to be established in order to understand the design of management interface. The concept of a **subsystem** describes a **distinct functional component**, such as the _power_ or the _thermal_ subsystem. Further within these subsystems there are different **classes of data** available, which are referred to as the **data domains**. The `status` data domain describes the **current state and status** of the system and is read-only, such as the version of the firmware for example. The `spec` data domain in contrast describes **desired state** of the system. These data domains are used to make the system [declarative and eventually consistent as described here](mycelium-overview.md#declarative-and-eventually-consistent). The `telemetry` data domain exposes frequently changing data, that will often be represented in a timeseries.
-->

## Address structure

All registers are addressed via 8-bit addresses, which follow a common scheme as described in the table below.

| Bits  | Name                  | Description                                                                       |
| ----- | --------------------- | --------------------------------------------------------------------------------- |
| `7:4` | Data&nbsp;category    | A 4-bit identifier that corresponds to a [data category][mycelium_data_category]. |
| `3:0` | Register&nbsp;address | A 4-bit address for a register within a [data category][mycelium_data_category].  |

## Registers

This section describes all available register addresses that are part of the interface. The _Access_ column specifies the access to the register, which be either **RW** indicating _read-write_ access or **RO** indicating _read-only_ access. The _Datatype_ column indirectly specifies the register length in **bytes** via the datatype expected to be read from or written to the specified register. All registers use most significant byte first byte ordering.

### Metadata registers

| Register | Address | Datatype  | Access | Name            | Description                                               |
| -------- | ------- | --------- | ------ | --------------- | --------------------------------------------------------- |
| ADRPTR   | 0x00    | `uint8_t` | RW     | Address pointer | The register address of the next read or write operation. |

### Status registers

| Register | Address | Datatype   | Access | Name                        | Description                                                                                      |
| -------- | ------- | ---------- | ------ | --------------------------- | ------------------------------------------------------------------------------------------------ |
| BMCCPU   | 0x10    | `char[16]` | RO     | [BMC][bmc] CPU              | A string providing the CPU name of the BMC.                                                      |
| BMCFWN   | 0x11    | `char[16]` | RO     | [BMC][bmc] firmware name    | A string providing the name of the installed firmware.                                           |
| BMCFWV   | 0x12    | `char[16]` | RO     | [BMC][bmc] firmware version | A string providing the semantic version of the firmware.                                         |
| BMCSTA   | 0x13    | `uint8_t`  | RO     | [BMC][bmc] state            | The state of the baseboard management controller enumerated as described [here][mycelium_state]. |
| SBCSTA   | 0x14    | `uint8_t`  | RO     | [SBC][sbc] state            | The state of the single board controller enumerated as described [here][mycelium_state].         |
| FANDCC   | 0x15    | `uint8_t`  | RO     | DC fan state                | The state of the DC controlled fan enumerated as described [here][mycelium_state].               |
| FANPWM   | 0x15    | `uint8_t`  | RO     | [PWM][pwm] fan state        | The state of the PWM controlled fan enumerated as described [here][mycelium_state].              |
| FANMIN   | 0x16    | `uint8_t`  | RO     | Minimum fan speed           | The minimum rotational speed of the fan in Hertz before entering the stop band.                  |
| FANMAX   | 0x17    | `uint8_t`  | RO     | Maximum fan speed           | The maximum rotational speed of the fan in Hertz at full power.                                  |
| DUTMIN   | 0x18    | `uint8_t`  | RO     | Minimum fan duty            | The minimum duty cycle of the fan control signal before entering the stop band.                  |
| DUTMAX   | 0x19    | `uint8_t`  | RO     | Maximum fan duty            | The maximum duty cycle of the fan control signal at full power.                                  |

<!-- Glossary -->

[i2c]: https://en.wikipedia.org/wiki/I%C2%B2C
[bmc]: https://en.wikipedia.org/wiki/Intelligent_Platform_Management_Interface#Baseboard_management_controller
[sbc]: https://en.wikipedia.org/wiki/Single-board_computer
[pwm]: https://en.wikipedia.org/wiki/Pulse-width_modulation
[mycelium_state]: mycelium-properties.md#state
[mycelium_data_category]: mycelium-overview.md#data-category
