package vox

// #cgo LDFLAGS: -lstdc++ -ldl -lm
// #define SUNVOX_MAIN
// #include <stdio.h>
// #include <stdlib.h>
// #include <dlfcn.h>
// #include "vox.h"
import "C"

import (
    "errors"
    "fmt"
    "unsafe"
    "runtime"
)

func init() {
    runtime.LockOSThread()
}

const (
    // Init flags
    NO_DEBUG_OUTPUT = C.SV_INIT_FLAG_NO_DEBUG_OUTPUT
    USER_AUDIO_CALLBACK = C.SV_INIT_FLAG_USER_AUDIO_CALLBACK
    AUDIO_INT16 = C.SV_INIT_FLAG_AUDIO_INT16
    AUDIO_FLOAT32 = C.SV_INIT_FLAG_AUDIO_FLOAT32
    ONE_THREAD = C.SV_INIT_FLAG_ONE_THREAD

    // Module flags
    FLAG_EXISTS = C.SV_MODULE_FLAG_EXISTS
    FLAG_EFFECT = C.SV_MODULE_FLAG_EFFECT
    INPUTS_OFF = C.SV_MODULE_INPUTS_OFF
    INPUTS_MASK = C.SV_MODULE_INPUTS_MASK
    OUTPUTS_OFF = C.SV_MODULE_OUTPUTS_OFF
    OUTPUTS_MASK = C.SV_MODULE_OUTPUTS_MASK

    // Type flags
    INT16 = C.SV_STYPE_INT16
    INT32 = C.SV_STYPE_INT32
    FLOAT32 = C.SV_STYPE_FLOAT32
    FLOAT64 = C.SV_STYPE_FLOAT64
)

var (
    Version string
    slots = 0
)

// Init loads the sunvox dll and initializes the library.
func Init(dev string, freq, channels, flags int) error {
    if C.sv_load_dll() != C.int(0) {
        return errors.New("Could not load sunvox library")
    }

    device := C.CString(dev)
    defer C.free(unsafe.Pointer(device))

    ver := int(C.vox_init(device, C.int(freq), C.int(channels), C.int(flags)))
    if ver < 0 {
        return errors.New("Could not initialize sunvox library")
    }
    Version = fmt.Sprintf("%d.%d.%d", (ver>>16)&255, (ver>>8)&255, ver&255) 

    return nil
}

// Quit deinitializes the library and unloads the sunvox dll.
func Quit() error {
    if C.vox_deinit() != C.int(0) {
        return errors.New("Problem uninitializing sunvox library")
    }

    if C.sv_load_dll() != C.int(0) {
        return errors.New("Problem unloading sunvox library")
    }

    return nil
}

// SampleType returns the internal sample type of the sunvox engine.
func SampleType() int {
    return int(C.vox_get_sample_type())
}

// Slot is used to load and play sunvox songs.
type Slot int

// Open creates a new slot and laods a sunvox song into it.
func Open(path string) (Slot, error) {
    slot := slots
    if C.vox_open_slot(C.int(slot)) != C.int(0) {
        return -1, errors.New("Could not open new slot")
    }

    name := C.CString(path)
    defer C.free(unsafe.Pointer(name))
    if C.vox_load(C.int(slot), name) != C.int(0) {
        return -1, errors.New(fmt.Sprintf("Could not open song %s", path))
    }

    slots++
    return Slot(slot), nil
}

// Close closes the slot. The slot should no longer be used after calling it.
func (s Slot) Close() error {
    if C.vox_close_slot(C.int(s)) != C.int(0) {
        return errors.New(fmt.Sprintf("Problem closing slot %v", s))
    }
    return nil
}

// SetVolume sets the volume of the slot.
func (s Slot) SetVolume(vol int) error {
    if C.vox_volume(C.int(s), C.int(vol)) != C.int(0) {
        return errors.New(fmt.Sprintf("Could not change slot %v's volume to %v", s, vol))
    }
    return nil
}

// Play starts playback from where ever the song was stopped.
func (s Slot) Play() error {
    if C.vox_play(C.int(s)) != C.int(0) {
        return errors.New(fmt.Sprintf("Could not play slot %v", s))
    }
    return nil
}

// Line returns the current line in the song.
func (s Slot) Line() int {
    return int(C.vox_get_current_line(C.int(s)))
}
