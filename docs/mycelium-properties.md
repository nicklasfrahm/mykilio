# Properties

This document describes common system properties and how their data is encoded.

## State

The known state of the resource. If JSON is used to encode the value, it is represented as a `string`. If binary data is used to encode the value, it is represented as an enumerated `uint8_t`. The enumerated values are based on [Redfish][redfish].

| String             | Binary | Description                                                                                                          |
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

Below you may find instructions on how to use this property with your language of choice:

- **C / C++:** `m6m_state_t` in [`mycelium.h`][mycelium_h].

[redfish]: https://www.dmtf.org/standards/redfish
[mycelium_h]: https://github.com/nicklasfrahm/mycelium/blob/main/src/include/mycelium.h
