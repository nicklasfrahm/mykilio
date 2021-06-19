# Mykilio

[![docs](https://github.com/nicklasfrahm/mykilio/actions/workflows/docs.yml/badge.svg?branch=main)](https://github.com/nicklasfrahm/mykilio/actions/workflows/docs.yml)
[![ecad](https://github.com/nicklasfrahm/mykilio/actions/workflows/ecad.yml/badge.svg?branch=main)](https://github.com/nicklasfrahm/mykilio/actions/workflows/ecad.yml)
[![infrastructure](https://github.com/nicklasfrahm/mykilio/actions/workflows/infrastructure.yml/badge.svg?branch=main)](https://github.com/nicklasfrahm/mykilio/actions/workflows/infrastructure.yml)
[![alpha](https://github.com/nicklasfrahm/mykilio/actions/workflows/alpha.yml/badge.svg?branch=main)](https://github.com/nicklasfrahm/mykilio/actions/workflows/alpha.yml)
[![controllers](https://github.com/nicklasfrahm/mykilio/actions/workflows/controllers.yml/badge.svg?branch=main)](https://github.com/nicklasfrahm/mykilio/actions/workflows/controllers.yml)

⚠️ **Experimental**: This repository is under active development. Usage in production is not advised, but any kind of feedback is welcome.

Mykilio is the proposal for a new Living Standard with the goal to reimagine infrastructure management and monitoring. It aims to be scalable, lightweight, extensible and secure by applying IoT principles to the datacenter and homelabs.

## 📖 Documentation

The documentation can be found at https://mykil.io.

## 📂 Other files

Tracking some files with Git is impractical, which is why these files can be found in this [Google Drive Folder](https://drive.google.com/drive/folders/1nM-uztG-QIkFoa5lGW787VEHX6BM3DAs?usp=sharing). Currently it contains the following content:

- `mcad/` - FreeCAD assemblies and part files

## 📊 Statistics

Here are some statistics to document my learnings throughout the development process.

| 💣 Fault         | 📈 Occurrences | 😅 Poor excuse                                                                                          |
| ---------------- | -------------- | ------------------------------------------------------------------------------------------------------- |
| Reverse polarity | 1              | Maybe purple and dark grey were not the best choice for +24V and GND.                                   |
| Overvoltage      | 1              | I have shaky hands and why does everything have to be so small?!                                        |
| Short-circuit    | 1              | Next time I am going to solder proper connectors. Promise!                                              |
| Wrong footprint  | 2              | Why do TQFP and DIP have a different pin numbering!?<br/>It was late and SS/TR is very close to RT/CLK. |

## 📄 License

This project is licensed under the terms of the [MIT license](./LICENSE.md).
