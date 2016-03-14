#ifndef GPU_H
#define GPU_H

#include "MEM.h"
#include "gl.h"

/**
 *                  Layout of Tile Sets in memory
 *   http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics
 * ------------------------------------------------------------------------
 *  Region	         Usage
 * =========    ========================
 * 8000-87FF	Tile set #1: tiles 0-127
 * 8800-8FFF	Tile set #1: tiles 128-255 & Tile set #0: tiles -1 to -128
 * 9000-97FF	Tile set #0: tiles 0-127
 * 9800-9BFF	Tile map (Background) #0
 * 9C00-9FFF	Tile map (Background) #1
 * ----------------------------------------
 * FE00-FE9F   Sprite Attribute Table (OAM)
 */

#define TILE_SET_1U   0x8000 /* Unsigned Tile Region */
#define TILE_SET_1S   0x8800 /* Signed Tile Region */
#define TILE_SET_BG_0 0x9800
#define TILE_SET_BG_1 0x9C00

/* Graphics Library */
#define WHITE 0xFFFFFFFF
#define LGRAY 0xFFCCCCCC
#define DGRAY 0xFF777777
#define BLACK 0xFF000000

#define WIN_WIDTH  160
#define WIN_HEIGHT 144

/* Primarily Drawn From - http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html */

/* Scroll and Window Positions */
#define SCROLLY 0xFF42
#define SCROLLX 0xFF43
#define WINDOWY 0xFF4A
#define WINDOWX 0xFF4B
#define LCDY    0XFF44

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

/**
 *          Palettes 
 *  http://bgb.bircd.org/pandocs.htm#videodisplay
 * -------------------------------------------------
 * Bit 1-0 - Shade for Color 0
 * Bit 3-2 - Shade for Color 1
 * Bit 5-4 - Shade for Color 2
 * Bit 7-6 - Shade for Color 3
 * -------------------------------------------------
 * Color # | BGP Mode   | OBP Mode 0 and 1
 * =============================================
 *     0      White        Transparent
 *     1      Light Gray   Light Gray
 *     2      Dark Gray    Dark Gray
 *     3      Black        Black
 */

#define BGPAL  0XFF47 // Background Palette Data 
#define OBPAL0 0XFF48 // Object Palette 0 Data
#define OBPAL1 0XFF49 // Object Palette 1 Data

/** 
 *        Sprite Attributes 
 * ---------------------------------
 * All 40 sprites are stored between 0x8000-0x8FFF
 * All unsigned Values 
 */
#define SPRITE_OAM 0xFE00 // Sprite Object Attribute Memory
#define BG_PRIOR 1<<7
#define YFLIP    1<<6
#define XFLIP    1<<5
#define PAL_NO   1<<4

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

void draw_tile(gb_short control);
void draw_sprite(gb_short control);
color get_color(gb_short ncolor, gb_long paladdr);

#endif
