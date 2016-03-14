#include <stdio.h>
#include "cins.h"


int main() {
    for(int i=0x73f; i<0x750; i++) {
        printf("At 0x%04x : 0x%02x\n",i, GB_ROM[i]);
    }
    return 0;
}
