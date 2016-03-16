# Gameboy Emulator (we have yet to come up with a cooler name for this!)

## Responsibilities
Eni -- Work on the interface and interrupts of the given controller. After that, implementation of the GPU.

Neel -- Work on reading the ROM cartridge and interfacing it with the RPI. Following that would be inclusion of banks for longer games (e.g. Pokemon).

Guille -- Work on the CPU emulation and opcodes. Afterwards, work on the implementation of the GPU emulation.

## Goal of the project
To make a working GameBoy emulator completely on bare-metal RPI. In particular, we want to get a game like Tetris and, as our reach, a game like
Pokemon to work by reading directly from the gameboy ROM (interfacing with the hardware).

## Milestone for T-1 week
Finishing up the first part for each person's work (controller/reading simple ROM/CPU emulation). Beginning major work on the GPU emulation.

## Resources needed
- Header for the pins.
- Pokemon cartridge
- Tetris cartridge
- SNES gamepad

## Notes
The current state of the repository can be found under [guillean's public repo](https://github.com/guillean/CS107E-GB-Emulator).
