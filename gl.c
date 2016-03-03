#include "gl.h"
#include "font.h"

void gl_init(unsigned width, unsigned height, unsigned mode) {
    _state.height = height;
    _state.width = width;
    _state.mode = mode;
    _state.fbsize_b = width*height;

    fb_init(width, height, GL_DEPTH, mode);
    _state.curr_buf = fb_get_draw_buffer();
}

void gl_swap_buffer(void) {
    fb_swap_buffer();
    _state.curr_buf = fb_get_draw_buffer();
}

color gl_color( unsigned char r, unsigned char g, unsigned char b ) {
    return (color)((unsigned)r | (unsigned)g << 8 | (unsigned)b << 16 | GL_BLACK);
}

unsigned gl_get_width(void) {
    return _state.width;
}
unsigned gl_get_height(void) {
    return _state.height;
}

void gl_clear(color c) {
    unsigned long long *buf = (unsigned long long *)fb_get_draw_buffer(); 

    // I'm just going to assume the buffer size in bytes is divisible by 16.
    unsigned long long t_col = (unsigned long long)c << 32 | c;
    unsigned int t_size = _state.fbsize_b/2;
    for(unsigned i=0; i<t_size; i+=8, buf+=8) {
        buf[0] = t_col;
        buf[1] = t_col;
        buf[2] = t_col;
        buf[3] = t_col;
        buf[4] = t_col;
        buf[5] = t_col;
        buf[6] = t_col;
        buf[7] = t_col;
    }
}

void gl_draw_pixel(int x, int y, color c) {
    if(x >= _state.width || y >= _state.height) return;
    color (*buf)[_state.width] = (color (*)[_state.width])_state.curr_buf;
    buf[y][x] = c;
}

// Unsafe functions, only local use; but fast.
static void u_draw_pixel(int x, int y, color c) {
    color (*buf)[_state.width] = (color (*)[_state.width])_state.curr_buf;
    buf[y][x] = c;
}

// Slow due to branch prediction/access patterns. Should be improved.
void gl_draw_rect(int x, int y, int w, int h, color c) {
    if(x+w >= _state.width || y+h >= _state.height || x < 0 || y < 0) return;

    for(int i=w-1; i>=0; i--) 
        for(int j=h-1; j>=0; j--) 
            u_draw_pixel(i+x, j+y, c);
           
}

// Somewhat fast, since this will be used fairly often.
void gl_draw_char(int x, int y, char letter, color c) {
    static unsigned font_buf[GL_MAX_FONT_SIZE];

    if(x+font_get_width() >= _state.width || y+font_get_height() >= _state.height) 
        return;

    font_get_char(letter, (char *)font_buf, font_get_size());

    color (*buf)[_state.width] = 
        (color (*)[_state.width])(&((color (*)[_state.width])_state.curr_buf)[y][x]);

    for(int i=font_get_height()*font_get_width()-1; i>=0; i--)
        buf[i/GL_FONT_WIDTH][i%GL_FONT_WIDTH] = font_buf[i] & c;
}

// Will not write over the end of the buffer, but also may be slow if printing
// a large string that goes outside of it.
void gl_draw_string( int x, int y, char* string, color c) {
    char *p = string;
    while(*p) {
        gl_draw_char(x, y, *p, c);
        p++;
        x += GL_FONT_WIDTH;
    }
}

unsigned gl_get_char_height(void) { return GL_FONT_HEIGHT; }
unsigned gl_get_char_width(void) { return GL_FONT_WIDTH; }

