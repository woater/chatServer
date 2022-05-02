#ifndef CHATSERVER_BASE_CLIENT_HPP__
#define CHATSERVER_BASE_CLIENT_HPP__

#include "muduo/net/TcpClient.h"
#include "muduo/base/Logging.h"
#include <muduo/net/Callbacks.h>
#include <mutex>
#include "../lengthCodec/lengthCodec.hpp"

class ChatClient {
public:
    ChatClient(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& serverAddress);
    ChatClient(const ChatClient&) = delete;
    ChatClient(const ChatClient&&) = delete;
    ChatClient& operator=(const ChatClient&) = delete;
    ChatClient& operator=(const ChatClient&&) = delete;

    void onConnection(const muduo::net::TcpConnectionPtr& connection);
    void onStringMessage(const muduo::net::TcpConnectionPtr& connection, const std::string& message, muduo::Timestamp time);
    void write(std::string& message);
    void connect();
    void disconnect();
private:
    muduo::net::TcpClient client_;
    LengthCodec codec_;
    std::mutex mutex_;
    muduo::net::TcpConnectionPtr connection_;
};

#endif