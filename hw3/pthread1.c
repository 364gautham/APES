/*
* FILE DESCRIPTION : MULTITHREADED PROGRAM WITH THREE THREADS
* Master creates two child threads for two different applications
* Child 1 : Has Job od periodic task of logginf CPU utilisation data to text file
* Child 2: Searches a defined text file for characters having 3 occurences
* Name of text file is to be provided by user
* AUTHOR : GAUTHAM K A
* DATE : 17th February 2018
* APES HW3
*/ 


#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/time.h>
#include<sys/syscall.h>

char file[20] ;
static void *child1(void *arg);
static void *child2(void *arg);
pthread_t thread1 ,thread2;

/*  Structure used for arguments passing between threads*/

struct thread_info{
	pthread_t tid[2];
	FILE *fp;
};

/* Node for linked list operation */	

struct node{
	char b;
	int numb;
	struct node* next;
	
};

/* Periodic Task Called from Child 1 thread to log CPU stats */	

void cpu_task( int signum)
{	
	char ch;
	FILE *fp1 = fopen(file,"a");
	FILE *fptr = fopen("/proc/stat","r");
	if(fptr == NULL){
	printf("\nFile cannot be opened for reading\n");	
	exit(0);
	}
	ch=fgetc(fptr);
	while(ch != EOF){
	fprintf(fp1,"%c",ch);
	ch = fgetc(fptr);
	}
	fclose(fptr);
	fclose(fp1);
	
}

/* Signal Handler to terminate child 1 using USR1 (USER DEFINED) signal */
	
static void my_sig(int a){

	//printf("iin signal handler \n");
	struct thread_info *tinfo;	
	FILE * fp1 = fopen(file,"a");
	fprintf(fp1,"\n \n *****Exiting thread Child 1***\n\n");
	fclose(fp1);
	pthread_cancel(thread1);


}

/*
*
*Main thread creates child threads: 
*Child 1 : Periodic task which reports CPU stats every 100us
*Child 2: Linked list search operation to find characters in a text file
* Each of the three threads logs THREAD ID and POSIX thread ID.
*/

void main(){
	
	FILE *fp;
	int ret;
	struct timeval now;
	struct timespec start,end,start1,end1;

	
	
	char str[200];
	pthread_attr_t attr;
	struct thread_info *tinfo;
	
	tinfo = (struct thread_info*)malloc(sizeof(struct thread_info));
	if(! tinfo)
	{
		printf("Could not allocate thread info struct\n");
		exit(1);
	}
	
	printf("Enter file name to log to \n");
	scanf("%s",file);
	strcat(file,".txt");	
	pid_t tid = syscall(SYS_gettid);
	fp = fopen( file ,"a");
	if(!fp){
	printf("Could not open file for writing\n");
	exit(1);
	}
	snprintf(str,sizeof(str),"Master PID : %d and TID: %d \n",getpid(),tid);
	fprintf(fp,"POSIX THREAD ID MASTER %ld\n",pthread_self());
	printf("pid %d",getpid());
	ret = fprintf(fp,"%s\n",str);
	if(!ret)
	{
		printf("Could not write to file");
	}

/* START threading*/

	fprintf(fp,"%s","\n\n ****Parent : Creating threads*****\n\n");

	tinfo->fp = fp;
	fclose(fp);
	pthread_attr_init(&attr);
	
	if(pthread_create(&thread1,NULL,child1,(void*)tinfo))
	{
		printf("Could not create thread\n");
		exit(1);
	}/*
	else{
		gettimeofday(&now,NULL);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"Thread1\n");
		fprintf(ptr,"%ld.%06ld\n",now.tv_usec);
		fclose(ptr);		
	}*/	
	else{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"\n****Thread1: CHILD 1 Timestamp****\n");
		fprintf(ptr,"%ld\n",(start.tv_sec*(long)1e9)+start.tv_nsec);
		fclose(ptr);	
		
}
	
	pthread_attr_init(&attr);
	if(pthread_create(&thread2,NULL,child2,(void*)tinfo))
	{
		printf("Could not create thread\n");
		exit(1);
	} 
	        /*else{
		gettimeofday(&now,NULL);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"Thread2\n");
		fprintf(ptr,"%ld.%06ld\n",now.tv_usec);
		fclose(ptr);		
	}
else{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"\n****Thread2 Timestamp****\n");
		fprintf(ptr,"%ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);
		fclose(ptr);	
		
}*/

	

	if(pthread_join(thread1,NULL))
	{
		printf("Could not join\n");
		exit(1);
	}
	/*else{
		gettimeofday(&now,NULL);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"Thread1\n");
		fprintf(ptr,"%u.%06u\n",now.tv_usec);
		fclose(ptr);		
	}*/
	else{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"\n****Thread1 Exit Timestamp****\n");
		fprintf(ptr,"%ld\n",(end.tv_sec*(long)1e9)+end.tv_nsec);
		fclose(ptr);	
		


}
	if(pthread_join(thread2,NULL))
        {
                printf("Could not join\n");
                exit(1);
        }
	/*else{
		gettimeofday(&now,NULL);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"Thread2\n");
		fprintf(ptr,"%u.%06u\n",now.tv_usec);
		fclose(ptr);		
	}
else{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end1);
		FILE *ptr = fopen(file,"a");
		fprintf(ptr,"\n****Thread2 Exit Timestamp****\n");
		fprintf(ptr,"%ld\n",(end1.tv_sec*(long)1e9)+end1.tv_nsec);
		fclose(ptr);	
		
}*/

		
	fp = fopen(file,"a");
	fprintf(fp,"%s","Parent(Master Thread) : exiting\n"); 
	fclose(fp);

}


