package main

import (
    "hagerbot.com/vox"
)

func main() {
    err := vox.Init("", 44100, 2, 0)
    if err != nil {
        panic(err)
    }

    println(vox.Version)

    slot, err := vox.Open("../data/songs/test.sunvox")
    if err != nil {
        panic(err)
    }

    slot.SetVolume(256)

    slot.Play()

    for {}

    slot.Close()
    vox.Quit()
}