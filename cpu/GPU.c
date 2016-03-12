#include "GPU.h"
#include "gl.h"

static unsigned _gpu_mode;
static unsigned _gpu_clock;
static unsigned _gpu_line;

// define 

/*
unsigned int ScrollY; // (0xFF42): The Y Position of the BACKGROUND where to start drawing the viewing area from 
unsigned int ScrollX; // (0xFF43): The X Position of the BACKGROUND to start drawing the viewing area from
unsigned int WindowY; // (0xFF4A): The Y Position of the VIEWING AREA to start drawing the window from
unsigned int WindowX; // (0xFF4B): The X Positions -7 of the VIEWING AREA to start drawing the window from
*/

gb_short gpu_read(gb_long addr) {
    if(addr >= 0x8000 && addr <= 0x9FFF) return vram[addr]; // Not sure if this is correct
    else return 0;
}

void gpu_init() {
    gl_init( WIN_WIDTH, WIN_HEIGHT, 1);
    // ScrollY = ScrollX = WindowY = WindowX = 0;
    printf("gpu_init\n");
}

void gpu_exec() {
    _gpu_clock += 4;
    
    /*
    printf("_gpu_clock = %d\n", _gpu_clock);
    printf("_gpu_mode = %d\n", _gpu_mode);
    printf("_gpu_line = %d\n", _gpu_line);
    */

    switch(_gpu_mode) {
    case MODE_HBLANK:
        if(_gpu_clock >= 201) {
            _gpu_clock = 0;
            _gpu_line++;

            if(_gpu_line>=143) {
                _gpu_mode = MODE_VBLANK;
                gpu_drawscreen();
                return;
            }
            _gpu_mode = MODE_READOM;   
        }
        break;
    case MODE_VBLANK:
        if(_gpu_clock >= 4560) {
            _gpu_mode = MODE_READOM;
            _gpu_line = 0;
            _gpu_clock = 0;
        }
        break;
    case MODE_READOM:
        if(_gpu_clock >= 77) {
            _gpu_mode = MODE_READAL;
            _gpu_clock = 0;
        }
        break;
    case MODE_READAL:
        if(_gpu_clock >= 169) {
            _gpu_mode = MODE_HBLANK;
            _gpu_clock = 0;

            gpu_writeline();
        }
        break;
    }

}

void gpu_writeline() {
    //TODO: A crapload of work

    // Check LCD Control Register
    gb_short control = gpu_read(LCD_CONTROL_REG); 

    if(control & BG_DISPLAY_ENAB) { 
        draw_tile(control);
    }

    if(control & OBJ_SPRITE_ENAB) {
        draw_sprite();
    }


}

void gpu_drawscreen() {
    gl_swap_buffer();
    //TODO: Draw everything into the framebuffer; probably just swap the buffer out
}

void draw_tile(gb_short control) {
    gb_long tileMem = 0;
    gb_long bgMem = 0;

    gb_short scrollY = gpu_read(SCROLLY) ;
    gb_short scrollX = gpu_read(SCROLLX) ;
    gb_short windowY = gpu_read(WINDOWY) ;
    gb_short windowX = gpu_read(WINDOWX) - 7;

    int windowInUse = 0; // boolean variable
    int unsig = 1;

    if(control & WIN_DISP_ENABLE) { // Check if current window is within windows Y position
        if(windowY <= gpu_read(SCROLLY + 2)) { // 0xFF44
            windowInUse = 1;
        }
    }

    // Choose Tile Data to Use
    if(control & WIN_BG_TILEDATA) {
        tileMem = TILE_SET_1U;
    } else {
        tileMem = TILE_SET_1S;
        unsig = 0; 
    }

    if(!windowInUse) { // Choose Background Tile Set To Use
        (control & BG_TILE_MAP_SEL) ? (bgMem = TILE_SET_BG_1) : (bgMem = TILE_SET_BG_0);
    } else { // Choose Window Tile Set To Use
        (control & WIN_TILE_SELECT) ? (bgMem = TILE_SET_BG_1) : (bgMem = TILE_SET_BG_0);
    }

}

void draw_sprite() {

}
