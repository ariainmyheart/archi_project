#include <stdio.h>

unsigned int get_word(FILE* in)
{
    unsigned char tmp;
    int ret = 0;
    int i;
    for (i = 0; i < 4; i++) {
        fread(&tmp, sizeof(char), 1, in);
        ret = ret << 8;
        ret = ret | tmp;
    }
    return ret;
}

int main()
{
    FILE* iimage = fopen("iimage.bin", "rb");

    printf("%08x\n", get_word(iimage));

    int n = get_word(iimage);
    printf("%08x\n", n);

    int i;
    for (i = 0; i < n; i++)
        printf("%08x\n", get_word(iimage));

    fclose(iimage);

    return 0;
}
