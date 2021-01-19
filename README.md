# Additive
Additive oscillator for KORG NTS-1


## What's this
Simple additive oscillator for KORG NTS-1.


## Prerequisite
[logue-sdk](https://github.com/korginc/logue-sdk)

If you use pre-built binary, logue-sdk is not required. Simply transfer [Additive.ntkdigunit](https://github.com/kachine/nts1Additive/raw/main/Additive.ntkdigunit) to your NTS-1 using NTS-1 digital Librarian software.


## How to build
You have to change PLATFORMDIR variable in Makefile, it should correspond to where you installed logue-sdk (something like below).
```Makefile
PLATFORMDIR = ${PATHTO}/logue-sdk/platform/nutekt-digital
```

Then, simply type "make" to build this project.
```sh
$ make
```


## How to use
There are 7 paramters for this oscillator:
- Overtone 2 level

This parameter controls the level of overtone 2.
Its frequency is fixed to 2x of root tone.
Choose "OT2" by pressing OSC and tweaking TYPE knob, then input by B knob.

- Overtone 3 level

This parameter controls the level of overtone 3.
Its frequency is fixed to 3x of root tone.
Choose "OT3" by pressing OSC and tweaking TYPE knob, then input by B knob.

- Overtone 4 level

This parameter controls the level of overtone 4.
Its frequency is fixed to 4x of root tone.
Choose "OT4" by pressing OSC and tweaking TYPE knob, then input by B knob.

- Overtone 5 level

This parameter controls the level of overtone 5.
Its frequency is fixed to 5x of root tone.
Choose "OT5" by pressing OSC and tweaking TYPE knob, then input by B knob.

- Overtone 6 level

This parameter controls the level of overtone 6.
Its frequency is fixed to 6x of root tone.
Choose "OT6" by pressing OSC and tweaking TYPE knob, then input by B knob.

- Overtone 7 level

This parameter controls the level of overtone 7.
Its frequency is fixed to 7x of root tone.
Choose "OT7" by pressing OSC and tweaking TYPE knob, then input by B knob.

- LFO destination

This parameter switches the LFO destination.
Mapped to OSC mode A knob(SHPE) and MIDI CC#54
The parameter range is divided into four. So, LFO destination change will be take effect by quarter of A knob motion, or MIDI CC#54 value by each 32.
|LFO destination|value|
|---|---|
|None|first quarter|
|Odd overtones|second quarter|
|Even overtones|third quarter|
|All overtones|fourth quarter|

If you use NTS-1's built-in LFO, S(shape) is effective for level of overtone modulation. Its target overtone is selectable via LFO destination parameter. Ofcourse, P(pitch) is also effective for pitch modulation.


---
The names of products or companies are the trademarks or registered trademarks of their respective companies. These companies have no relation to this project.
