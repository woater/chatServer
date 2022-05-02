#ifndef CHATSERVER_BASE_SERVER_HPP__
#define CHATSERVER_BASE_SERVER_HPP__

#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include <muduo/net/Callbacks.h>

#include <set>

class ChatServer {
public:
    // constructor
    ChatServer(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr);
    // no-copyable
    ChatServer(const ChatServer&) = delete;
    ChatServer(const ChatServer&&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&&) = delete;

    // bahavior on new connection
    void onConnection(const muduo::net::TcpConnectionPtr& connection);
    // bahavior on new message
    void onMessage(const muduo::net::TcpConnectionPtr& connection,
            muduo::net::Buffer* buffer,
            muduo::Timestamp time);
    
    void start();
private:
    typedef std::set<muduo::net::TcpConnectionPtr> ConnectionSet;

    muduo::net::TcpServer server_;
    ConnectionSet connections_;
};

#endif