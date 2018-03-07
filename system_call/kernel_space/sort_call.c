#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include<linux/types.h>
#include<asm-generic/errno.h>

#include "sort_call.h"

asmlinkage long sys_sortstring(int32_t __user* uns_buf, int32_t __user* sor_buf,int32_t size_buf)				
{
	//int32_t buf= size_buf;
	int32_t i,j,temp;
	int32_t *kern_buff = NULL;
	printk(KERN_INFO"Syscall enters\n");
	printk(KERN_INFO"size of the buffer %d \n",size_buf);

	if(uns_buf == NULL)
	{
		printk(KERN_ALERT "received null buffer from user\n");
		return EFAULT;
	}
	
	kern_buff = (int32_t*)kmalloc(4*size_buf, GFP_KERNEL);

	if(kern_buff == NULL)
	{
		printk(KERN_ALERT "Memory allocation failed in kernel space\n");
		return ENOMEM;
	}
	
	if(copy_from_user (kern_buff,uns_buf,(4*size_buf)) !=0)
	{
		printk(KERN_ALERT "copy from user space failed\n");
		return ESPIPE;
	}
	
	printk(KERN_ALERT "Sorting of buffer data started\n");
	
	
	for(i=0;i<(size_buf-1);i++)
	{		
		for(j=0;j<(size_buf-i-1);j++){
			if(*(kern_buff +j) < *(kern_buff+j+1))
			{
				temp = *(kern_buff+j);
				*(kern_buff+j)=*(kern_buff+j+1);
				*(kern_buff+j+1)=temp;
			}
		}
	}
	
	//sor_buf = &(kern_buf);
	printk(KERN_ALERT "Sorting of buffer is complete\n");
	if(copy_to_user(sor_buf,kern_buff,4*size_buf) !=0)
	{
		printk(KERN_ALERT" copy to user is unsuccessfull\n");
		return ESPIPE;
	}
	
	kfree(kern_buff);
	
	printk(KERN_INFO " system call exits \n");
	
	return 0;
		



}

