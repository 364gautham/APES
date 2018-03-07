#include<time.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/kernel.h>
#include<sys/syscall.h>


int main()
{
	int pid,uid;
	uint32_t i;
	uint32_t buffer_size = 25;
	int32_t *usort_array = NULL;
	int32_t *sort_array = NULL;
	time_t T =time(NULL);
	struct tm tm = *localtime(&T);
	
	pid = getpid();
	uid = getuid();
	
	printf("Process ID: %d\n",pid);
	printf("USER ID: %d\n",uid);
	printf("Process ID: %d\n",pid);	
	printf("System Date is : %02d / %02d / %04d \n",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
	printf("System Time is : %02d / %02d / %04d \n",tm.tm_hour,tm.tm_min,tm.tm_sec);
	
	usort_array = (int32_t*)malloc(buffer_size);
	if(usort_array == NULL)
	{	
		perror("Malloc failed in allocating memory\n");
	}
	sort_array = (int32_t*)malloc(buffer_size);
	if(sort_array == NULL)
	{	
		perror("Malloc failed in allocating memory\n");
	}
	
	for(i=0;i<buffer_size;i++)
	{
		*(usort_array+i)= rand() % 1000;
		printf("%d \t\n ",*(usort_array+i));
	}
	
	syscall(333,usort_array,sort_array,buffer_size);

	printf("Sorted Array \n");
	
	for(i=0;i<buffer_size;i++)
	{
		printf("%d \t",*(sort_array+i));
	}
	
	free(sort_array);
	free(usort_array);
	return 0;
}	
