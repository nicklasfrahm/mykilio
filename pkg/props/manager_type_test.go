package props

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNewManagerType(t *testing.T) {
	isIt := assert.New(t)

	isIt.Equal(ManagerTypeAuxiliaryController.JSON, NewManagerType(0).JSON)
	isIt.Equal(ManagerTypeBMC.JSON, NewManagerType(uint8(1)).JSON)
	isIt.Equal(ManagerTypeEnclosureManager.JSON, NewManagerType(uint16(2)).JSON)
	isIt.Equal(ManagerTypeManagementController.JSON, NewManagerType(int32(3)).JSON)
	isIt.Equal(ManagerTypeRackManager.JSON, NewManagerType(int64(4)).JSON)
	isIt.Equal(ManagerTypeService.JSON, NewManagerType(int(5)).JSON)
}
