# Resource properties

This document describes common resource properties and how their data is encoded. The resource properties here should be considered first when designing new resources or adding new resource properties to existing resources.

## Encoding

This section explains how different data types are to be encoded. Based on the capabilities of the device, the developer can choose between a binary encoding scheme or a JSON encoding scheme.

### Enumeration

If JSON is used to encode an enumeration, its value is represented as a `string`. If binary data is used to encode the value, it is represented as an enumerated `uint8`.

## Properties

### Health

The known health of a resource, which is encoded as [enumeration](#enumeration) and based on [Redfish][redfish].

| JSON     | Binary | Description                                        |
| -------- | ------ | -------------------------------------------------- |
| Critical | 0      | A critical condition requires immediate attention. |
| OK       | 1      | No conditions require special attention.           |
| Warning  | 2      | A condition requires attention.                    |

Libraries:

- **Go:** `Health` in [`github.com/nicklasfrahm/mykilio/pkg/props`][props_go]
- **C / C++:** `mykilio_health_t` in [`embedded/lib/mykilio.h`][mykilio_h]

### State

The known state of the resource, which is encoded as [enumeration](#enumeration) and based on [Redfish][redfish].

| JSON               | Binary | Description                                                                                                          |
| ------------------ | ------ | -------------------------------------------------------------------------------------------------------------------- |
| Absent             | 0      | This function or resource is either not present or detected.                                                         |
| Deferring          | 1      | The element does not process any commands but queues new requests.                                                   |
| Disabled           | 2      | This function or resource is disabled.                                                                               |
| Enabled            | 3      | This function or resource is enabled.                                                                                |
| InTest             | 4      | This function or resource is undergoing testing, or is in the process of capturing information for debugging.        |
| Qualified          | 5      | The element quality is within the acceptable range of operation.                                                     |
| Quiesced           | 6      | The element is enabled but only processes a restricted set of commands.                                              |
| StandbyOffline     | 7      | This function or resource is enabled but awaits an external action to activate it.                                   |
| StandbySpare       | 8      | This function or resource is part of a redundancy set and awaits a failover or other external action to activate it. |
| Starting           | 9      | This function or resource is starting.                                                                               |
| UnavailableOffline | 10     | This function or resource is present but cannot be used.                                                             |
| Updating           | 11     | The element is updating and might be unavailable or degraded.                                                        |

Libraries:

- **Go:** `State` in [`github.com/nicklasfrahm/mykilio/pkg/props`][props_go]
- **C / C++:** `mykilio_state_t` in [`embedded/lib/mykilio.h`][mykilio_h].

### Manager type

The type of manager that this resource represents, which is encoded as [enumeration](#enumeration) and based on [Redfish][redfish].

| JSON                 | Binary | Description                                                                                     |
| -------------------- | ------ | ----------------------------------------------------------------------------------------------- |
| AuxiliaryController  | 0      | A controller that provides management functions for a particular subsystem or group of devices. |
| BMC                  | 1      | A controller that provides management functions for a single computer system.                   |
| EnclosureManager     | 2      | A controller that provides management functions for a chassis or group of devices or systems.   |
| ManagementController | 3      | A controller that primarily monitors or manages the operation of a device or system.            |
| RackManager          | 4      | A controller that provides management functions for a whole or part of a rack.                  |
| Service              | 5      | A software-based service that provides management functions.                                    |

Libraries:

- **Go:** `ManagerType` in [`github.com/nicklasfrahm/mykilio/pkg/props`][props_go]
- **C / C++:** `mykilio_manager_type_t` in [`embedded/lib/mykilio.h`][mykilio_h].

[redfish]: https://www.dmtf.org/standards/redfish
[mykilio_h]: https://github.com/nicklasfrahm/mykilio/blob/main/embedded/lib/mykilio.h
[props_go]: https://github.com/nicklasfrahm/mykilio/blob/main/pkg/props.go
