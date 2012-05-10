// general includes
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

// udp client includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <libgen.h>
#include <netdb.h>

// our includes
#include "frequencies.h"
#include "uartVals.h"


// UART defines
#define UART_DEV "/dev/ttymxc1"
#define RAW_DATA

#define PLAY(FREQ) FREQ

// Sound Stuff Defines
int playNote(unsigned char);

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define BASE_ADDRESS 0xD3000000

int getFPGAData()
{
    int fd_mem;
    unsigned int offset = 36, data = 0;
    unsigned int *pmem, *pbase;

    // open the driver
    fd_mem = open("/dev/mem", O_RDWR|O_SYNC);
    if(!fd_mem) {
        printf("Unable to open /dev/mem.  Ensure it exists (major=1, minor=1)\n");
        return -1;
    }

     // calculate address and do read
    pbase = (unsigned int *)mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, 0xD3000000 & ~MAP_MASK);
    pmem = pbase + ((offset & MAP_MASK)>>2);
    data = *pmem;

    //printf("pmem is ", *pmem);
    printf("pbase %p\n", pbase);
    printf("%p\n", pmem);
    printf("Data out it %d\n", data);

    // close driver
    close(fd_mem);

    // report results
  //  printf("\n\rRead 0x%08X from virtual address 0x%08X (physical address 0x%08X)\n\r", data, (unsigned int)pmem, BASE_ADDRESS+(offset&~3));
    return data;
}


int main(int argc, char* argv[])
{
   // client setup
   struct sockaddr_in addr;
   struct hostent* host;
   char* tmp;
   int port;
   int fd_sock;
   int c;

   host = gethostbyname("128.104.180.233");
   if (host == NULL) {
      printf("unable to resovle host\n");
      fprintf(stderr, "%s: unable to resolve host\n", "128.104.180.233");
      return 1;
   }
   port = 1224UL;
   fd_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (fd_sock == -1) {
      printf("socket\n");
      perror("socket");
      return 1;
   }
   memset(&addr, 0, sizeof(struct sockaddr_in));
   addr.sin_family = AF_INET;
   memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);
   addr.sin_port = htons(port);

   if (connect(fd_sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in)) == -1) {
      printf("connect\n");
      perror("connect");
      return 1;
   }

   // UART setup
   int fd = open(UART_DEV, O_RDWR, 0);
   if (fd == -1) {
      goto error;
   }

   struct termios termInfo;
   if (tcgetattr(fd, &termInfo) == -1) {
      goto error;
   }

   /* turn off flow control */
   termInfo.c_cflag &= ~CRTSCTS;


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

int mode = 0;
int cur_mode = -1;
while (1) {
   // read value from FPGA
   //mode = getFPGAData();
   printf("mode: %d\n", mode);
   char send_buffer[64];
   // send mode across uart if different from current mode
   if (mode != cur_mode) {
      cur_mode = mode;
      write(fd, send_buffer, 1);
   }

   // read in from UART
   char buffer[64];
   size_t i = read(fd, buffer, 1);
   unsigned char val = buffer[0];
   int freq = playNote(val);
   char buf[5];
   snprintf(buf, 5, "%d", freq);
   write(fd_sock, buf, 4);  

}

   close(fd);

   return 0;

error:
   perror(UART_DEV);
   return 1;

}


int playNote (unsigned char note) {
    switch (note) {
        case UC3 :
            return PLAY(FC3);
            break;
        case UC3S :
            return PLAY(FC3S);
            break;
        case UD3F :
            return PLAY(FD3S);
            break;
        case UD3 :
            return PLAY(FD3);
            break;
        case UD3S :
            return PLAY(FD3S);
            break;
        case UE3F :
            return PLAY(FE3F);
            break;
        case UE3 :
            return PLAY(FE3);
            break;
        case UE3S :
            return PLAY(F3);
        break;
        case UF3F :
        return PLAY(FE3);
        case UF3 :
            return PLAY(FF3);
            break;
        case UF3S :
            return PLAY(FF3S);
            break;
        case UG3F :
            return PLAY(FG3F);
            break;
        case UG3 :
            return PLAY(FG3);
            break;
        case UG3S :
            return PLAY(FG3S);
            break;
        case UA3F :
            return PLAY(FA3F);
            break;
        case UA3 :
            return PLAY(FA3);
            break;
        case UA3S :
            return PLAY(FA3S);
            break;
        case UB3F :
            return PLAY(FB3F);
            break;
        case UB3 :
            return PLAY(FB3);
            break;
        case UB3S :
            return PLAY(FC4);
            break;
        case UC4F :
            return PLAY(FB3);
            break;
        case UC4 :
            return PLAY(FC4);
            break;
        case UC4S :
            return PLAY(FC4S);
            break;
        case UD4F :
            return PLAY(FD4S);
            break;
        case UD4 :
            return PLAY(FD4);
            break;
        case UD4S :
            return PLAY(FD4S);
            break;
        case UE4F :
            return PLAY(FE4F);
            break;
        case UE4 :
            return PLAY(FE4);
            break;
        case UE4S :
            return PLAY(FF4);
            break;
        case UF4F :
            return PLAY(FE4);
            break;
        case UF4 :
            return PLAY(FF4);
            break;
        case UF4S :
            return PLAY(FF4S);
            break;
        case UG4F :
            return PLAY(FG4F);
            break;
        case UG4 :
            return PLAY(FG4);
            break;
        case UG4S :
            return PLAY(FG4S);
            break;
        case UA4F :
            return PLAY(FA4F);
            break;
        case UA4 :
            return PLAY(FA4);
            break;
        case UA4S :
            return PLAY(FA4S);
            break;
        case UB4F :
            return PLAY(FB4F);
            break;
        case UB4 :
            return PLAY(FB4);
            break;
        case UB4S :
            return PLAY(FC5);
            break;
        case UC5F :
            return PLAY(FB4);
            break;
        case UC5 :
            return PLAY(FC5);
            break;
        case UC5S :
            return PLAY(FC5S);
            break;
        case UD5F :
            return PLAY(FD5S);
            break;
        case UD5 :
            return PLAY(FD5);
            break;
        case UD5S :
            return PLAY(FD5S);
            break;
        case UE5F :
            return PLAY(FE5F);
            break;
        case UE5 :
            return PLAY(FE5);
            break;
        case UE5S :
            return PLAY(FF5);
            break;
        case UF5F :
            return PLAY(FE5);
            break;
        case UF5 :
            return PLAY(FF5);
            break;
        case UF5S :
            return PLAY(FF5S);
            break;
        case UG5F :
            return PLAY(FG5F);
            break;
        case UG5 :
            return PLAY(FG5);
            break;
        case UG5S :
            return PLAY(FG5S);
            break;
        case UA5F :
            return PLAY(FA5F);
            break;
        case UA5 :
            return PLAY(FA5);
            break;
        case UA5S :
            return PLAY(FA5S);
            break;
        case UB5F :
            return PLAY(FB5F);
            break;
        case UB5 :
            return PLAY(FB5);
            break;
        case UOFF :
            return PLAY(OFF);
            break;
        default :
       return PLAY(OFF);
       break;
    }

}
