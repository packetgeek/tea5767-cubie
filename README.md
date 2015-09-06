# fmradio
Control software for the TEA5767 and RDA5807SP in TEA5767 mode, on the CubieTruck.  Ported from the Raspberry Pi, with the wiringPi library dependency removed.

Controls include:

- mute.c - mutes the audio output
- searchdn.c - searches for the next station downward (currently doesn't work on TEA5767)
- searchup.c - searches for the next station upward (currently doesn't work on TEA5767)
- status.c - reports on frequency and if stereo is being rec'd
- tune.c - tunes the radio to a specific frequency (e.g., 98.7)
- unmute.c - unmutes the audio output

Read the source for each to determine how to compile them.

