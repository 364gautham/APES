#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <asm/current.h>
#include <linux/proc_fs.h>
#include <linux/kfifo.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/delay.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("KTHREAD_MODULE");
MODULE_AUTHOR("GAUTHAM");


#define FIFO_SIZE 8
 
static DEFINE_KFIFO(test,long long unsigned int,FIFO_SIZE);
struct task_struct *task ;
struct task_struct *task1 ;


int thread_fnc1(void *data)
{

	int a;
	int b;
	u64 buf[6];
	struct task_struct *tk;
	struct task_struct *temp;

	u64 vru;

	while(!kthread_should_stop()){
	tk = current;

	INIT_KFIFO(test);
	//printk(KERN_INFO "In Thread 1:sending data to thread 2\n");
	/*
	if(kfifo_initialized(&test)){
			printk(KERN_INFO"INITIALISED FIFO\n");
		}
	*/
	
	
	temp = tk;
	b=temp->pid;
	buf[0]= b;
	vru = tk->se.vruntime;
	buf[1]= vru;

	temp = list_entry(temp->tasks.prev,struct task_struct,tasks);
	b = temp->pid;	
	buf[2]=b;
	vru = temp->se.vruntime;
	buf[3]= vru;

	tk = list_entry(tk->tasks.next,struct task_struct,tasks);
	b = tk->pid;	
	buf[4]=b;
	vru = tk->se.vruntime;
	buf[5]= vru;
	
	for(a=0;a<6;a++){
	
		kfifo_put(&test,buf[a]);

	}
	//b = ab->vruntime;
	//buf[3]=b;

	//kfifo_put(&test,buf[0]);
		
	
	ssleep(1);
	
	
	}
	return 0;	
	//do_exit(0);	
}


int thread_fnc2(void *data)
{

	
	u64 dt[10];
	while(!kthread_should_stop()){
	printk(KERN_INFO "In Thread 2 \n");
	
	kfifo_get(&test, dt);
	printk(KERN_INFO "Curent Task PID is : %llu\n",dt[0]);	
	kfifo_get(&test, dt);
	printk(KERN_INFO "Curent Task Vruntime is : %llu\n",dt[0]);

	kfifo_get(&test, dt);
	printk(KERN_INFO "Previous Task PID is : %llu\n",dt[0]);	
	kfifo_get(&test, dt);
	printk(KERN_INFO "Previous Task Vruntime is : %llu\n",dt[0]);

	kfifo_get(&test, dt);
	printk(KERN_INFO "Next Task PID is : %llu\n",dt[0]);	
	kfifo_get(&test, dt);
	printk(KERN_INFO "Next Task Vruntime is : %llu\n",dt[0]);
	

	ssleep(2);
	
	}
	return 0;
	//do_exit(0);	
}

int kth_module(void)
{
	
	
	
	task= kthread_run(thread_fnc1,NULL,"thread1");
	printk(KERN_INFO"IN INIT MODULE\n");
	if(!task){
	printk(KERN_INFO "Thread_create failed \n");
	}

	task1 = kthread_run(thread_fnc2,NULL,"thread2");
	if(!task1){
	printk(KERN_INFO "Thread_create: 2 failed \n");
	}
	
	

	return 0;
}


void kth_exit_module(void)
{

	printk(KERN_INFO"Exiting kernel thread module\n");
	kthread_stop(task);
	kthread_stop(task1);	
}

module_init(kth_module);
module_exit(kth_exit_module);


