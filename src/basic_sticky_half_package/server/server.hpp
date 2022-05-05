#ifndef CHATSERVER_BASIC_STICKY_HALF_PACKAGE_SERVER_HPP__
#define CHATSERVER_BASIC_STICKY_HALF_PACKAGE_SERVER_HPP__

#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include <muduo/net/Callbacks.h>
#include "../lengthCodec/lengthCodec.hpp"

#include <set>

class ChatServer {
public:
    // 构造函数
    ChatServer(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr);
    // 不可复制、拷贝
    ChatServer(const ChatServer&) = delete;
    ChatServer(const ChatServer&&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&&) = delete;

    // 处理连接（新连接或断开）
    void onConnection(const muduo::net::TcpConnectionPtr& connection);
    // bahavior on new message
    void onStringMessage(const muduo::net::TcpConnectionPtr& connection,
                        const std::string& msg,
                        muduo::Timestamp time);
    
    void start();
private:
    typedef std::set<muduo::net::TcpConnectionPtr> ConnectionSet;
    
    muduo::net::TcpServer server_;
    LengthCodec codec_;
    ConnectionSet connections_;
};

#endif