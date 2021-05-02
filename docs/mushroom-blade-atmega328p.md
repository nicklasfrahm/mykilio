# Blade (ATmega328P)

This variant of the blade is based on the very popular ATmega328P microcontroller that is also used in the Arduino Uno and the Arduino Nano. Due to its usage as a microcontroller on these popular developer boards, it has a well-established online community, which provides a lot of resources on how to develop applications with this microcontroller.

## Schematic

The electronic schematic of the PCB can be found [here](https://github.com/nicklasfrahm/mycelium/blob/main/docs/mushroom-blade-atmega328p.pdf).

## Firmware

To build the firmware, start the development container and run:

```bash
$ make
```

To clean the build artifacts, run:

```bash
$ make clean
```

If you are using Windows or WSL 2, you can download [AVRDUDE for Windows](https://github.com/mariusgreuel/avrdude). Install it by placing the files from the archive in `C:\Windows\System32` and renaming `avrdude.exe` to `avrdude`. Now you can open your repository folder in Git Bash or WSL 2 and run:

```bash
$ PORT=COM3 make flash
```

**Note:** You can't flash from within the development container. So make sure to run `make` in your development container first before flashing the firmware with `make flash` from outside your container.

### Connection issues

If you are not able to flash the firmware to the device, this may have several reasons. One potential cause can be missing, bricked or outdated bootloader. Make sure you are using the most recent bootloader by having a look at the [bootloader section](#bootloader).

If you don't feel comfortable updating the bootloader and you are sure that the board has a bootloader installed, the issue could be caused by a BAUD mismatch of the bootloader and the programmer. By default the `Makefile` will use a BAUD of `115200` to be compatible with the bootloader that is part of this repository. Some older devices however come with an old bootloader that only supports a BAUD of `57600`. You can adjust the BAUD by changing the command to:

```bash
$ PORT=COM3 BAUD=57600 make flash
```

## Bootloader

If the board is factory-new, it does not have a bootloader installed. Therefore it is not possible to install any firmware via the USB to serial interface. The bootloader can be installed via the in-circuit serial programming (ICSP) header by the help of an in-system programmer.

For this example, we will use two Arduino Nanos with an ATmega328P, because this will come very close to flashing the bootloader to the actual board via the ISCP header. The **programmer** has the most recent bootloader installed and therefore uses a BAUD of `115200`. The **target** has no bootloader or an old version installed.

Begin by wiring up the ICSP headers of the Arduinos as shown in the diagram below.

![Connection between Arduino Nano ICSP headers for bootloader burning](assets/img/bootloader-burning-connection.jpg)

The first command will install the [Arduino ISP programmer firmware](https://github.com/arduino/arduino-examples/blob/main/examples/11.ArduinoISP/ArduinoISP/ArduinoISP.ino) on the programmer Arduino.

```bash
$ PORT=COM3 make isp
```

The second command will install the bootloader by first setting the fuses for the clock configuration and other important settings and then flashing the [Optiboot bootloader](https://github.com/Optiboot/optiboot) onto the device.

```bash
$ PORT=COM3 make burn
```

## Acknowledgements

I would like to thank Kasper Laursen ([@Clapfire](https://github.com/Clapfire)) for teaching me how to use KiCAD as well as Christian Mai for reviewing my schematic design and giving me invaluable feedback to improve the board design.
