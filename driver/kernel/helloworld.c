#include <linux/init.h>  
#include <linux/module.h>  
#include<linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define MAXNUM 100
#define MAJORNUM 456

static ssize_t globalvar_read(struct file *,char __user*,size_t ,loff_t *);
static ssize_t globalvar_write(struct file *,const char __user*,size_t ,loff_t *);
static int globalvar_open(struct inode *inode,struct file *filp);
static int globalvar_release(struct inode *inode,struct file *filp);


struct dev{
	struct cdev log;
	int flag;
	char buffer[MAXNUM+1];
	char *rd,*wr,*end;
}globalvar;
static struct class *myclass;
int major = MAJORNUM;

struct file_operations globalval_ops = {
	.read = globalvar_read,
	.write = globalvar_write,
	.open = globalvar_open,
	.release = globalvar_release,
};
static int globalvar_open(struct inode *inode,struct file *filp){

	return (0);
}
static int globalvar_release(struct inode *inode,struct file *filp){

	return (0);
}
static ssize_t globalvar_read(struct file *filp,char __user*buf,size_t len,loff_t *off){
	
	return len;
}
static ssize_t globalvar_write(struct file *filp,const char __user*buf,size_t len,loff_t * off){
	
	return len;
}
static int __init hello_init(void)  
{  
	int result = 0;
	int err = 0;
	dev_t dev = MKDEV(major,0);
	if(major){
		result = register_chrdev_region(dev,1,"charmem");
	}
	else{
		result = alloc_chrdev_region(&dev,0,1,"charmem");
		major = MAJOR(dev);
	}
	if(result<0)
		return result;
	cdev_init(&globalvar.log,&globalval_ops);
	globalvar.log.owner = THIS_MODULE;
	err = cdev_add(&globalvar.log,dev,1);
	if(err)
		printk(KERN_INFO "Error %d adding char_mem device", err);
   	 else
	{
		printk("globalvar register success\n");
		globalvar.rd = globalvar.buffer; //读指针
		globalvar.wr = globalvar.buffer; //写指针
		globalvar.end = globalvar.buffer + MAXNUM;//缓冲区尾指针
		globalvar.flag = 0; // 阻塞唤醒标志置 0
	}
	myclass = class_create(THIS_MODULE, "logdev0");
	device_create(myclass, NULL, dev, NULL, "logdev0");
	printk(KERN_INFO "Hello world,I'm in function %s!!!\n",__func__);  
	return 0;  
}  
module_init(hello_init);  
        
static void __exit hello_exit(void)  
{  
    printk(KERN_INFO "Bye Bye,I'm in function %s!!!\n",__func__);  
}  
module_exit(hello_exit);
MODULE_LICENSE("GPL");
