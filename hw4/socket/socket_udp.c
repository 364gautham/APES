#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <stdbool.h>


typedef struct msg
{
  char str[25];
  bool led_state;
} ms_t;



int main()
{
  pid_t cpid = 0;
  int psock = 0, csock = 0;
  ms_t data;
  char *psockaddr_path = "/tmp/parent_sockaddr";
  char *csockaddr_path = "/tmp/child_sockaddr";
  struct sockaddr_un psockaddr, csockaddr;
  socklen_t psockaddr_len, csockaddr_len;

  bzero(&data, sizeof(ms_t));
  remove(psockaddr_path);
  remove(csockaddr_path);

 

  switch (cpid = fork())
  {
    case -1: 
      perror("fork failed");
      break;

    case 0:      


      csock = socket(AF_UNIX, SOCK_DGRAM, 0);     

      if(csock == -1){
        perror("creation of child socket \n");
	exit(1);
	}

      psockaddr_len = sizeof(psockaddr);
      csockaddr_len = sizeof(csockaddr);

      bzero(&psockaddr, psockaddr_len);
      bzero(&csockaddr, csockaddr_len);

      csockaddr.sun_family = AF_UNIX;
      strncpy(csockaddr.sun_path, csockaddr_path, sizeof(csockaddr.sun_path) -1);

      if(bind(csock, (struct sockaddr *) &csockaddr, sizeof(struct sockaddr_un)) == -1)
        {
		perror("bind child");
		exit(1);
	}
    

      if(recvfrom(csock, (void *) &data, sizeof(ms_t), 0, (struct sockaddr *) &psockaddr, &psockaddr_len) == -1)
        {
		perror("receive parent to child");
		exit(1);	
	}
      

      printf("Received string in Child : %s Received LED State: %s\n", data.str, data.led_state ? "true" : "false");

      strcat(data.str, " Child");
      data.led_state = !data.led_state;

      printf("Sending modified string: %s  Modified Received LED State: %s.\n", data.str, data.led_state ? "true" : "false");

      if(sendto(csock, (const void *) &data, sizeof(ms_t), 0, (const struct sockaddr *) &psockaddr, psockaddr_len) == -1)
        {
		perror("sending from child to parent");
		exit(1);
	}

        if(close(csock == -1))        
          {
		perror("close child_sock");
		exit(1);
	}

        remove(csockaddr_path);
       

      break;

    default: 
      psock = socket(AF_UNIX, SOCK_DGRAM, 0);     //create a datagram socket

      if(psock == -1)
       {
	 perror("creation parent_sock");
	 exit(1);
	}
	
      psockaddr_len = sizeof(psockaddr);
      csockaddr_len = sizeof(csockaddr);
      bzero(&psockaddr, psockaddr_len);
      bzero(&csockaddr, csockaddr_len);

      psockaddr.sun_family = AF_UNIX;
      strncpy(psockaddr.sun_path, psockaddr_path, sizeof(psockaddr.sun_path) -1);

      csockaddr.sun_family = AF_UNIX;
      strncpy(csockaddr.sun_path, csockaddr_path, sizeof(csockaddr.sun_path) -1);

      if(bind(psock, (struct sockaddr *) &psockaddr, sizeof(struct sockaddr_un)) == -1)
        {
		perror("bind parent");
		exit(1);
	}



      strcpy(data.str, "IPC Socket Parent");
      data.led_state = false;

      /* Parent sleeping to allow child to set up socket */

      usleep(500);

      printf(" PARENT Sending string: %s LED State: %s \n", data.str, data.led_state ? "true" : "false");

      if(sendto(psock, (const void *) &data, sizeof(ms_t), 0, (const struct sockaddr *) &csockaddr, csockaddr_len) == -1)
        {
		perror("send parent to child");
		exit(1);
	}

      bzero(&data, sizeof(ms_t));

      if(recvfrom(psock, (void *) &data, sizeof(ms_t), 0, (struct sockaddr *) &csockaddr, &csockaddr_len) == -1)
        {
		perror("receive from child to parent");
		exit(1);
	}

      printf("PARENT Received string: %s  Received LED State: %s\n", data.str, data.led_state ? "true" : "false");

      if(close(psock == -1))        
        {
		perror("close parent_sock");
		exit(1);
	}

      remove(psockaddr_path);

      printf("IPC Socket communication successful\n");

      break;
  }

  exit(EXIT_SUCCESS);
}



