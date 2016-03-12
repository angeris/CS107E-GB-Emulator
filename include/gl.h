#ifndef GL_H
#define GL_H

#include "fb.h"

#define GL_SINGLEBUFFER FB_SINGLEBUFFER
#define GL_DOUBLEBUFFER FB_DOUBLEBUFFER

void gl_init(unsigned width, unsigned height, unsigned mode);

#define GL_DEPTH 4

#define GL_BLACK 0xFF000000
#define GL_WHITE 0xFFFFFFFF
#define GL_RED   0xFF0000FF
#define GL_GREEN 0xFF00FF00
#define GL_BLUE  0xFFFF0000

// #define MIN(x,y) ((x)<(y))?(x):(y) // previously defined in printf.h

#define GL_MAX_FONT_SIZE 12*18

#define GL_FONT_WIDTH 12
#define GL_FONT_HEIGHT 18

/*
 * Current state of the machine kept locally. Somewhat redundant, but wouldn't
 * want to screw with accessing this memory directly from the GPU (would involve
 * modifying some code in fb and allowing additional access to stuff).
 */
static struct {
    unsigned height;
    unsigned width;
    unsigned mode;
    unsigned fbsize_b; // So it doesn't have to be recomputed every time.
    unsigned char *curr_buf; // Branch prediction is crappy (c.f. line 52 fb.c).
} _state;

typedef unsigned color;

void gl_swap_buffer(void);

color gl_color( unsigned char r, unsigned char g, unsigned char b );

unsigned gl_get_width(void);
unsigned gl_get_height(void);

void gl_clear(color c);

void gl_draw_pixel(int x, int y, color c);

void gl_draw_rect(int x, int y, int w, int h, color c);

void gl_draw_char(int x, int y, char letter, color c);

void gl_draw_string( int x, int y, char* string, color c);

unsigned gl_get_char_height(void);
unsigned gl_get_char_width(void);

#endif
