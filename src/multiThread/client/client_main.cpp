#include "client.hpp"
#include "muduo/net/EventLoop.h"
#include <muduo/net/InetAddress.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpServer.h>
#include <iostream>
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <ip address(in number)> <port>\n", argv[0]);
        return -1;
    }

    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

    muduo::net::EventLoopThread loopThread;
    muduo::net::InetAddress listenAddress(argv[1], port);

    ChatClient client(loopThread.startLoop(), listenAddress);

    client.connect();

    // 读命令行输入，发到服务器
    std::string line;
    while (std::getline(std::cin, line)) {
        // line += line;
        // std::cout << "send: " << line.size() << std::endl;
        client.write(line);
    }

    return 0;
}