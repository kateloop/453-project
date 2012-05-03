#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <sys/mman.h>

// UART defines
#define UART_DEV "/dev/ttymxc1"
#define RAW_DATA

// Sound Stuff Defines
#define MEM(x)   (*(volatile unsigned long *)(x))
#define FREQUENCY_PLAYER 0xD3000008 

// Memory Map Defines
#define MEM_DEV  "/dev/mem"
#define MAP_SIZE 2

typedef volatile struct {
   uint32_t count;
   uint32_t scratch[4];
} Registers;
 
int main(int argc, char* argv[])
{

   // Memory Map Setup
   long pageSize = sysconf(_SC_PAGESIZE);
   Registers* ptr = NULL;
   int status = 0;
   int fd_mem = -1;

     fd_mem = open(MEM_DEV, O_RDWR | O_SYNC, 0);
     if (fd_mem == -1) {
        perror(MEM_DEV);
        status = 1;
        goto exit;
     }

   ptr = mmap(NULL, MAP_SIZE * pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, 0xD3000000);
   if (ptr == MAP_FAILED) {
      perror(MEM_DEV);
      status = 1;
      goto exit2;
   }

   ptr->scratch[2] = 100000000/(512*300);
   printf("%08x\n", ptr->scratch[2]);
   fflush (stdout); 


   // UART setup
   /* read "man 2 open" to learn about O_NONBLOCK and O_NOCTTY */
   int fd = open(UART_DEV, O_RDWR, 0);
   if (fd == -1) {
      goto error;
   }

   struct termios termInfo;
   if (tcgetattr(fd, &termInfo) == -1) {
      goto error;
   }

   /* flow control */
   termInfo.c_cflag &= ~CRTSCTS;
   //termInfo.c_cflag |= CRTSCTS;

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

   if (tcsetattr(fd, TCSANOW, &termInfo) == -1) {
      goto error;
   }

   /* read "man select" for more advanced/event driven reading */
#if 0
while (0) {
   write(fd, "a", 1);
   char buffer[64];
   size_t i = read(fd, buffer, 1);

   printf("%d: %02x\n", i, buffer[0]);
   fflush(stdout);
   printf("done writing, attempt read\n");
   fflush(stdout);
   char buffer[64];
   read(fd, buffer, 1);
   printf("done reading\n");
   fflush(stdout);
   printf(buffer);
   printf("write and read complete\n");
   fflush(stdout);
}
#endif
   close(fd);
   
   munmap((void*) ptr, MAP_SIZE * pageSize);


   return 0;

exit:
   return status;

exit2:
   close (fd_mem);
   return status;

error:
   perror(UART_DEV);
   return 1;

}
