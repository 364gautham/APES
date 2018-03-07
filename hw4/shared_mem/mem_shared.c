#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdbool.h>


typedef struct msg
{
  char str[16];
  bool led_state;
} ms_t;



int main()
{
  pid_t cpid ;
  int shm;
  sem_t *sem;
  void *map_addr = NULL;
  ms_t data;

  bzero(&data, sizeof(ms_t));
  shm_unlink("shared_mem");
  sem_unlink("semaphore");



  switch (cpid = fork())
  {
    case -1: 
     perror("fork failed");
      break;

    case 0: 
  
      shm = shm_open("shared_mem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
      sem = sem_open("semaphore", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 0);

      if(shm == -1)
        {
	perror("Failed to create descriptor\n");
	exit(1);
   	}

      if(ftruncate(shm, sizeof(ms_t)) == -1)
        {
	perror("Failed to truncate : Resize\n");
	exit(1);
   }

      map_addr = mmap(NULL, sizeof(ms_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
      if (map_addr == MAP_FAILED)
        {
	perror("Failed to mmap \n");
	exit(1);
   }

     

      sem_wait(sem);
      memcpy((void *) &data, map_addr, sizeof(ms_t));           /* Copy shared memory to data*/

      printf("Received string in Child: %s. Received LED State: %s.\n", data.str, data.led_state ? "true" : "false");

      strcat(data.str, " child");
      data.led_state = true;
      printf("Sending modified string: %s . Modified Received LED State: %s.\n", data.str, data.led_state ? "true" : "false");

      memcpy(map_addr, (void *) &data, sizeof(ms_t));           /* Copy data to shared memory */
      msync(map_addr, sizeof(ms_t), MS_SYNC);
      sem_post(sem);

      close(shm);
      sem_close(sem);
      sem_unlink("semaphore");


      break;

    default: 
      shm = shm_open("shared_mem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
      sem = sem_open("semaphore", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 0);

      if(shm == -1)
        {
	perror("Failed to create descriptor\n");
	exit(1);
   }

      if(ftruncate(shm, sizeof(ms_t)) == -1)
        {
	perror("Failed to resize\n");
	exit(1);
   }

      map_addr = mmap(NULL, sizeof(ms_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
      if (map_addr == MAP_FAILED)
        {
	perror("Failed to mmap\n");
	exit(1);
   }

     

      strcpy(data.str, "IPC: Memory- Parent ");
      data.led_state = false;

      
      usleep(500);

      printf("Updating string in the shared memory: %s LED State: %s.\n", data.str, data.led_state ? "true" : "false");

      memcpy(map_addr, (void *) &data, sizeof(ms_t));           /* Copy data to shared memory */
      msync(map_addr, sizeof(ms_t), MS_SYNC);
      sem_post(sem);
      usleep(500);

      bzero(&data, sizeof(ms_t));

      sem_wait(sem);
      memcpy((void *) &data, map_addr, sizeof(ms_t));           /* Copy shared memory to data*/

      printf(" Received string in Parent : %s. Received LED State: %s.\n", data.str, data.led_state ? "true" : "false");

      close(shm);
      sem_close(sem);
      sem_unlink("semaphore");
      shm_unlink("shared_mem");
      printf("Communication successful. \n");

      break;
  }

  exit(EXIT_SUCCESS);

}



