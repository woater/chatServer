#include "server.hpp"
#include "muduo/net/EventLoop.h"
#include <muduo/net/InetAddress.h>

int main() {
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddress(2022);

    ChatServer server(&loop, listenAddress);

    server.start();
    loop.loop();

    return 0;
}