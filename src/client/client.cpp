#include "client.hpp"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>
#include <functional>
#include <iostream>
#include <mutex>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

ChatClient::ChatClient(muduo::net::EventLoop* loop,
        const muduo::net::InetAddress& serverAddress)
: client_(loop, serverAddress, "ChatClient") {
    client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&ChatClient::onMessage, this, _1, _2, _3));
}

void ChatClient::onConnection(const muduo::net::TcpConnectionPtr& connection) {
    LOG_INFO << "new connection from " << connection->peerAddress().toIpPort();
    // 需要将连接保存以进行数据发送
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (connection->connected()) {
            connection_ = connection;
        } else {
            connection_.reset();
        }
    }
    
}

void ChatClient::onMessage(const muduo::net::TcpConnectionPtr& connection, muduo::net::Buffer* buffer, muduo::Timestamp time) {
    std::string msg(buffer->retrieveAllAsString());
    LOG_INFO << "ChatClient - " << connection->localAddress().toIpPort() << " recieved at " << time.toString() << " with " << msg.size() << " bytes data";
    printf("%s\n", msg.c_str());
}

void ChatClient::write(std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    // 需要检查connecion_是否存在（客户端程序存在未连接的状态）
    // 如果connection_为空指针，会报段错误SIGSEGV
    if (connection_) {
        connection_->send(message.c_str(), message.size());
    }
}

void ChatClient::connect(){
    client_.connect();
}
void ChatClient::disconnect() {
    client_.disconnect();
}