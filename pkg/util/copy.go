package util

import (
	"io"
	"os"
)

// Copy copies an input file (src) to an output
// file (dst). The specified destination directory
// must exist before attempting to copy the file.
func Copy(src string, dst string) error {
	in, err := os.Open(src)
	if err != nil {
		return err
	}
	defer in.Close()

	out, err := os.Create(dst)
	if err != nil {
		return err
	}
	defer out.Close()

	_, err = io.Copy(out, in)
	if err != nil {
		return err
	}
	return out.Close()
}
