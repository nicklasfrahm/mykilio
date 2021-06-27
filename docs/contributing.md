# Contributing

We would love for you to contribute. If you decide to do so, please have a look at this document about some conventions and practical hints.

## FreeCAD

When developing mechanical parts with FreeCAD, there are some things that should be considered regarding the internal structure of the mechanical parts:

1. If you create assemblies, please use the **Assembly 4** addon workbench in KiCAD.
1. When attaching parts inside assemblies, try to minimize the usage of offsets. This can be done by using attachment methods, such as **Concentric** or **Align O-X-Y**.
1. If the file is a result of a `.step`-file import, create a compound with the **KiCAD StepUp** addon workbench. Afterwards, create a new part and move the single shape into the part. Finally, delete all intermediate and hidden parts that were created in the process.
