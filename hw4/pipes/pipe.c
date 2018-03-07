#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/types.h>


struct buf{
	char *str;
	int stlen;
	int led_st;
};	
	

int main()
{

	int fdp2c[2];  /* pipe file descriptors */
	int fdc2p[2];	
	struct buf temp,temp1;
	pid_t cpid;
	
	temp.str = "Gautham ipc p2c";
	temp.stlen = strlen(temp.str);
	temp.led_st = 0;

	temp1.str = "Gautham ipc c2p";
	temp1.stlen = strlen(temp1.str);
	temp1.led_st = 1;
	
	
	if(pipe(fdp2c) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);	
	}
	if(pipe(fdc2p) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);	
	}
	cpid = fork();
	if(cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if(cpid == 0){
	close(fdp2c[1]);   /* child only reads from pipe */
	close(fdc2p[0]);
	if(write(fdc2p[1],&temp1,sizeof(temp1))  != sizeof(temp1))
		printf("Failed : c2p write\n");
	struct buf par;
	if(read(fdp2c[0],&par,sizeof(par))!= sizeof(par))
		printf("Failed to read in child\n");
		
	printf("string recv in child is %s \n", par.str);
	printf("strlen  %d \n", par.stlen);
	printf("LED State %d \n", par.led_st);
	exit(EXIT_SUCCESS);
	
	}
 	
	else{
	
	close(fdp2c[0]);
	close(fdc2p[1]);
	

	if(write(fdp2c[1],&temp,sizeof(temp))  != sizeof(temp))
		printf("Failed : p2c write\n");
	struct buf par;
	if(read(fdc2p[0],&par,sizeof(par))!= sizeof(par))
		printf("Failed to read in parent\n");
		
	printf("string in parent is %s \n", par.str);
	printf("strlen in parent is %d \n", par.stlen);
	printf("LED State %d \n", par.led_st);
	exit(EXIT_SUCCESS);
	}
}
		
		
