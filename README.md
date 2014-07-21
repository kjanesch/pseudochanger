pseudochanger
=============
A project that aims to add an auxiliary input to head units that use the Panasonic variant of the Alpine MBus protocol.

Alpine MBus is a bidirectional one-wire serial interface used for communication between audio head units and peripherals like CD players, CD changers, and tape decks. A period for each bit is 3 ms, and low/high bits are indicated by how long the line is held low. Low for the first 600us indicates a 1, low for 1800 us indicates a 0. Data is a multiple of four bits, and a four-bit check follows the packet, an XOR of all the other 4-bit chunks.

The Panasonic variant uses different ping and ACK codes, sends some data at different times, and does what looks like type detection by sending out a cycle of codes. The included library reflects these differences.

This project was developed specifically for the 2002 Subaru WRX head unit, model CQ-EF7260A, and should work with other Panasonic head units that have the same 16-pin connector on the back, as also found in some Mazdas and Hondas.

This project contains:
 - Arduino MBusPanasonic library
   - Timing-driven bus interface
   - Sends static timestamp of Disc 1, Track 1, Time A:0A to indicate aux input 
 - Example implementation (pseudochanger)
   - Uses the Teensy branch of [finsprings/arduinaap](https://github.com/finsprings/arduinaap/tree/teensy) for the iPod Accessory Protocol interface.
 - Circuit and PCB diagrams
   - V1 is the handmade, free-handed version
   - V2 is the (incomplete) PCB of V1 with minor tweaks
   - V2.5 is a completely respun, integrated design
     - Built-in Atmega 32U4 (or 16U4 depending on code space needs)
     - Onboard switching regulator to generate 5V supply from +12V and accessory on/off
     - Smaller (0805/0603) SMT components
     - Ethernet jack for iPod connector
     - Op-amp based switch to make it solid state

TODO: 
 - Library:
   - Interrupt-driven approach to receiving and sending MBus data
   - Integrated polling routine to extract button presses
   - Fix issues with generating timestamps
   - Add a short example that doesn't keep state, switch between inputs, or control iPods (fewer I/Os, less setup, smaller code, no dependencies)
 - Implementation: 
   - Sort out why iPod interface doesn't work (Hardware? Software?)
 - EAGLE files:
   - Finish V2.5 PCB and BOM, construct boards IRL and test