/* Child one - thread 1 */


static void * child1(void *arg){
	struct thread_info *tinfo = (struct thread_info*)arg;
	char str[200];
	//char str1[100];
	//const char d[2] = " ";
	//char *token;

	//sigset_t set;
	FILE * fp = tinfo->fp;
	fp = fopen(file,"a");
	
	pid_t tid = syscall(SYS_gettid);
	fprintf(fp,"******Entered CHILD1 THREAD*****\n");
	snprintf(str,sizeof(str),"Thread PID: %d TID: %d \n",getpid(),tid);
	fprintf(fp,"%s",str);
	fprintf(fp,"POSIX THREAD ID THREAD 1 %ld\n",pthread_self());	

	/*
	FILE *fp1 = fopen("/proc/stat","r");
	int i =0 ;
	long int sum =0 ,idle,lastsum=0,lastidle=0;
	long double idlefraction;
	fgets(str1,100,fp1);	
	fclose(fp1);
	token = strtok(str,d);
	while(token!=NULL)
	{
		token = strtok(NULL,d);
		if(token!=NULL)
			sum += atoi(token);
		if(i==3)
		idle = atoi(token);
		i++;
	}
	
	printf("\n Ide for : %lf %% of the time.",(1.0-(idle-lastidle)*1.0/(sum-lastsum))*100);

	*/
	fclose(fp);

	struct sigaction sa;
	struct itimerval timer;
	
	memset(&sa,0,sizeof(sa));
	sa.sa_handler = &cpu_task;
	sigaction(SIGVTALRM,&sa,NULL);
	timer.it_value.tv_sec = 0;
	timer.it_interval.tv_usec = 100000;
	
	timer.it_interval.tv_sec =0;
	timer.it_value.tv_usec =100000;
	
	setitimer(ITIMER_VIRTUAL,&timer,NULL);
	
	/*signal from thread Child 1 */
 	
	struct sigaction sa1;
	sa1.sa_handler = &my_sig;

	sigaction(SIGUSR1,&sa1,NULL);
	
	while(1);
	
	
	
	fprintf(fp,"Closing file and thread is being exited\n");
	fclose(fp);
	pthread_exit((void*)0);
	
}

static void * child2(void *arg){
	struct thread_info *tinfo = (struct thread_info*)arg;
	struct timespec start,finish;
	double elapsed;
	struct timespec start1,end1;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start1);
	clock_gettime(CLOCK_MONOTONIC,&start);
		
	char str[200];
	FILE * fp = tinfo->fp;
	fp = fopen(file,"a");
	fprintf(fp,"\n****Thread2 ENTRY Timestamp****\n");
	fprintf(fp,"%ld\n",(start1.tv_sec*(long)1e9)+start1.tv_nsec);		
	pid_t tid = syscall(SYS_gettid);
	fprintf(fp,"\n****entered child2 thread****\n");
	snprintf(str,sizeof(str),"Thread PID: %d TID: %d \n",getpid(),tid);
	fprintf(fp,"%s",str);

	fprintf(fp,"\n POSIX THREAD ID Child 2:  %ld\n",pthread_self());	
	
	

/* Linked List operation*/

	
	FILE *fptr;
	char filename[20];
	char ch;
	struct node* temp = (struct node*)malloc(sizeof(struct node));

	struct node* head = temp;		
	int i=0,j=0,m=0;
	fptr = fopen("read","r");
	if(fptr == NULL)
	fprintf(fp,"Cannot open file : read file \n");
	ch = fgetc(fptr);	
	while(ch != EOF)
	{
		if(i==0){
			
		temp->b = ch;
		temp->numb = 1;
		temp->next = NULL;
		i++;	
		ch = fgetc(fptr);	
		continue;
		}

		if(ch == temp->b){
		temp->numb += 1 ;
		ch = fgetc(fptr);			
		continue;
		}
		if(temp->next != NULL)
		{
			temp = temp->next;
			while(temp->next != NULL)
			{
				if(temp->b == ch){
				temp->numb += 1;
				m =0;
				temp =head;
					ch = fgetc(fptr);
				continue;
				}		
				temp = temp->next;
				m=1;
			}
			if(temp->b == ch){
			temp->numb +=1;	
			m=0;	
			temp =head;
				ch = fgetc(fptr);
			continue;}
		}

		else{
			struct node* temp1 = (struct node*)malloc(sizeof(struct node));
			temp1->b =ch;
			temp1->numb = 1;
			temp1->next = NULL;
			temp->next = temp1;
			temp = head;
			
		}
		if(m==1){
		
			struct node* temp1 = (struct node*)malloc(sizeof(struct node));
			temp1->b =ch;
			temp1->numb = 1;
			temp1->next = NULL;
			temp->next = temp1;
			temp = head;
			m= 0;	
		}
			ch = fgetc(fptr);	
	}
		
		fprintf(fp,"\n Characters that occur thrice in the given text file are\n\n");
		while(head->next != NULL){
		
		if(head->numb == 3)		
		fprintf(fp, "%c\n",head->b);
		head = head->next;
				
		}
	
		if(head->numb == 3)
		fprintf(fp,"%c\n",head->b);
		fclose(fptr);
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end1);
		fprintf(fp,"\n Thread2 EXIT Timestamp****\n");
		fprintf(fp,"%ld\n",(end1.tv_sec*(long)1e9)+end1.tv_nsec);
		
		fclose(fp);
		pthread_exit((void*)0);	

}


		

	



	
	
