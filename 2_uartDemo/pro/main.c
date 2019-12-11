#include <stdio.h>
//#include "uart.h"

//uart /dev/ttyUSB0 115200
int main( int argc, char *argv[] ) {

    if (argc<3)
    {
        printf("example: uart /dev/ttyUSB0 115200 \n");
        return 0;
    }
    for (size_t i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    return 0;
}
