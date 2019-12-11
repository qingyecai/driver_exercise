#include <stdio.h>
#include <fcntl.h> //文件控制定义 
#include <termios.h>//终端控制定义 
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include "uart.h"

int uart_open(struct uart *uart)
{
	struct termios old_tio, new_tio;
	tcflag_t cflag = CS8 | CLOCAL| CREAD;
	int oflag = O_RDWR | O_NOCTTY;

	// if (uart->blocking_mode == 0)
	// 	oflag |= O_NONBLOCK;

	uart->uart_fd = open(uart->uart_path, oflag);
	if (uart->uart_fd < 0)
		return -1;

	tcgetattr(uart->uart_fd, &old_tio);
	memset(&new_tio, 0, sizeof(new_tio));

	switch(uart->baud_rate) {
		case 1200:
			cflag |= B1200;		break;
		case 2400:
			cflag |= B2400;		break;
		case 4800:
			cflag |= B4800;		break;
		case 9600:
			cflag |= B9600;		break;
		case 19200:
			cflag |= B19200;	break;
		case 38400:
			cflag |= B38400;	break;
		case 57600:
			cflag |= B57600;	break;
		case 115200:
			cflag |= B115200;	break;
		default:
			close(uart->uart_fd);
			return -1;
	}

	if (uart->parity_check > 0)
	{
		cflag |= PARENB;
		if (uart->parity_check == 1)
			cflag |= PARODD;
	}

	if (uart->two_stop_bit == 1)
		cflag |= CSTOPB;

	new_tio.c_cflag = cflag;
	new_tio.c_iflag = IGNPAR;
	new_tio.c_oflag = 0;
	new_tio.c_lflag = 0;

	// if (uart->blocking_mode == 0)
	// 	new_tio.c_cc[VMIN] = 0;
	// else
	// 	new_tio.c_cc[VMIN] = 1;

	new_tio.c_cc[VTIME] = 0;
	tcflush(uart->uart_fd, TCIFLUSH);
	tcsetattr(uart->uart_fd, TCSANOW, &new_tio);

	return 0;
}

int uart_close(struct uart *uart)
{
	if (uart->uart_fd)
	{
		close(uart->uart_fd);
		uart->uart_fd = 0;
	}
	return 0;
}

int uart_recv(struct uart *uart, char *data, unsigned int data_len)
{
	unsigned int read_len = 0;
    if (uart->uart_fd < 0)
        return -1;


    fd_set fs_read; 
    struct timeval tv_timeout; 
      
    FD_ZERO(&fs_read); 
    FD_SET(uart->uart_fd, &fs_read); 

  
    tv_timeout.tv_sec = (10*20/115200+2); 
    tv_timeout.tv_usec = 0; 
    select(uart->uart_fd+1, &fs_read, NULL, NULL, &tv_timeout);

    if (FD_ISSET(uart->uart_fd, &fs_read)) 
    { 
        read_len = read(uart->uart_fd, data, data_len); 
        printf("read len = %d\n", read_len); 
        return read_len; 
    } 
    else 
    { 
        return -1; 
    } 


	return 0;
}

int uart_send(struct uart *uart, char *data, unsigned int data_len)
{
	int send_len = 0;
	if (uart->uart_fd < 0)
		return -1;

	send_len = write(uart->uart_fd, data, data_len);
	if (send_len == data_len)
    {
        printf("total_send is %d\n",send_len); 
        return send_len; 
    }
    else
    {
        tcflush(uart->uart_fd, TCOFLUSH);//TCOFLUSH刷新写入的数据但不传送 
        return -1; 
    }

	return 0;
}