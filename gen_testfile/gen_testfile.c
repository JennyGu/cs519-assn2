#include<stdio.h>

int main(void) {

    unsigned int buffer[512 / sizeof(int)];
    unsigned int i, bufloc;

    FILE *fh = fopen("loop_block.tst", "w");

    for (i = 0; i < 256; i++) {

        for (bufloc = 0; bufloc < (512 / sizeof(int)); bufloc++)
            buffer[bufloc] = i & (i << 8) & (i << 16) & (i << 24);

        fwrite((void *) buffer, 512, 1, fh);

    }
    
    fclose(fh);

    return 0;
}
