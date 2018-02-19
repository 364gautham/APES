#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TIMER_MODULE");
MODULE_AUTHOR("GAUTHAM");

static struct timer_list my_timer;
static int count ;
static int timer_count=500;
static char* my_name = "Gautham";

module_param(my_name,charp,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP );
module_param(timer_count,int, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP );


void my_timer_callback(unsigned long data)
{
	int ret;	
	count = count + 1;	
	printk("timer callback called %d %s \n	", count,my_name);
	ret = mod_timer(&my_timer ,jiffies + msecs_to_jiffies(timer_count));
	if(ret) 
	printk("error in mod timer\n");
}

int init_module(void)
{
	int ret;
	printk("timer module installing\n");
	setup_timer(&my_timer, my_timer_callback,0);
	//add_timer(&my_timer);
	
	printk("starting timer to fire in %d seconds (%ld) \n",timer_count,jiffies);

	ret = mod_timer(&my_timer ,jiffies + msecs_to_jiffies(timer_count));
	if(ret) 
	printk("error in mod timer\n");
	return 0;
}

void cleanup_module(void)
{
	int ret;
	ret = del_timer(&my_timer);
	if(ret)
		printk("timer is still in use\n");
	printk("timer module uninstalling\n");
}

//module_init(init_module);
//module_exit(cleanup_module);


