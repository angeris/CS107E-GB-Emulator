#include <stdio.h>
#include "bgl.h"


int main() {
    for(int i=0x150; i<0x160; i++) {
        printf("At 0x%04x : 0x%02x\n",i, GB_ROM[i]);
    }
    return 0;
}
