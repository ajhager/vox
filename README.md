vox Play and manipulate [SunVox](http://www.warmplace.ru/soft/sunvox/) songs in go
===
[Documentation](http://hagerbot.com/vox/docs.html "Documentation") &nbsp;

![Vox Screenshot](http://hagerbot.com/img/project_vox.png)

Notes
-----
* WINDOWS: There isn't a 64bit library for libsunvox yet, so you must use mingw32 and GOARCH=386.
* You'll have to copy the correct dynamic library for your platform from data into the folder with your program.

Install
-------
`go get hagerbot.com/vox`

Try it!
-------
```go
package main

import (
    "hagerbot.com/vox"
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
