package main

import (
	"github.com/ajhager/vox"
	"time"
)

func main() {
	err := vox.Init("", 44100, 2, 0)
	if err != nil {
		panic(err)
	}
	defer vox.Quit()

	println(vox.Version)

	song, err := vox.Open("../data/songs/test.sunvox")
	if err != nil {
		panic(err)
	}
	defer song.Close()

	println(song.Name())

	song.SetVolume(256)

	song.Mod[7].Trigger(0, 64, 128, 0, 0)
	time.Sleep(1 * time.Second)
	song.Mod[7].Trigger(0, 64, 128, 0, 0)
	time.Sleep(1 * time.Second)

	song.Play()

	for !song.Finished() {
	}
}
