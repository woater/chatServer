# chatService

a brodcast chat server/client service, based on muduo net library writen by ChenShuo: http://github.com/chenshuo/muduo

# 协议

参考《Linux多线程编程》7.3节中定义的聊天服务协议，

- 服务端程序在某个端口侦听新的连接。
- 客户向服务端发起连接。
- 连接建立之后，客户端随时准备接受服务端的消息并在屏幕上显示出来。
- 客户端接受键盘输入，以回车为界，把消息发送给服务端。
- 服务端接收到消息之后，依次发送给每个连接到它的客户端；原来发送消息的客户端进程也会收到这条消息。
- 一个服务端进程可以同时服务多个客户端进程，当有消息到达服务端后，每个客户端进程都会收到同一条消息，服务端广播发送消息的顺序是任意的，不一定哪个客户端会先收到这条消息。
- （可选）如果消息A先于消息B到达服务端，那么每个客户端都会先收到A再收到B。

# 实现版本

- 不处理分包和粘包问题，使用最简单的echo服务器变体作为实现：base
- 引入头部长度信息，处理分包和粘包问题  base_sticky_half_package
- 使用线程池    multiThread
- 负载测试      loadTest



