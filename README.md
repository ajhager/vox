vox Play and manipulate [SunVox](http://www.warmplace.ru/soft/sunvox/) songs in go
===

Notes
-----
* WINDOWS: There isn't a 64bit library for libsunvox yet, so you must use mingw32 and GOARCH=386.
* You'll have to copy the correct dynamic library for your platform from data into the folder with your program.

Install
-------
`go get github.com/ajhager/vox`

Try it!
-------
```go
package main

import (
    "github.com/ajhager/vox"
)

func main() {
    vox.Init("", 44100, 2, 0)
    defer vox.Quit()
    song, _ := vox.Open("test.sunvox")
    defer song.Close()
    song.Play()
    for {}
}
```
