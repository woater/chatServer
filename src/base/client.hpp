#ifndef CHATSERVER_BASE_CLIENT_HPP__
#define CHATSERVER_BASE_CLIENT_HPP__

#include "muduo/net/TcpClient.h"
#include "muduo/base/Logging.h"

class ChatClient {
public:
    ChatClient(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& serverAddress,
            const std::string& nameArg);
    ChatClient(const ChatClient&) = delete;
    ChatClient(const ChatClient&&) = delete;
    ChatClient& operator=(const ChatClient&) = delete;
    ChatClient& operator=(const ChatClient&&) = delete;

    void onMessage();
private:
    muduo::net::TcpClient client_;
};

#endif