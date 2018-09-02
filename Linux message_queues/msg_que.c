#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdbool.h>


typedef struct mesg
{
  char str[20];
  bool led_state;
}ms_t;



int main(void)
{
  pid_t ch_pid;
  mqd_t mq;
  struct mq_attr attr;
  ms_t data;

  bzero(&data, sizeof(ms_t));
  mq_unlink("/msg_queue");



  switch (ch_pid = fork())
  {
    case -1: 
      perror("fork failed\n");
      break;

    case 0: 
    
      mq = mq_open("/msg_queue", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, NULL);

      if(mq == -1){
        perror("Failed message queue descriptor in Child");
	exit(1);
	}

      if(mq_receive(mq, (char *) &data, sizeof(data), 0) == -1){
        perror("Receiving from parent to child");
	exit(1);
	}

      printf("Received string in Child : %s Received LED State: %s\n", data.str, data.led_state ? "true" : "false");

      strcat(data.str, " Child");
      data.led_state = true;
      printf("CHILD Sending modified string: %s Modified Received LED State: %s\n", data.str, data.led_state ? "true" : "false");

      if(mq_send(mq, (const char *) &data, sizeof(data), 0) == -1)
        perror("sending from child to parent");

      mq_close(mq);
  

      break;

    default: 
      bzero(&attr, sizeof(attr));	
      attr.mq_flags = O_RDWR;			/* Setting attributes for message queue descriptor */
      attr.mq_maxmsg = 5;
      attr.mq_msgsize = sizeof(ms_t);

      mq = mq_open("/msg_queue", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &attr);

      if(mq == -1){
        perror("parent side creation of message queue descriptor");
	exit(1);
	}

   

      strcpy(data.str, "IPC Parent msg_queue");
      data.led_state = false;
	/* Parent sleeping so that child will set and read from queue */

      usleep(500);

      printf("Sending string from parent: %s LED State: %s /n", data.str, data.led_state ? "true" : "false");

      if(mq_send(mq, (const char *) &data, sizeof(data), 0) == -1){
        perror("sending from parent to child");
	exit(1);
	}

      bzero(&data, sizeof(ms_t));

      if(mq_receive(mq, (char *) &data, sizeof(data), 0) == -1){
        perror("receiving from child to parent");
	exit(1);
	}




      printf("Received string in Parent : %s Received LED State: %s /n", data.str, data.led_state ? "true" : "false");

      mq_close(mq);
      mq_unlink("/msg_queue");
      printf("Closing and Unlinking message queue.\n");

      break;
  }

  exit(EXIT_SUCCESS);

}




