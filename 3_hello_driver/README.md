**Shell 命令行**

> `modinfo hello.ko` #modinfo用来查看模块信息  
> `insmod hello.ko` #加载模块  
> `lsmod` #查看已经载入的模块，看看有没有hello  
> `dmesg` #如果成功应该可以看到最后一行输出了hello world  
> `rmmod hello` #卸载模块