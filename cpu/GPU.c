#include "GPU.h"

static unsigned _gpu_mode;
static unsigned _gpu_clock;
static unsigned _gpu_line;

gb_short gpu_read(gb_long addr) {
    // if(addr >= 0x8000 && addr <= 0x9FFF) 
    return vram[addr]; // Not sure if this is correct
    // Have to read from the Sprite OAM located at 0xFE00
    // else return 0;
}

void gpu_init() {
    gl_init( WIN_WIDTH, WIN_HEIGHT, 1);
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

    // Check LCD Control Register
    gb_short control = gpu_read(LCD_CONTROL_REG); 
    if(control & BG_DISPLAY_ENAB) draw_tile(control);
    if(control & OBJ_SPRITE_ENAB) draw_sprite(control);
}

void gpu_drawscreen() {
    gl_swap_buffer();
}

void draw_tile(gb_short control) {
    gb_long tileMem = 0;
    gb_long bgMem = 0;

    gb_short scrollY = gpu_read(SCROLLY) ;
    gb_short scrollX = gpu_read(SCROLLX) ;
    gb_short windowY = gpu_read(WINDOWY) ;
    gb_short windowX = gpu_read(WINDOWX) - 7;

    int usingWindow = 0; // boolean - whether or not the window is in use
    int unsig = 1; // boolean - whether or not tile set data is signed or unsigned

    if(control & WIN_DISP_ENABLE) { // Check if current window is within LCD Y position
        if(windowY <= gpu_read(LCDY)) {
            usingWindow = 1;
        }
    }

    // Choose Tile Data to Use
    if(control & WIN_BG_TILEDATA) {
        tileMem = TILE_SET_1U;
    } else {
        tileMem = TILE_SET_1S;
        unsig = 0; 
    }

    if(!usingWindow) { // Choose Background Tile Set To Use
        (control & BG_TILE_MAP_SEL) ? (bgMem = TILE_SET_BG_1) : (bgMem = TILE_SET_BG_0);
    } else { // Choose Window Tile Set To Use
        (control & WIN_TILE_SELECT) ? (bgMem = TILE_SET_BG_1) : (bgMem = TILE_SET_BG_0);
    }

    gb_short yPos = 0; // Which of the 32 vertical tiles is the current scanline is drawing
    usingWindow ? (yPos = gpu_read(LCDY) - windowY) : (yPos = scrollY + gpu_read(LCDY));

    // Choose which of the 8 vertical pixels of the current tile the sanline is on
    gb_long tileRow = (((gb_short)(yPos/8))*32);

    // Draw horizontal pixels for this scanline
    gb_short xPos;
    for(int px = 0; px < 160; px++) {
        xPos = px+scrollX;
        if(usingWindow && (px >= windowX)) xPos = px - windowX;

        gb_long tileCol = (xPos/8);
        gb_long_s tileNum;

        // Get the signed or unsigned tile identity number
        gb_long tileAddr = bgMem+tileRow+tileCol;
        if(unsig)
            tileNum = (gb_short)gpu_read(tileAddr);
        else 
            tileNum = (gb_short_s)gpu_read(tileAddr);

        // Find the tile in memory
        gb_long tileLoc = tileMem;
        if(unsig)
            tileLoc += (tileNum * 16);
        else
            tileLoc += ((tileNum+128)*16);

        // Find data from the correct vertical line of the current tile
        gb_short line = yPos % 8;
        line *= 2; // each vertical line takes up two bytes of memory
        gb_short data1 = gpu_read(tileLoc + line);
        gb_short data2 = gpu_read(tileLoc + line + 1);

        // Pixel 0 in the tile is bit 7 of data1 and data2
        // What's going on  here?
        int cBit = xPos % 8;
        cBit -= 7;
        cBit *= -1;

        // Combine data1 and data2 to get color id for this pixel
        int cNum;
        (data2 << cBit) ? (cNum = 1) : (cNum = 0); // Double check this is being done correctly
        cNum <<= 1;
        (data1 << cBit) ? (cNum |= 1) : (cNum |= 0);

        // Get actual color from background color palette
        color c = get_color(cNum, (gb_long)BGPAL);
        int finalY = gpu_read(LCDY);

        // Check To Be Within Bounds
        if ((finalY<0) || (finalY>143) || (px<0) || (px>159)) continue;

        gl_draw_pixel(px, finalY, c);
    }
}

