#include "MEM.h"

#define SIZE_KB     1024
#define RAM_SIZE    8

gb_short _gb_ram[RAM_SIZE*SIZE_KB];
gb_short _gb_rom[ROM_SIZE*SIZE_KB]; //TODO: Needs to be changed if we're to be using banks.
