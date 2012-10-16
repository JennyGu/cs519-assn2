#include <stdio.h>

#define BLOCK 512

unsigned int write_order[8] = {20, 55, 128, 40, 32, 1, 60, 32};

static unsigned int add(unsigned char *buffer, unsigned int size) {
    unsigned int i, count = 0;

    for (i = 0; i < size; i ++) {
        count += buffer[i];
    }

    return count;
}

int main(int argc, char *argv[]) {
    
    unsigned int i, valid = 1;
    unsigned char val[BLOCK];
    FILE *fh;

    if (argc < 2) {
        printf("Need a file argument\n");
        return -1;
    }

    fh = fopen(argv[1], "r+");
    if (fh == NULL) {
        printf("Unable to open file\n");
        return -1;
    }

    printf("Readback... ");

    for (i = 0; i < 256; i++) {
        fread(val, BLOCK, 1, fh);

        if ((val[0] != i) || (val[1] != i) || (val[2] != i) || (val[3] != i))
            valid = 0;
    }

    if (valid)
        printf("success\n");
    else
        printf("failed\n");
    

    printf("Modify... ");

    for (i = 0; i < 8; i++) {
        fseek(fh, write_order[i] * 512, 0);
        fread(val, BLOCK, 1, fh);
        val[ write_order[i] * 2 ] = 0;
        
        fseek(fh, write_order[i] * 512, 0);
        fwrite(val, BLOCK, 1, fh);
    }
   
    valid = 0;
    fseek(fh, 0, 0);
    for (i = 0; i < 256; i ++) {

        fread(val, BLOCK, 1, fh);

        if ((add(val, BLOCK) == (i * 512)) ||
            (add(val, BLOCK) == ((i * 512) - i)))
            valid++;
    }

    if (valid == 256)
        printf("success\n");
    else
        printf("failed\n");
        

    return 0;
}
