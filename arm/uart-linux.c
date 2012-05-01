#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define UART_DEV "/dev/ttymxc1"
#define RAW_DATA

int main(int argc, char* argv[])
{
   printf("entered main\n");
   fflush(stdout);
   /* read "man 2 open" to learn about O_NONBLOCK and O_NOCTTY */
   int fd = open(UART_DEV, O_RDWR, 0);
   if (fd == -1) {
      printf("fd == -1\n");
      fflush(stdout);
      goto error;
   }

   struct termios termInfo;
   if (tcgetattr(fd, &termInfo) == -1) {
      printf ("tcgetattr == -1\n");
      fflush(stdout);
      goto error;
   }

   /* flow control */
   termInfo.c_cflag &= ~CRTSCTS;
   //termInfo.c_cflag |= CRTSCTS;
   printf("flow control is off\n");
   fflush (stdout);

   /* BAUD rate */
   cfsetispeed(&termInfo, B19200);
   cfsetospeed(&termInfo, B19200);
   printf("baud rate set\n");
   fflush(stdout);


#ifdef RAW_DATA
   /* If you need to process the data in a raw format, you'll want to enable
    * this code.  Basically, if your transmitting/receiving anything other
    * than text data, enable this code.
    */
   cfmakeraw(&termInfo);
   printf("in raw data defines\n");
   fflush(stdout);
#endif

   if (tcsetattr(fd, TCSANOW, &termInfo) == -1) {
      printf("tcsetattr == -1\n");
      fflush(stdout);
      goto error;
   }

   /* read "man select" for more advanced/event driven reading */
   printf("Now going to continuosly read and write\n");
   fflush(stdout);
while (1) {
   write(fd, "a", 1);
   char buffer[64];
   size_t i = read(fd, buffer, 1);

   printf("%d: %02x\n", i, buffer[0]);
   fflush(stdout);
#if 0
   printf("done writing, attempt read\n");
   fflush(stdout);
   char buffer[64];
   read(fd, buffer, 1);
   printf("done reading\n");
   fflush(stdout);
   printf(buffer);
   printf("write and read complete\n");
   fflush(stdout);
#endif
}
   close(fd);

   return 0;

error:
   perror(UART_DEV);
   return 1;
}
