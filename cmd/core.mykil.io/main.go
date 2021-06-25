package main

import (
	"github.com/nicklasfrahm/mykilio/pkg/controller"
	"github.com/nicklasfrahm/mykilio/pkg/core"
)

var (
	version  = "dev"
	apiGroup = "test.mykil.io"
)

func main() {
	// Create new controller for the specified apiGroup.
	controller.New(&controller.Metadata{
		APIGroup: apiGroup,
		Version:  version,
	}).ConnectDB().Install(core.V1Alpha1).Start()
}
