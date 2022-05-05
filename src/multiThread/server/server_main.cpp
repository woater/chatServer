#include "server.hpp"
#include "muduo/net/EventLoop.h"
#include <muduo/net/InetAddress.h>

int main(int argc, char* argv[]) {
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddress(2022);

    ChatServer server(&loop, listenAddress);

    if (argc >= 2) {
        int threadNum = atoi(argv[1]);
        server.setThreadNum(threadNum);
    }

    server.start();
    loop.loop();

    return 0;
}