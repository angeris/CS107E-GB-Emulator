#include "GPU.h"
#include "gl.h"

static unsigned _gpu_mode;
static unsigned _gpu_clock;
static unsigned _gpu_line;

// define 

unsigned int ScrollY; // (0xFF42): The Y Position of the BACKGROUND where to start drawing the viewing area from 
unsigned int ScrollX; // (0xFF43): The X Position of the BACKGROUND to start drawing the viewing area from
unsigned int WindowY; // (0xFF4A): The Y Position of the VIEWING AREA to start drawing the window from
unsigned int WindowX; // (0xFF4B): The X Positions -7 of the VIEWING AREA to start drawing the window from

gb_short gpu_read(gb_long addr) {
    /* TODO: Implement this */
    gb_short yeet = 0x00;
    return yeet;
}

void gpu_init() {
    gl_init( WIN_WIDTH, WIN_HEIGHT, 1);
    ScrollY = ScrollX = WindowY = WindowX = 0;
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
        draw_tile();
    }

    if(control & OBJ_SPRITE_ENAB) {
        draw_sprite();
    }


}

void gpu_drawscreen() {
    gl_swap_buffer();
    //TODO: Draw everything into the framebuffer; probably just swap the buffer out
}

void draw_tile() {

}

void draw_sprite() {

}
