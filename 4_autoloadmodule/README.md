ubuntu18.04 不再使用initd管理系统，改用systemd.  

# **使用systemd设置开机启动**  
>为了像以前一样，在/etc/rc.local中设置开机启动程序，需要以下几步:  
```sudo ln -fs /lib/systemd/system/rc-local.service /etc/systemd/system/rc-local.service```  
```sudo touch /etc/rc.local```  
```sudo chmod 755 /etc/rc.local ```  
```编辑rc.local，添加需要开机启动的任务```  

# rc.loacl 例子
#!/bin/bash
echo "test rc " > /var/test.log  
#startup hello.ko  
insmod /home/ubuntu/gitlab/linux-demo/3_hello_driver/hello.ko  


