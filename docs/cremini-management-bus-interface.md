# Management bus interface

With the default open-source firmware the individual blades can be controlled via a [TWI (I<sup>2</sup>C) communication bus][i2c]. The blades use **5V CMOS logic levels** for the TWI signals (SDA, SCL) on the management bus.

## Address structure

All registers are addressed via 8-bit addresses, which follow a common scheme as described in the table below.

| Bits  | Name                  | Description                                                                      |
| ----- | --------------------- | -------------------------------------------------------------------------------- |
| [7:4] | Data&nbsp;category    | A 4-bit identifier that corresponds to a [data category][mykilio_data_category]. |
| [3:0] | Register&nbsp;address | A 4-bit address for a register within a [data category][mykilio_data_category].  |

## Data framing

The reader is expected to have basic familiarity with the principles and bus states of TWI. SDA is the serial data line, while SCL is the serial clock line.

A _signal_ is a bus event, such as **START** or **ACK**, whereas an _operation_ is the combination of signals between **two START signals** or the **last START** and the **STOP signal**. A _transaction_ in contrast is the combination of signals between the **first START** and **STOP signal**.

Independent of the fact if it is a read or a write _transaction_, the following operations are performed to interact with remote register via TWI.

1.  The client sends **START** signal by pulling SDA low, while SCL is high.

1.  The client writes the _server address byte_ to the bus, which specifies the server it would like to address. The least significant bit of this byte is the **R/W bit**. If it is set to `1` it indicates a read _operation_, whereas if it is set to `0`, it indicates a write _operation_.

1.  The server acknowledges the receival by pulling SDA low when SCL is high and therefore sending an **ACK** signal.

1.  The client sends the _register address byte_ to the bus, which indicates the register address it wants to write to. This **must** be `0x00` if a read _transaction_ is to be performed. Writing to register address `0x00`sets the \_ _address pointer_ on the server and thus indicating the location of the next read _operation_.

1.  The server sends an **ACK** signal.

a. If it is a _write transaction_, the following steps are performed after step 5:

6. The client sends `n` more **data bytes**, which are each acknowlegded by the server.

7. The client sends a **STOP** condition by releasing the SDA line to a high signal, while SCL is high.

b. In contrast, if it is a _read transaction_, the following step are further performed after step 5:

6. The client sends a single **data byte** containing the register address to be read to the server and the server sends an **ACK** signal.

7. The client sends another **START** signal, often also referred to as **REPEATED START** by pulling SDA low while SCL is high.
8. The client sends the _address byte_ with the **R/W** bit set to `1` to indicate a read operation.
9. The server sends an **ACK** signal.

10. Now the server sends `n` **data bytes**, which are each acknowlegded by the server with an **ACK** signal.

11. The _transaction_ is ended by the client sending a **NACK** signal to request the server to release the bus. This is then followed up by the client sending a **STOP** condition.

## Register overview

This section describes all available register addresses that are part of the interface. The _Access_ column specifies the access to the register, which can be either **RW** indicating _read-write_ access or **RO** indicating _read-only_ access. The _Datatype_ column indirectly specifies the register length in **bytes** via the datatype expected to be read from or written to the specified register. All registers use most significant byte first byte ordering.

### Metadata registers

| Register | Address | Datatype | Access | Name            | Description                                               |
| -------- | ------- | -------- | ------ | --------------- | --------------------------------------------------------- |
| ADRPTR   | 0x00    | `uint8`  | RW     | Address pointer | The register address of the next read or write operation. |

### Status registers

