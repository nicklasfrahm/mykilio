# Management interface

With the default open-source firmware the individual blades can be controlled via the [TWI (I<sup>2</sup>C) protocol](https://en.wikipedia.org/wiki/I%C2%B2C). The blades use **5V CMOS logic levels** for the TWI signals (SDA, SCL) on the management bus. The registers exposed by the management interface can be accessed remotely via TWI.

Some core concepts need to be established in order to understand the design of management interface. The concept of a **subsystem** describes a **distinct functional component**, such as the _power_ or the _thermal_ subsystem. Further within these subsystems there are different **classes of data** available, which are referred to as the **data domains**. The `status` data domain describes the **current state and status** of the system and is read-only, such as the version of the firmware for example. The `spec` data domain in contrast describes **desired state** of the system. These data domains are used to make the system [declarative and eventually consistent as described here](mycelium-overview.md#declarative-and-eventually-consistent). The `telemetry` data domain exposes frequently changing data, that will often be represented in a timeseries.

All registers are addressed via 8-bit addresses, which follow a common scheme as described in the table below.

| Bits  | Name                      | Description                                                                                       |
| ----- | ------------------------- | ------------------------------------------------------------------------------------------------- |
| `7:6` | Subsystem&nbsp;identifier | One of 4 possible subsystems within the blade.                                                    |
| `5:4` | Data&nbsp;domain          | Can be either: <ul><li>status (`0b00`)</li><li>spec (`0b01`)</li><li>telemetry (`0b10`)</li></ul> |
| `3:0` | Register&nbsp;address     | A 4-bit address within the subsystem and the data domain.                                         |

The table below describes all available registers that can be addressed.

| Subsystem | Address | Access | Name                 | Description                                                                               |
| --------- | ------- | ------ | -------------------- | ----------------------------------------------------------------------------------------- |
| Metadata  | 0x00    | R/W    | Address&nbsp;pointer | Write a register address to this register to set the location of the next read operation. |
