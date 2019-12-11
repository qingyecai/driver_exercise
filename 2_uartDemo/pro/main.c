#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

//uart /dev/ttyUSB0 115200
int main( int argc, char *argv[] ) 
{
    struct uart uart;
    char a[10] = {0};
    char b[10] = {0};
    if (argc < 3)
    {
        printf("example: uart /dev/ttyUSB0 115200 \n");
        return 0;
    }
    uart.uart_path = argv[1];
    uart.baud_rate = atoi(argv[2]);
    // uart.blocking_mode = 0;
    uart.parity_check = 0;
    uart.two_stop_bit = 0;

    uart_open(&uart);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            a[j] = 0x30 + i;
        }
        uart_send(&uart, a, 10);
        if (uart_recv(&uart, b, 10)>0)
        {
            printf("%s\n", b);
        }
    }    

    uart_close(&uart);
    return 0;
}
