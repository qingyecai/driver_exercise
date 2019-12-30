#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm-generic/ioctls.h>
#include <linux/termios.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define NEWCHR_CNT			    1		  	/* 设备号个数 */
#define CHRDEVBASE_NAME			"chrdevbase"	/* 名字 */

struct chrdevbase_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;		/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
};
struct chrdevbase_dev chrdev;

static char readbuf[100];		/* 读缓冲区 */
static char writebuf[100];		/* 写缓冲区 */
static char kerneldata[] = {"kernel data!"};

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int chrdevbase_open(struct inode *inode, struct file *filp)
{
	//printk("chrdevbase open!\n");
	return 0;
}

/*
 * @description		: 从设备读取数据 
 * @param - filp 	: 要打开的设备文件(文件描述符)
 * @param - buf 	: 返回给用户空间的数据缓冲区
 * @param - cnt 	: 要读取的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int retvalue = 0;
	
	/* 向用户空间发送数据 */
	memcpy(readbuf, kerneldata, sizeof(kerneldata));
	retvalue = copy_to_user(buf, readbuf, cnt);
	if(retvalue == 0){
		printk("kernel senddata ok!\n");
	}else{
		printk("kernel senddata failed!\n");
	}
	
	//printk("chrdevbase read!\n");
	return 0;
}

/*
 * @description		: 向设备写数据 
 * @param - filp 	: 设备文件，表示打开的文件描述符
 * @param - buf 	: 要写给设备写入的数据
 * @param - cnt 	: 要写入的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	int retvalue = 0;
	/* 接收用户空间传递给内核的数据并且打印出来 */
	retvalue = copy_from_user(writebuf, buf, cnt);
	if(retvalue == 0){
		printk("kernel recevdata:%s\n", writebuf);
	}else{
		printk("kernel recevdata failed!\n");
	}
	
	//printk("chrdevbase write!\n");
	return 0;
}

/*
 * @description		: 关闭/释放设备
 * @param - filp 	: 要关闭的设备文件(文件描述符)
 * @return 			: 0 成功;其他 失败
 */
static int chrdevbase_release(struct inode *inode, struct file *filp)
{
	printk("chrdevbase release！\n");
	return 0;
}
long chrdevbase_unlocked_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
    printk("chrdevbase_unlocked_ioctl！\n");
    printk("%x,%lx\n", cmd, arg);
    if(cmd==TCGETS)
    {
       
        
    }
    else if (cmd==TCSETS)
    {
        struct termios *p=(struct termios*)arg;
        if (p->c_cflag|B115200)
        {
            printk("B115200\n");
        }
    }
    return 0;
}
long chrdevbase_compact_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
    printk("chrdevbase_compact_ioctl！\n");
    printk("%x,%lx\n\n", cmd, arg);
    return 0;
}
/*
 * 设备操作函数结构体
 */
static struct file_operations chrdevbase_fops = {
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .read = chrdevbase_read,
    .write = chrdevbase_write,
    .release = chrdevbase_release,
    //.compat_ioctl = chrdevbase_compact_ioctl,
    .unlocked_ioctl = chrdevbase_unlocked_ioctl,
}
;

/*
 * @description	: 驱动入口函数 
 * @param 		: 无
 * @return 		: 0 成功;其他 失败
 */
static int __init chrdevbase_init(void)
{
/* 1、创建设备号 */
	if (chrdev.major) {		/*  定义了设备号 */
		chrdev.devid = MKDEV(chrdev.major, 0);
		register_chrdev_region(chrdev.devid, NEWCHR_CNT, CHRDEVBASE_NAME);
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&chrdev.devid, 0, NEWCHR_CNT, CHRDEVBASE_NAME);	/* 申请设备号 */
		chrdev.major = MAJOR(chrdev.devid);	/* 获取分配号的主设备号 */
		chrdev.minor = MINOR(chrdev.devid);	/* 获取分配号的次设备号 */
	}
	printk("newcheled major=%d,minor=%d\r\n",chrdev.major, chrdev.minor);	
	
	/* 2、初始化cdev */
	chrdev.cdev.owner = THIS_MODULE;
	cdev_init(&chrdev.cdev, &chrdevbase_fops);
	
	/* 3、添加一个cdev */
	cdev_add(&chrdev.cdev, chrdev.devid, NEWCHR_CNT);

	/* 4、创建类 */
	chrdev.class = class_create(THIS_MODULE, CHRDEVBASE_NAME);
	if (IS_ERR(chrdev.class)) {
		return PTR_ERR(chrdev.class);
	}

	/* 5、创建设备 */
	chrdev.device = device_create(chrdev.class, NULL, chrdev.devid, NULL, CHRDEVBASE_NAME);
	if (IS_ERR(chrdev.device)) {
		return PTR_ERR(chrdev.device);
	}
	return 0;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit chrdevbase_exit(void)
{
	cdev_del(&chrdev.cdev);/*  删除cdev */
	unregister_chrdev_region(chrdev.devid, NEWCHR_CNT); /* 注销设备号 */

	device_destroy(chrdev.class, chrdev.devid);
	class_destroy(chrdev.class);
	printk("chrdevbase exit!\n");
}

/* 
 * 将上面两个函数指定为驱动的入口和出口函数 
 */
module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

/* 
 * LICENSE和作者信息
 */
MODULE_LICENSE("GPL");
//MODULE_AUTHOR("");


