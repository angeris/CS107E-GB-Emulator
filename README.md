# CS107E-GB-Emulator
The final project for the Stanford CS107e class. Gameboy emulator running on bare-metal RPI.

### CPU
------
The nice part about the GameBoy in general is that it has no BIOS. In paritcular, the entirety of a given game is run directly from op-codes found in the game's ROM, which makes it an interesting platform that requires no propietary software to emulate.

The CPU has an 8-bit instruction set, for a total of 256 possible OP-codes (of which 245 are actually used). The general opcode table, on which all of the opcodes in the current emulator were based on, can be found [here](http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html), and the explanation for the opcodes was found in the PanDocs (referenced below, but [here](http://bgb.bircd.org/pandocs.htm) for ease-of-access).

### Registers
------
All of the necessary documentation on the registers and their behaviour was found in the [PanDocs](http://bgb.bircd.org/pandocs.htm). The GameBoy makes use of several sets of registers to perform tasks like ROM banking (i.e. reading a ROM which has a total storage space > 64kb, which is the address space given by the 16-bit addresses), interrupt enabling and reading, display, tile, and sprite drawing, among several other things. The primary resource was PanDocs for most of the documentation, though code from [Omar's Implementation](https://github.com/osnr/gambo/blob/master/core/src/main/java/com/gambo/core/Mmu.java) was used as reference in case of any ambiguities.

### GPU
------
The GPU primarily reads data from the Virtual RAM area of memory (Addresses 0x8000 to 0x9FFF). A more detailed breakdown can be found in [GPU.h](https://github.com/cs107e/eniasebiomo-guillean-neelbedekar-project/blob/master/cpu/GPU.h). The GB puts out graphics on a 144x160 pixel screen. The graphics are drawn line by line, and access data from 'tilesets' stored in VRAM. These tilesets are 8x8 or 8x16 tiles. Tiles can contain background patterns or sprite patterns. They are systematically accessed from memory and drawn to the screen based on input from the CPU defining where in the game a player may be, and what needs to be drawn to the screen at any point in time. The GB displays in four colors (White, Black, Light Gray and Dark Gray) which are defined by four 2-bit configurations (00, 01, 10, 11). Further comments and implementation details can be found in [GPU.c](https://github.com/cs107e/eniasebiomo-guillean-neelbedekar-project/blob/master/cpu/GPU.c).
+ Makefile commands - `make gpu` and `make install-gpu`
	* Compiles to main-gpu.bin

### Interrupts
------
Although not included in the final design, one way to emulate the system interrupts that the gameboy experiences at approximately 60MHz is by scheduling timer interrupts. The interrupt uses the System Timer and BCM2835 Interrupt Controller, to enable interrupts, and dynamically schedule them as needed. This implementation allows interrupts to be scheduled every 60 milliseconds by taking the time from the System Clock, adding 60000 microseconds, and placing that new value in the System Timer Compare Register. An interrupt is called when the value written into this control register is equal to the System Timer. More documentation and comments found in [interrupt.c](https://github.com/cs107e/eniasebiomo-guillean-neelbedekar-project/blob/master/controller/interrupt.c)

### Controller
------
![SNES Controller](http://ecx.images-amazon.com/images/I/41aqbyIKdcL.jpg)

Although not included in the final design, the classic SNES controller operates as a shift register - which essentially means that the system must send a sort of "request" to the controller to get the state of the buttons. The implementation is somewhat similar to the PS2 input we programmed in assignment 6. The controller has 7 pins - 5 of which are used. +5v, Ground, Data1, Latch and Clock. To get the button states, first a high to low pulse must be sent to the LATCH pin, then a single bit can be read to be high or low from the DATA pin. A pulse is then sent to the CLOCK pin, and another bit is read. Clock pulses are sent and Data is read until all 12 pin states have been received. The states are stored in a struct of unsigned int bitfields, which allow all 12 states to be sent in an object the size of an int.
+ Makefile commands - `make controller` and `make install-controller`
	* Compiles to main-controller.bin

## Attributions
------
### Primary Source - [GB Pan Docs](http://bgb.bircd.org/pandocs.htm)
+ GPU Implementation found in GPU.c closely follows [this tutorial](http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html) @ [codeslinger.co.uk](codeslinger.co.uk)
	* [Imrannazar GB Emulation Tutorial](http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics)
+ Controller implementation found in controller.h and SNESPad.h based on an Arduino implementation found in [this git repo](https://github.com/TacticalCode/SNESpaduino) from [this blog](http://en.tacticalcode.de/2013/06/snespaduino-super-nintendo-gamepad-for-arduino.html) 	
+ Interrupt implementation based on
	* [ARM Docs](https://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)
	* [BCM2835 Docs](http://embedded-xinu.readthedocs.org/en/latest/arm/rpi/BCM2835-Interrupt-Controller.html)
	* [Random Forum Posts like this one](https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=64008) that contained the **actually correct** implementation unlike the actual documentation *... who would've thought*
    * [Omar's Implementation](https://github.com/osnr/gambo/blob/master/core/src/main/java/com/gambo/core/Mmu.java) which was used sometimes as a reference when there was some ambiguity in the PanDocs.
