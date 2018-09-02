#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include<linux/list.h>
#include<linux/kernel.h>
#include<asm/current.h>



MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PROCINFO_MODULE");
MODULE_AUTHOR("GAUTHAM");



int tree_module(void)
{
	struct task_struct *tk;
	int count  = 0;
	struct list_head *child;
	child = &(tk->children);
	tk = current;
	

	printk(KERN_INFO"Process tree information from current - init\n");
	
	

	for(tk = current ; tk != &init_task;tk= tk->parent)
	{
	
		count = 0;
		printk(KERN_INFO"Current process name is :  %s and its PID is %d",tk->comm,tk->pid);	

		printk(KERN_INFO"Current task nice value is %d",task_nice(tk));

		printk(KERN_INFO"Current task state is %ld",tk->state);

	
		list_for_each(child,&(tk->children))
		{
			count = count+1;		
		
		}
	
	printk(KERN_INFO"Number of children for current task are %d",count);
		
	}
		
	return 0;
}

void tree_exit_module(void)
{

	printk(KERN_INFO"Exiting process tree module\n");
}

module_init(tree_module);
module_exit(tree_exit_module);


