#include<stdio.h>

int main(void) {

    unsigned char buffer[512];
    unsigned int i, bufloc;

    FILE *fh = fopen("loop_block.tst", "w");

    for (i = 0; i < 256; i++) {

        for (bufloc = 0; bufloc < 512 ; bufloc++)
            buffer[bufloc] = i;

        fwrite((void *) &buffer, 512, 1, fh);

    }
    
    fclose(fh);

    return 0;
}
