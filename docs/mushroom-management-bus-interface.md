# Management bus interface

With the default open-source firmware the individual blades can be controlled via the [TWI (I<sup>2</sup>C) protocol][i2c]. The blades use **5V CMOS logic levels** for the TWI signals (SDA, SCL) on the management bus.

<!--
Some core concepts need to be established in order to understand the design of management interface. The concept of a **subsystem** describes a **distinct functional component**, such as the _power_ or the _thermal_ subsystem. Further within these subsystems there are different **classes of data** available, which are referred to as the **data domains**. The `status` data domain describes the **current state and status** of the system and is read-only, such as the version of the firmware for example. The `spec` data domain in contrast describes **desired state** of the system. These data domains are used to make the system [declarative and eventually consistent as described here](mycelium-overview.md#declarative-and-eventually-consistent). The `telemetry` data domain exposes frequently changing data, that will often be represented in a timeseries.
-->

## Address structure

All registers are addressed via 8-bit addresses, which follow a common scheme as described in the table below.

| Bits  | Name                      | Description                                          |
| ----- | ------------------------- | ---------------------------------------------------- |
| `7:4` | Subsystem&nbsp;identifier | A logical subsystem within the blade.                |
| `3:0` | Register&nbsp;address     | A 4-bit address for a register within the subsystem. |

## Register addresses

The table below describes all available register addresses, where **RW** indicates **read-write** and **RO** indicates **read-only** access to the register. For each register its length is given in bytes. An in the following section the word _string_ refers to a char array in C.

| Register | Address | Length | Access | Name                        | Description                                                                                      |
| -------- | ------- | ------ | ------ | --------------------------- | ------------------------------------------------------------------------------------------------ |
| ADRPTR   | 0x00    | 1      | RW     | Address pointer             | The register address of the next read or write operation.                                        |
| BMCCPU   | 0x10    | 16     | RO     | [BMC][bmc] CPU              | A string providing the CPU name of the BMC.                                                      |
| BMCFWN   | 0x11    | 16     | RO     | [BMC][bmc] firmware name    | A string providing the name of the installed firmware.                                           |
| BMCFWV   | 0x12    | 16     | RO     | [BMC][bmc] firmware version | A string providing the semantic version of the firmware.                                         |
| BMCSTA   | 0x13    | 1      | RO     | [BMC][bmc] state            | The state of the baseboard management controller enumerated as described [here][mycelium_state]. |

## Registers

The following section describes the registers and the information the individual bits represent.

### BMCSTA

<!-- Glossary -->

[i2c]: https://en.wikipedia.org/wiki/I%C2%B2C
[bmc]: https://en.wikipedia.org/wiki/Intelligent_Platform_Management_Interface#Baseboard_management_controller
[mycelium_state]: mycelium-properties.md#state
