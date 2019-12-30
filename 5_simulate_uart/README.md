1. 执行```make```命令，生成```chrdevbase.ko```
2. 执行```gcc chrdevbaseApp.c -o chrdevbaseApp```
3. 手动挂载模块 ```insmod chrdevbase```
<!-- 4. 手动创建节点 ```mknod /dev/chrdevbase c 200 0``` -->
5. 自动创建节点
6. 测试 读```./chrdevbaseApp /dev/chrdevbase 1``` 
7. 测试 写```./chrdevbaseApp /dev/chrdevbase 2``` 