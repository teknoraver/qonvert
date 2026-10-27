# Qonvert

Qonvert converts videos and DVDs to 3GP, to watch them on the mobile phones of 2007:
176×144 MPEG-4 video at 12.5 fps, encoded in two passes, with normalized mono AMR audio.
The *Cartoon* option tunes the video encoder for animations.

<p align="center"><img src="https://github.com/user-attachments/assets/d3b10266-ac58-4f0d-ab4d-52c360665325" alt="Qonvert main window" width="412"></p>

It's kept for historical reference: the conversion is done by external tools which are hard to find nowadays,
`mplayer`, `mencoder`, `normalize-audio`, `amrenc` and `MP4Box`, which must be in the `PATH`.

## Build

Requirements: CMake 3.19 or newer and Qt 6 (Widgets).

```
cmake -B build
cmake --build build
```

## License

Copyright (C) 2007 Matteo Croce

Qonvert is released under the GNU General Public License version 3 or later, see [COPYING](COPYING).
