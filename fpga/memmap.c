//============================================================================
// Name        : main.c
// Author      :
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define BASE_ADDRESS 0xD3000000

int main()
{
    int fd;
    unsigned int offset = 24, data = 0;
    unsigned int *pmem, *pbase;

    printf("Teaching mode! :-)\n\r");

    // open the driver
    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if(!fd) {
        printf("Unable to open /dev/mem.  Ensure it exists (major=1, minor=1)\n");
        return -1;
    }


    // calculate address and do read
    pbase = (unsigned int *)mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0xD3000000 & ~MAP_MASK);
    pmem = pbase + ((offset & MAP_MASK)>>2);
    data = *pmem;
	
	//printf("pmem is ", *pmem);
	printf("Data out it %d"\n, data);

    // close driver
    close(fd);

    // report results
  //  printf("\n\rRead 0x%08X from virtual address 0x%08X (physical address 0x%08X)\n\r", data, (unsigned int)pmem, BASE_ADDRESS+(offset&~3));
    return data;
}
