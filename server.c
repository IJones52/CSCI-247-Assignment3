#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "shm.h"

int main(int argc, char* argv[])
{
	int retVal = 0;

//  <Confirm argc is 2 and if not print a usage string.>
if(argc != 2 || sizeof(atoi(argv[1])) != sizeof(int) ){
  printf("Usage: ./server.c [int argument] \n" );
  return(1);
}
/*  <Use the POSIX "shm_open" API to open file descriptor with
    "O_CREAT | O_RDWR" options and the "0666" permissions>*/
    const char *name = "shm.h";
		int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if(fd == -1){
    printf("Error creating shared memory object\n" );
    return(1);
  }

  /*<Use the "ftruncate" API to set the size to the size of your
    structure shm.h>*/
		ShmData *shmPtr;
		off_t size = sizeof(shmPtr);
    if(ftruncate(fd, size) != 0){
      printf("Error finding header file\n" );
			return(1);

		}
		/*  <Use the "mmap" API to memory map the file descriptor>

		  <Set the "status" field to INVALID>
		  <Set the "data" field to atoi(argv[1])>
		  <Set the "status" field to VALID>
		*/

	const size_t region_size = sizeof(shmPtr);
	void	*shmPtr1 = mmap(0,region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			if(shmPtr1 == MAP_FAILED){
				printf("Mapping failed\n" );
				return(1);
		 }

		shmPtr = (ShmData *) shmPtr1;

		 shmPtr->status = 0;
		 shmPtr->data = atoi(argv[1]);
		 shmPtr->status = 1;
  printf("[Server]: Server data Valid... waiting for client\n");

 while(shmPtr->status != 2)
    {
      sleep(1);
    }

  printf("[Server]: Server Data consumed!\n");
/*
  <use the "munmap" API to unmap the pointer>

  <use the "close" API to close the file Descriptor>

  <use the "shm_unlink" API to revert the shm_open call above>*/
int unmapped = munmap(shmPtr, region_size);
if(unmapped != 0){
		printf("Unmapping error\n" );
		return(1);

}

close(fd);


int unlinked = shm_unlink(name);
if(unlinked != 0){
		printf("Unlinking error error\n" );
		return(1);

}

  printf("[Server]: Server exiting...\n");


  return(retVal);

}
/*
Task 5: Postulate how you could use this IPC
Based on your knowledge of timers from the previous assignment, postulate (by providing a small analytical
paragraph) how you would use this method of IPC where a server is a producer of data and a client is a consumer
of data, to periodically get the data from the producer at regular intervals.

	In the previous assignment, we used timers to do a task on a regular interval. This assignment utilized a server to store data, and a client to recived the stored data.
These to tactics could be used in combination to have a process, the server, do a function such as checking on a system function on a regular interval and returning a value. In this context, the client, or multiple clients, would recive
the same data from the server, and could run different calculations on it based on what the client require, i.e. user input. This is a very basic version of what is used to run things like online
multiplayer video games, where a server gives info to the clients, and the clients make desecions based on what the server sends them in combination with the user's wishes.


*/
