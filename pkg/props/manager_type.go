package props

import (
	"github.com/rs/zerolog/log"
)

// Enumeration is a type to store different encoding schemes of an enumeration.
type Enumeration struct {
	// JSON encodes the enumeration as a string.
	JSON string
	// Binary encodes the enumeration as a uint8.
	Binary uint8
}

// Health is the known health of a resource, which is encoded as enumeration and based on Redfish.
type Health Enumeration

var (
	// HealthCritical indicates that a critical condition requires immediate attention.
	HealthCritical Health = Health{"Critical", 0}
	// HealthOK indicates that no conditions require special attention.
	HealthOK Health = Health{"OK", 1}
	// HealthWarning indicates that a condition requires attention.
	HealthWarning Health = Health{"Warning", 2}
)

// State is the known state of the resource, which is encoded as enumeration and based on Redfish.
type State Enumeration

var (
	// StateAbsent indicates that this function or resource is either not present or detected.
	StateAbsent State = State{"Absent", 0}
	// StateDeferring indicates that the element does not process any commands but queues new requests.
	StateDeferring State = State{"Deferring", 1}
	// StateDisabled indicates that this function or resource is disabled.
	StateDisabled State = State{"Disabled", 2}
	// StateEnabled indicates that this function or resource is enabled.
	StateEnabled State = State{"Enabled", 3}
	// StateInTest indicates that this function or resource is undergoing testing, or is in the process of capturing information for debugging.
	StateInTest State = State{"InTest", 4}
	// StateQualified indicates that the element quality is within the acceptable range of operation.
	StateQualified State = State{"Qualified", 5}
	// StateQuiesced indicates that the element is enabled but only processes a restricted set of commands.
	StateQuiesced State = State{"Quiesced", 6}
	// StateStandbyOffline indicates that this function or resource is enabled but awaits an external action to activate it.
	StateStandbyOffline State = State{"StandbyOffline", 7}
	// StateStandbySpare indicates that this function or resource is part of a redundancy set and awaits a failover or other external action to activate it.
	StateStandbySpare State = State{"StandbySpare", 8}
	// StateStarting indicates that this function or resource is starting.
	StateStarting State = State{"Starting", 9}
	// StateUnavailableOffline indicates that this function or resource is present but cannot be used.
	StateUnavailableOffline State = State{"UnavailableOffline", 10}
	// StateUpdating indicates that the element is updating and might be unavailable or degraded.
	StateUpdating State = State{"Updating", 11}
)

// ManagerType is the type of manager that this resource represents, which is encoded as enumeration and based on Redfish.
type ManagerType Enumeration

var (
	// ManagerTypeAuxiliaryController is a controller that provides management functions for a particular subsystem or group of devices.
	ManagerTypeAuxiliaryController ManagerType = ManagerType{"AuxiliaryController", 0}
	// ManagerTypeBMC is a controller that provides management functions for a single computer system.
	ManagerTypeBMC ManagerType = ManagerType{"BMC", 1}
	// ManagerTypeEnclosureManager is a controller that provides management functions for a chassis or group of devices or systems.
	ManagerTypeEnclosureManager ManagerType = ManagerType{"EnclosureManager", 2}
	// ManagerTypeManagementController is a controller that primarily monitors or manages the operation of a device or system.
	ManagerTypeManagementController ManagerType = ManagerType{"ManagementController", 3}
	// ManagerTypeRackManager is a controller that provides management functions for a whole or part of a rack.
	ManagerTypeRackManager ManagerType = ManagerType{"RackManager", 4}
	// ManagerTypeService is software-based service that provides management functions.
	ManagerTypeService ManagerType = ManagerType{"Service", 5}
)

var managerTypeSlice = []*ManagerType{
	&ManagerTypeAuxiliaryController,
	&ManagerTypeBMC,
	&ManagerTypeEnclosureManager,
	&ManagerTypeManagementController,
	&ManagerTypeRackManager,
	&ManagerTypeService,
}

var managerTypeMap = map[string]*ManagerType{
	ManagerTypeAuxiliaryController.JSON:  &ManagerTypeAuxiliaryController,
	ManagerTypeBMC.JSON:                  &ManagerTypeBMC,
	ManagerTypeEnclosureManager.JSON:     &ManagerTypeEnclosureManager,
	ManagerTypeManagementController.JSON: &ManagerTypeManagementController,
	ManagerTypeRackManager.JSON:          &ManagerTypeRackManager,
	ManagerTypeService.JSON:              &ManagerTypeService,
}

func NewManagerType(value interface{}) ManagerType {
	stringValue, ok := value.(string)
	if ok {
		return *managerTypeMap[stringValue]
	}
	intValue, ok := value.(int)
	if ok {
		return *managerTypeSlice[intValue]
	}

	log.Error().Msgf("❌ NewManagerType: failed to parse: invalid value: %v", value)

	return ManagerTypeAuxiliaryController
}
