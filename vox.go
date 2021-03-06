package vox

// #define SUNVOX_MAIN
// #cgo linux LDFLAGS: -lstdc++ -ldl -lm
// #cgo darwin LDFLAGS: -lstdc++ -ldl -lm
// #include <stdio.h>
// #include <stdlib.h>
// #ifndef _WIN32
// #include <dlfcn.h>
// #endif
// #ifdef _WIN32
// #include <windows.h>
// #endif
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

    // Note constants
    NOTE_OFF = C.NOTECMD_NOTE_OFF
    ALL_NOTES_OFF = C.NOTECMD_ALL_NOTES_OFF
    CLEAN_SYNTHS = C.NOTECMD_CLEAN_SYNTHS
    STOP = C.NOTECMD_STOP
    PLAY = C.NOTECMD_PLAY

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
func Quit() {
    C.vox_deinit()
    C.sv_unload_dll()
}

// SampleType returns the internal sample type of the sunvox engine.
func SampleType() int {
    return int(C.vox_get_sample_type())
}

// Ticks returns the current tick counter (from 0 to 0xFFFFFFFF).
func Ticks() uint {
    return uint(C.vox_get_ticks())
}

// TicksPerSecond returns the number of sunvox ticks per second.
func TicksPerSecond() uint {
    return uint(C.vox_get_ticks_per_second())
}

// Module synths
type Module struct {
    num C.int
    song C.int
    name string
    red, green, blue byte
    x, y int
}

func (m *Module) Name() string {
    return m.name
}

func (m *Module) Color() (byte, byte, byte) {
    return m.red, m.green, m.blue
}

func (m *Module) Position() (int, int) {
    return m.x, m.y
}

// Trigger plays a note on a channel in a module.
// channel: 0..7
// note: 0..127 - note; 128 - note off; 129, 130.. - note command
// vel: 1..129
// ctl: CCXX; CC - controller number; X - std effect
// val: controller value
func (m *Module) Trigger(channel, note, vel, ctl, val int) {
    C.vox_send_event(m.song, C.int(channel), C.int(note), C.int(vel), m.num, C.int(ctl), C.int(val))
}

// Song is used to load and play sunvox songs.
type Song struct {
    slot C.int
    name string
    volume, bpm, tpl, frames, lines int
    Mod []*Module
}

// Open creates a new slot and laods a sunvox song into it.
func Open(path string) (*Song, error) {
    slot := C.int(slots)
    if C.vox_open_slot(slot) != C.int(0) {
        return nil, errors.New("Could not open new slot")
    }

    name := C.CString(path)
    defer C.free(unsafe.Pointer(name))
    if C.vox_load(slot, name) != C.int(0) {
        return nil, errors.New(fmt.Sprintf("Could not open song %s", path))
    }

    slots++
    num_mods := int(C.vox_get_number_of_modules(slot))
    modules := make([]*Module, num_mods)
    for i := 0; i < num_mods; i++ {
        mod := new(Module)
        mod.num = C.int(i)
        mod.song = slot
        mod.name = C.GoString(C.vox_get_module_name(slot, C.int(i)))
        color := C.vox_get_module_color(slot, C.int(i))
        mod.red = byte(color & 255)
        mod.blue = byte((color >> 8) & 255)
        mod.green = byte((color >> 16) & 255)
        xy := C.vox_get_module_xy(slot, C.int(i))
        mod.x = int(xy >> 16)
        mod.y = int(xy & 0xffff)
        modules[i] = mod
    }
    song := &Song{slot, C.GoString(C.vox_get_song_name(slot)), 100, int(C.vox_get_song_bpm(slot)), int(C.vox_get_song_tpl(slot)), int(C.vox_get_song_length_frames(slot)), int(C.vox_get_song_length_lines(slot)), modules}
    song.SetVolume(song.volume)
    return song, nil
}

// Close the song. The song should not be used after calling this.
func (s *Song) Close() {
    C.vox_close_slot(s.slot)
}

// Volume returns the volume of the song.
func (s *Song) Volume() int {
    return s.volume
}

// SetVolume sets the volume of the song.
func (s *Song) SetVolume(vol int) {
    C.vox_volume(s.slot, C.int(vol))
    s.volume = vol
}

// Play starts playback from where ever the song was stopped.
func (s *Song) Play() error {
    if C.vox_play(s.slot) != C.int(0) {
        return errors.New(fmt.Sprintf("Could not play slot %v", s))
    }
    return nil
}

// Replay starts playback from the beginning.
func (s *Song) Replay() {
    C.vox_play_from_beginning(s.slot)
}

// Pause stops the song's playback at its current position.
func (s *Song) Pause() {
    C.vox_stop(s.slot);
}

// Finished indicates if the song has gotten to the end.
func (s *Song) Finished() bool {
    ended := C.vox_end_of_song(s.slot)
    if ended == 0 {
        return false
    }
    return true;
}

// Line returns the current line in the song.
func (s *Song) Line() int {
    return int(C.vox_get_current_line(s.slot))
}

// SetLooping enables or disables looping.
func (s *Song) SetLooping(loop bool) {
    if loop {
        C.vox_set_autostop(s.slot, C.int(0));
    } else {
        C.vox_set_autostop(s.slot, C.int(1));
    }
}

// Seek sets the offset of the song in lines, interpreted according to whence: // 0 means relative to the start of the song, 1 means relative to the current // line, and 2 means relative to the end.
func (s *Song) Seek(offset, whence int) {
    switch whence {
    case 1:
        C.vox_rewind(s.slot, C.int(s.Line() + offset))
    case 2:
        C.vox_rewind(s.slot, C.int(s.Lines() + offset))
    default:
        C.vox_rewind(s.slot, C.int(offset))
    }
}

// Name returns the name of the song.
func (s *Song) Name() string {
    return s.name
}

// Level returns the current signal level of a channel.
func (s *Song) Level(channel int) int {
    return int(C.vox_get_current_signal_level(s.slot, C.int(channel)))
}

// BeatsPerMinute returns the songs beats per minute.
func (s *Song) BeatsPerMinute() int {
    return s.bpm
}

// TicksPerLine returns the number of ticks per line.
func (s *Song) TicksPerLine() int {
    return s.tpl
}

// Frames gives the length of the song in frames.
func (s *Song) Frames() int {
    return s.frames
}

// Lines gives the length of the song in lines.
func (s *Song) Lines() int {
    return s.lines
}