void draw_sprite(gb_short control) {
    int use8x16sprite = 0; // boolean
    if(control & OBJ_SPRITE_SIZE) use8x16sprite = 1;

    // Iterate through all the sprites, and check which ones are displayed
    for(int sprite = 0; sprite < 40; sprite++) {
        gb_short index = sprite*4; // Each sprite occupies four bytes 
        gb_short yPos = gpu_read(SPRITE_OAM+index) - 16;
        gb_short xPos = gpu_read(SPRITE_OAM+index+1)-8;
        gb_short tileLoc = gpu_read(SPRITE_OAM+index+2);
        gb_short attributes = gpu_read(SPRITE_OAM+index+3);

        int yflip, xflip;
        (attributes & YFLIP) ? (yflip = 1) : (yflip = 0);
        (attributes & XFLIP) ? (xflip = 1) : (xflip = 0);

        int scanline = gpu_read(LCDY);

        int ysize = use8x16sprite ? 16 : 8;

        // Check if current sprite is being written to the current scanline
        if((scanline >= yPos) && (scanline < (yPos+ysize))) {
            int line = scanline - yPos;
            // Check if the sprite is being flipped vertically
            // If so, read in data backwards
            if(yflip) {
                line -= ysize;
                line *= -1;
            }

            line *= 2; // Sprites take up 2 bytes
            gb_long dataAddr = (TILE_SET_1U + (tileLoc * 16)) + line;
            gb_short data1 = gpu_read(dataAddr);
            gb_short data2 = gpu_read(dataAddr+1);

            // Reading in pixels from right to left
            for(int tilePix = 7; tilePix >= 0; tilePix--) {
                int cBit = tilePix;
                // Check if the sprite is being flipped horizontally
                if(xflip) {
                    cBit -= 7;
                    cBit *= -1;
                }

                int cNum;
                (data2 << cBit) ? (cNum = 1) : (cNum = 0); // Double check this is being done correctly
                cNum <<= 1;
                (data1 << cBit) ? (cNum |= 1) : (cNum |= 0);

                // Check which palette to use
                gb_long cAddr = (attributes & PAL_NO) ? OBPAL1 : OBPAL0;
                color c = get_color(cNum, cAddr);
                if(c == WHITE) continue; // white is transparent for sprites

                // Draw pixel to fb
                int xPix = 0 - tilePix;
                xPix += 7;
                int pixel = xPos+xPix;

                // Check that the pixel is within bounds
                if ((scanline<0) || (scanline>143) || (pixel<0) || (pixel>159)) continue;
                gl_draw_pixel(pixel, scanline, c);
            }

        }

    }

}

color get_color(gb_short ncolor, gb_long paladdr) {
    color result = WHITE;
    gb_short palette = gpu_read(paladdr);
    int hi = 0;
    int lo = 0;

    // Find which bits of the color palette the color #s map to
    switch (ncolor) {
        case 0: hi = 1; lo = 0; break;
        case 1: hi = 3; lo = 2; break; 
        case 2: hi = 5; lo = 4; break; 
        case 3: hi = 7; lo = 6; break;
    }

    // Get the color from the palette
    int color = 0;
    color = (palette << hi);
    color = color << 1;
    color |= (palette << lo);

    // Convert to Graphics Library color
    switch (color) {
        case 0: result = WHITE; break;
        case 1: result = LGRAY; break;
        case 2: result = DGRAY; break;
        case 3: result = BLACK; break;
    }

    return result;
}
