#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define UART_DEV "/dev/ttyS0"
//#define RAW_DATA

int main(int argc, char* argv[])
{
   /* read "man 2 open" to learn about O_NONBLOCK and O_NOCTTY */
   int fd = open(UART_DEV, O_RDWR, 0);
   if (fd == -1)
      goto error;

   struct termios termInfo;
   if (tcgetattr(fd, &termInfo) == -1)
      goto error;

   /* flow control */
   //termInfo.c_cflag &= ~CRTSCTS;
   termInfo.c_cflag |= CRTSCTS;

   /* BAUD rate */
   cfsetispeed(&termInfo, B19200);
   cfsetospeed(&termInfo, B19200);

#ifdef RAW_DATA
   /* If you need to process the data in a raw format, you'll want to enable
    * this code.  Basically, if your transmitting/receiving anything other
    * than text data, enable this code.
    */
   cfmakeraw(&termInfo);
#endif

   if (tcsetattr(fd, TCSANOW, &termInfo) == -1)
      goto error;

   /* read "man select" for more advanced/event driven reading */
   write(fd, "\n", 1);
   char buffer[64];
   read(fd, buffer, 64);
   printf(buffer);

   close(fd);

   return 0;

error:
   perror(UART_DEV);
   return 1;
}
