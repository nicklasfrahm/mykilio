# Mushroom

Mushroom is a open source and open hardware blade server. It can take several single board computers that are pin-compatible with the Raspberry Pi and follow the hardware dimensions of the Model B variant of the Raspberry Pi.

## Features ✨

- Flexibility
  - **Modular design:** Start with a single node and scale up to 16
  - **Hackable hardware:** Write your own custom firmware for the blade
  - **Raspberry Pi support:** Connect any development board compatible with the Raspberry Pi
  - **Built-in LED:** Give custom status feedback
- Node management
  - Soft power-on and power-off
  - Hard reset via power-disconnect
  - Serial connection passthrough
- Sensors
  - Current and voltage probes for single board computer and blade
  - Single board computer connection detection
  - Power supply and ambient temperature probes
- Chassis fan (optional)
  - Standard 4-pin PWM fan header
  - PWM or DC fan control
  - Automatic control mode selection via power-on self test (POST)
- Reliability
  - USB backpower protection
  - ICSP header for bootloader flashing

## Components 📦

The blade server is used as a hardware platform for the reference implementation of the Mycelium API and it consists of the following components:

- **Blade:** A board that is attached directly to the GPIO header of the single board computer. It monitors power usage, controls the power state and can be controlled via TWI / I<sup>2</sup>C. Currently the following variants of the blade are available:

  - [Blade (ATmega328P)](mushroom-blade-atmega328p.md)

- **Backplane:** A board to supply the blade and the single board computers with power and connect them to a management bus. The backplane is available in the following variants:

  - Backplane (TPS54560)
  - Backplane (TPS54561)
  - Backplane (DM28)

## Roadmap 🔮

- **Manager card**: An ESP32 based PCB that connects to the backplane management bus and implements the Mycelium API for the blades.
- **Redundant power supply backplane**: A PCB that connects to the backplane management bus and allows to connect two redundant power supplies to be connected and controlled via the controller board.
