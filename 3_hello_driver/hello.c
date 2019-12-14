#include<linux/module.h>   //每个模块都要包括的头文件
#include<linux/kernel.h>   //用到了printk()函数
#include<linux/init.h>

MODULE_DESCRIPTION("HELLO MODULE");

MODULE_LICENSE("Dual BSD/GLP"); //没有指定license会出现error

static int hello_init(void)    //static使得该文件以外无法访问
{
    printk(KERN_ALERT "Hello World Init\n"); //只能使用内核里定义好的C函数，printk会根据日志级别将指定信息输出到控制台或日志文件中，KERN_ALERT会输出到控制台
    return 0;
}
static void hello_exit(void)
{
    printk(KERN_ALERT "Hello World Exit\n");
}

module_init(hello_init);
module_exit(hello_exit);