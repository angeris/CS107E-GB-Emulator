#ifndef GPU_H
#define GPU_H

#include "MEM.h"

/* Graphics Library */
// define colors
#define WIN_WIDTH 160
#define WIN_HEIGHT 144

/* Primarily Drawn From - http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html */

/* LCD Control Register */
#define LCD_CONTROL_REG 0xFF40 // check 

#define LCD_ENABLE      1<<7
#define WIN_TILE_SELECT 1<<6
#define WIN_DISP_ENABLE 1<<5
#define WIN_BG_TILEDATA 1<<4
#define BG_TILE_MAP_SEL 1<<3
#define OBJ_SPRITE_SIZE 1<<2
#define OBJ_SPRITE_ENAB 1<<1
#define BG_DISPLAY_ENAB 1<<0

/* Sprite Attributes */

/* GPU Modes */
#define MODE_HBLANK 0
#define MODE_VBLANK 1
#define MODE_READOM 2
#define MODE_READAL 3

extern gb_short vram[VRAM_SIZE];

gb_short gpu_read(gb_long addr); 
void gpu_exec();
void gpu_writeline();
void gpu_drawscreen();

void gpu_init();

void draw_tile();
void draw_sprite();

#endif
