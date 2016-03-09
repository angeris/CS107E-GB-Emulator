#ifndef GPU_H
#define GPU_H

#include "MEM.h"

#define LCD_ENABLE      1<<7
#define WIN_TILE_SELECT 1<<6
#define WIN_DISP_ENABLE 1<<5
#define WIN_BG_TILEDATA 1<<4
#define BG_TILE_MAP_SEL 1<<3
#define OBJ_SPRITE_SIZE 1<<2
#define OBJ_SPRITE_ENAB 1<<1
#define BG_DISPLAY_ENAB 1<<0

#define MODE_HBLANK 0
#define MODE_VBLANK 1
#define MODE_READOM 2
#define MODE_READAL 3

extern gb_short vram[VRAM_SIZE];

gb_short gpu_read(gb_long addr); 

#endif
