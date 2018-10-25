#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "shm.h"

int main(int argc, char* argv[])
{
  int          retVal = 0;


  /*<Use the POSIX "shm_open" API to open file descriptor with
    "O_RDWR" options and the "0666" permissions>*/
    const char *name = "shm.h";
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ShmData *shmPtr;
//  <Use the "mmap" API to memory map the file descriptor>
const size_t region_size = sizeof(shmPtr);
void	*shmPtr1 = mmap(0,region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmPtr1 == MAP_FAILED){
      printf("Mapping failed\n" );
      return(1);
   }
   shmPtr = (ShmData *) shmPtr1;


  printf("[Client]: Waiting for valid data ...\n");

  while(shmPtr->status != 1)
    {
      sleep(1);
    }

  printf("[Client]: Received %d\n",shmPtr->data);

  shmPtr->status = 2;

  // <use the "munmap" API to unmap the pointer>
  int unmapped = munmap(shmPtr, region_size);
  if(unmapped != 0){
  		printf("Unmapping error\n" );
  		return(1);

  }
  printf("[Client]: Client exiting...\n");

  return(retVal);

}
