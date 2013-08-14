pseudochanger
=============
A project that aims to add an auxiliary input to head units that use the Panasonic variant of the Alpine MBus protocol.

Alpine MBus is a bidirectional one-wire serial interface used for communication between audio head units and peripherals like CD players, CD changers, and tape decks. A period for each bit is 3 ms, and low/high bits are indicated by how long the line is held low. Low for the first 600us indicates a 1, low for 1800 us indicates a 0. Data is a multiple of four bits, and a four-bit check follows the packet, an XOR of all the other 4-bit chunks.

The Panasonic variant uses different ping and ACK codes, sends some data at different times, and does what looks like type detection by sending out a cycle of codes.

This project was developed specifically for the 2002 Subaru WRX head unit, model CQ-EF7260A, and should work with other Panasonic head units that have the same 16-pin connector on the back, as also found in some Mazdas and Hondas.

This project contains several different packages:
 - Arduino library
   - An interrupt-driven approach to receiving and sending data on the MBus
 - A simple changer emulator
   - Does not support any controls and only displays a static timecode 
   - Essentially plug-and-play, aside from a little circuitry
 - An advanced changer emulator
   - Supports playback controls and an autodetect interface for iPods