使用同步IO仿Proactor的高并发Web服务器
webbench压力测试可达31465QPS
![image](https://github.com/Kritoooo/WebServer/assets/97887192/6b364baf-f010-440d-a607-98aa0c282ba4)

2024/4/5 数据库连接池测试
krito@krito-Standard-PC-i440FX-PIIX-1996:~/code/webserver/sqlconnectpool$ ./a.out 
连接池 单线程 用时：81866049571纳秒, 81866 毫秒
krito@krito-Standard-PC-i440FX-PIIX-1996:~/code/webserver/sqlconnectpool$ g++ *.cpp -I /usr/include/mysql/ -lmysqlclient -lpthread
krito@krito-Standard-PC-i440FX-PIIX-1996:~/code/webserver/sqlconnectpool$ ./a.out 
非连接池 单线程 用时：86226185275纳秒, 86226 毫秒
![image](https://github.com/Kritoooo/WebServer/assets/97887192/e5282dd6-1122-4ed6-a871-fc3a6587ef48)