| Register | Address | Datatype   | Access | Name                 | Description                                                                                     |
| -------- | ------- | ---------- | ------ | -------------------- | ----------------------------------------------------------------------------------------------- |
| BMCSTA   | 0x10    | `uint8`    | RO     | BMC[^1] state        | The state of the baseboard management controller enumerated as described [here][mykilio_state]. |
| SBCSTA   | 0x11    | `uint8`    | RO     | SBC[^2] state        | The state of the single board controller enumerated as described [here][mykilio_state].         |
| PSUSTA   | 0x12    | `uint8`    | RO     | PSU[^3] state        | The state of the external power supply enumerated as described [here][mykilio_state].           |
| FANDCC   | 0x13    | `uint8`    | RO     | DC[^4] fan state     | The state of the DC controlled fan enumerated as described [here][mykilio_state].               |
| FANPWM   | 0x14    | `uint8`    | RO     | PWM[^5] fan state    | The state of the PWM controlled fan enumerated as described [here][mykilio_state].              |
| FANMIN   | 0x15    | `uint8`    | RO     | Minimum fan speed    | The minimum rotational speed of the fan in Hertz before entering the stop band.                 |
| FANMAX   | 0x16    | `uint8`    | RO     | Maximum fan speed    | The maximum rotational speed of the fan in Hertz at full power.                                 |
| DUTMIN   | 0x17    | `uint8`    | RO     | Minimum fan duty     | The minimum duty cycle of the fan control signal before entering the stop band.                 |
| DUTMAX   | 0x18    | `uint8`    | RO     | Maximum fan duty     | The maximum duty cycle of the fan control signal at full power.                                 |
| BMCCPU   | 0x19    | `char[16]` | RO     | BMC CPU[^6]          | A string providing the CPU name of the BMC.                                                     |
| BMCFWN   | 0x20    | `char[16]` | RO     | BMC firmware name    | A string providing the name of the installed firmware.                                          |
| BMCFWV   | 0x21    | `char[16]` | RO     | BMC firmware version | A string providing the semantic version of the firmware.                                        |

### Specification registers

