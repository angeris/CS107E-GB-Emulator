#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define SIZE_KB     1024
#define RAM_SIZE    8
#define ROM_SIZE    8

typedef uint16_t gb_long;
typedef uint8_t gb_short;

gb_short _gb_ram[RAM_SIZE*SIZE_KB];
gb_short _gb_rom[ROM_SIZE*SIZE_KB]; //TODO: Needs to be changed if we're to be using banks.

#endif
