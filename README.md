# seq32 README


![screenshot](https://raw.github.com/Stazed/seq32/wip/icons/seq32-2.0.0.png "Seq32 release-2.0.0")


# What is seq32?

This version is a fork of Stazed's [seq32](https://github.com/Stazed/seq32.git), which is a fork of [seq24](https://launchpad.net/seq24) (which is a fork of the [original seq24](http://filter24.org/seq24/)).

This fork was made so I can make seq32 more user-friendly for my own use - which tends to be as the driver for midi instruments that I play guitar and sing over. 

*see the end of this page for a list of changes to Stazed's version 2.0.0*

# Install

The dependencies are:

*   cmake
*   Gtkmm-3.0
*   Jackd
*   libasound2
*   liblo   (NSM support)
*   librt

Additional dependencies required by Gtkmm-3
*   sigc++-2.0
*   glibmm-2.4
*   cairomm-1.0
*   pangomm-1.4
*   atkmm-1.6

For compiling you will also need the development packages for the above dependencies.

# What to do with a fresh repository checkout?

## To make Using CMAKE:
```bash
    mkdir build
    cd build
    cmake ..
    make
    make install (as root)
```

## To run

the executable sits in `build/src/seq32`


## To remove:
```bash
    make uninstall (as root)
```
## SAST Tools

[PVS-Studio](https://pvs-studio.com/en/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.

# Changes to Stazed's version 2.0.0

- **tempo change in song mode:** You can now change tempo by adjusting the value in the main window even when in song mode. This is useful because it means you don't have to stop the song, adjust the tempo, and then start it again, which works against you trying to get the tempo to feel right while playing. NB there is incomplete funcionality to have songs that change tempo via a "tempo map" - this is still unfinished! NB, to save the new tempo, you should just save the midi file (CTRL+s) as ususal. 
- **note length changing:** The resolution of modern laptops is so great now that it's quite tricky to click on the tail end of a note to change its length. Now, with mouse anywhere over a note, if you hold down the ALT key, you'll see the mouse pointer change from `CENTER_PTR` to `LEFT_PTR`, and you can click-and-drag to change the note length. NB This is only implemented for the "Fruity" mouse interaction mode (in `Tools->Options->Mouse`)
- **pentatonic scales:** Added to the available scales
- **Scale lock: limit note entry to current scale:** a checkbox next to the 
- **Song blocks** *Goal* to have 'loopable' regions in songs that can be entered/left/rewound using MIDI.
    - **Play head placement:** CTRL+click places the cursor (red line) anywhere in a song
    - **Mark current position** press 'M' and the position in the song will be marked - it will appear as a yellow line when the red line is not over it! 
    

# Current issues

- **song blocks todo:**
    - **drawing issues** - looks like 'm' is marking positions, but the last one isn't drawn when play/spacebar is pressed. It doesn't appear until the whole window is refreshed
    - **stop transmitting midi when moving the transport** At the moment this happens if you press the L->R button but with the extra functionality there will be other times..
    - **option to not scroll after the end of the song** - not vital, but handy for editing perhaps, and would save a keystroke to move on to the next song when playing live
- **midi control of pattern groups** The idea is that a footswitch can send midi commands to move between pattern blocks in song mode.
- **ncurses version:** Super-efficient implementation for rasp-pi or somesuch - one day! 