| Register          | Address | Datatype | Access | Name                | Description                                                                                              |
| ----------------- | ------- | -------- | ------ | ------------------- | -------------------------------------------------------------------------------------------------------- |
| [SBCPON](#sbcpon) | 0x30    | `uint8`  | RW     | SBC power-on        | The desired state of the single-board computer power rail enumerated as described [here][mykilio_state]. |
| [SBCSON](#sbcson) | 0x31    | `uint8`  | RW     | SBC soft-on         | The desired state of the single-board computer enumerated as described [here][mykilio_state].            |
| [FANMOD](#fanmod) | 0x32    | `uint8`  | RW     | Fan control mode    | The mode used to control the fan enumerated as described [here](#fanmod).                                |
| [FANFDB](#fanfdb) | 0x33    | `uint8`  | RW     | Fan feedback source | The feedback source of the automatic fan control mode as described [here](#fanfdb).                      |
| [FANSET](#fanset) | 0x34    | `uint8`  | RW     | Fan setpoint        | The sensor range percentage at which the fan should run with full speed.                                 |
| [FANMAN](#fanman) | 0x35    | `uint8`  | RW     | Fan manual setpoint | The duty setpoint for the fan in manual mode.                                                            |
| [LEDMOD](#ledmod) | 0x36    | `uint8`  | RW     | LED control mode    | The mode used to control the LED enumerated as described [here](#ledmod).                                |
| [LEDFDB](#fanfdb) | 0x37    | `uint8`  | RW     | LED feedback source | The feedback source of the automatic LED control mode as described [here](#ledfdb).                      |
| [LEDSET](#fanset) | 0x38    | `uint8`  | RW     | LED setpoint        | The sensor range percentage at which the LED should light up with full brightness.                       |
| [LEDMAN](#fanman) | 0x39    | `uint8`  | RW     | LED manual setpoint | The duty setpoint for the LED in manual mode.                                                            |

### Telemetry registers

| Register | Address | Datatype | Access | Name                | Description                                                                              |
| -------- | ------- | -------- | ------ | ------------------- | ---------------------------------------------------------------------------------------- |
| FANSPD   | 0x50    | `uint8`  | RO     | Fan speed           | The current fan speed in Hertz.                                                          |
| FANDUT   | 0x51    | `uint8`  | RO     | Fan duty cycle      | The current fan duty cycle.                                                              |
| BMCVOL   | 0x52    | `float`  | RO     | BMC voltage         | The current drawn by the baseboard management controller and all other electronics.      |
| BMCCUR   | 0x53    | `float`  | RO     | BMC current         | The voltage supplied to the baseboard management controller and all other electronics.   |
| SBCVOL   | 0x54    | `float`  | RO     | SBC voltage         | The current drawn by the single-board computer.                                          |
| SBCCUR   | 0x55    | `float`  | RO     | SBC current         | The voltage supplied to the single-board computer.                                       |
| TMPAMB   | 0x56    | `float`  | RO     | Ambient temperature | The ambient temperature.                                                                 |
| TMPPSU   | 0x57    | `float`  | RO     | PSU temperature     | The temperature of the power supply or the VIN rail trace and is based on the backplane. |

### Action registers

| Register          | Address | Datatype | Access | Name                        | Description                                                                        |
| ----------------- | ------- | -------- | ------ | --------------------------- | ---------------------------------------------------------------------------------- |
| [ACTPCY](#actpcy) | 0x70    | `uint8`  | RW     | Power-cycle action          | Power-cycle the single-board computer by disconnecting and reconnecting the power. |
| [ACTREB](#actreb) | 0x71    | `uint8`  | RW     | Reboot action               | Reboot the single-board computer by halting it via GPIO[^7] and restarting it.     |
| [ACTENU](#actenu) | 0x72    | `uint8`  | RW     | Hardware enumeration action | Enumerate the connected hardware and update the status.                            |

## Register description

The following section explains more detailed how the register values are to be interpreted.

### SBCPON

This register controls the power rail of the single-board computer. Writing a value of `M6M_STATE_DISABLED` or `2` to this register will disable the power rail, while writing a value of `M6M_STATE_ENABLED` or `3` to the register will enable the power rail. Other values are ignored and have no effect.

<!-- prettier-ignore -->
!!! danger "Danger: Potential data loss"
    Interrupting power without shutting the single-board computer down may cause data loss, especially if the storage medium is an SD card. Please consider using the soft-on ([SBCSON](#sbcson)) register instead.

### SBCSON

This register controls the system state of the single board computer via a GPIO. Writing a value of `M6M_STATE_DISABLED` or `2` to this register will ensure that the single-board computer is off, while writing a value of `M6M_STATE_ENABLED` or `3` to the register will ensure that the single-board computer is on.

<!-- prettier-ignore -->
!!! note "Note: Limited support"
    Not all single board computers support this. Currently this is only known to be working for the Raspberry Pi with the [`gpio-shutdown` devicetree overlay][rpi-gpio-shutdown].

### FANMOD

The fan control mode of the fan can be automatic, where `x` is the feedback signal as configured by [FANFDB](#fanfdb) and `a` is automatically calculated based on the configuration of the fan setpoint ([FANSET](#fanset)) register.

| Mode        | Value | Description                                                                 |
| ----------- | ----- | --------------------------------------------------------------------------- |
| Off         | 0     | The fan is disabled.                                                        |
| Manual      | 1     | The fan is controlled by writing a value to the [FANMAN](#fanman) register. |
| Linear      | 2     | The fan is controlled according to the function `f(x) = a*x`.               |
| SquareRoot  | 3     | The fan is controlled according to the function `f(x) = a*sqrt(x)`.         |
| Quadratic   | 4     | The fan is controlled according to the function `f(x) = a*x^2`.             |
| Exponential | 5     | The fan is controlled according to the function `f(x) = a*e^x`.             |

### FANFDB

The fan feedback source that is used to compute the duty for any fan control mode other than _Off_ or _Manual_.

| Feedback source     | Value | Minimum sensor value | Maximum sensor value |
| ------------------- | ----- | -------------------- | -------------------- |
| Ambient temperature | 0     | 0°C                  | 100°C                |
| PSU temperature     | 1     | 0°C                  | 100°C                |
| BMC current         | 2     | 0A                   | 5A                   |
| SBC current         | 3     | 0A                   | 5A                   |

### FANSET

The fan setpoint configures the value at which the fan will run with full speed. This value is used to compute the parameter `a` if the fan control mode ([FANMOD](#fanmod)) is not _Off_ or _Manual_. The fan setpoint is a percentage of the full-range sensor maximum and therefore unitless irrespective of which feedback source ([FANFDB](#fanfdb)) is selected.

<!-- prettier-ignore -->
!!! example "Example: Controlling the fan based on a temperature measurement"
    Let's assume that we want don't want our CPU to overheat. Our requirement is that we want to run the fan at full speed if the ambient temperature sensor reads a value above 40°C.

    The maximum sensor value is of this sensor is 100°C as described [here](#fanfdb). To determine the register value, we can use the following code:

    ```cpp linenums="1"
    --8<-- "examples/docs/fanset_temperature.c"
    ```

    Create a file called `calc.c`, paste the code above and run `gcc -o calc calc.c -lm && ./calc && rm calc`. You should get the following output:

    ```txt
    Full-speed temperature: 40°C
    Fan setpoint: 102
    ```

    By writing the value `102` to our [FANSET](#fanset) register, the fan will now run on full speed if the sensor reads 40°C.

<!-- prettier-ignore -->
!!! example "Example: Controlling the fan based on compute load"
    Let's assume that we want to run our fan based on the compute load of the single board computer. We can indirectly measure the load by observing the current draw, so our requirement is that we our fan to run at full speed if the current sensor reads a value above 1.5A.

    The maximum sensor value is of this sensor is 5A as described [here](#fanfdb). To determine the register value, we can use the following code:

    ```cpp linenums="1"
    --8<-- "examples/docs/fanset_power.c"
    ```

    Create a file called `calc.c`, paste the code above and run `gcc -o calc calc.c -lm && ./calc && rm calc`. You should get the following output:

    ```txt
    Full-speed power: 7.5W
    Fan setpoint: 76
    ```

    By writing the value `76` to our [FANSET](#fanset) register, the fan will now run on full speed if the sensor reads 1.5A.

### FANMAN

Writing a value to this register will set the duty of the fan if the fan mode ([FANMOD](#fanmod)) is set to _Manual_. `0` corresponds to a duty of 0%, while `255` corresponds to 100%. Any value between `0` and `255` is valid.

### LEDMOD

Please use the [FANMOD](#fanmod) register configuration reference as the LED control mode register accepts the same options.

### LEDFDB

Please use the [FANFDB](#fanfdb) register configuration reference as the LED feedback source register accepts the same options.

### LEDSET

Please use the [FANSET](#fanset) register configuration reference as the LED setpoint register accepts the same options.

### LEDMAN

Please use the [FANMAN](#fanman) register configuration reference as the LED manual override register accepts the same options.

### ACTPCY

Writing any value other than `0` to this register will issue a power-cycle of the single-board computer.

<!-- prettier-ignore -->
!!! danger "Danger: Potential data loss"
    Issuing a power-cycle may cause data loss, especially if the storage medium is an SD card. Please consider using the reboot action ([ACTREB](#actreb)) instead.

### ACTREB

Writing any value other than `0` to this register will issue a soft reboot of the single-board computer using a GPIO pin.

<!-- prettier-ignore -->
!!! note "Note: Limited support"
    Not all single board computers support this. Currently this is only known to be working for the Raspberry Pi with the [`gpio-shutdown` devicetree overlay][rpi-gpio-shutdown].

### ACTENU

Writing any value other than `0` to this register will start an enumeration process that will gather the information for most of the [status registers](#status-registers) described earlier.

## Libraries

To interface with the blade via the management bus interface, the following libraries are provided:

- **C / C++:** [`mushroom.h`][mushroom_h].

[^1]: **BMC:** [baseboard management controller][bmc]
[^2]: **SBC:** [single-board computer][sbc]
[^3]: **PSU:** [power supply unit][psu]
[^4]: **DC:** [direct current][dc]
[^5]: **PWM:** [pulse-width modulation][pwm]
[^6]: **CPU:** [central processing unit][cpu]
[^7]: **GPIO:** [general-purpose input/output][gpio]

<!-- Glossary -->

[i2c]: https://en.wikipedia.org/wiki/I%C2%B2C
[bmc]: https://en.wikipedia.org/wiki/Intelligent_Platform_Management_Interface#Baseboard_management_controller
[cpu]: https://en.wikipedia.org/wiki/Central_processing_unit
[sbc]: https://en.wikipedia.org/wiki/Single-board_computer
[psu]: https://en.wikipedia.org/wiki/Power_supply
[dc]: https://en.wikipedia.org/wiki/Direct_current
[pwm]: https://en.wikipedia.org/wiki/Pulse-width_modulation
[gpio]: https://en.wikipedia.org/wiki/General-purpose_input/output
[mykilio_state]: mykilio-resource-properties.md#state
[mykilio_data_category]: mykilio-overview.md#data-category
[rpi-gpio-shutdown]: https://github.com/raspberrypi/firmware/blob/master/boot/overlays/README#L1015
[mushroom_h]: https://github.com/nicklasfrahm/mykilio/blob/main/embedded/lib/mushroom.h
