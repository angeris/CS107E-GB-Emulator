#include "GPU.h"

static unsigned _gpu_mode;
static unsigned _gpu_clock;
static unsigned _gpu_line;

gb_short gpu_read(gb_long addr) {

}

void gpu_exec() {
    _gpu_clock += 4;
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
}

void gpu_drawscreen() {
    //TODO: Draw everything into the framebuffer; probably just swap the buffer out
}
