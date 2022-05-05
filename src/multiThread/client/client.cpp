#include "client.hpp"
#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
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
: client_(loop, serverAddress, "ChatClient"),
codec_(std::bind(&ChatClient::onStringMessage, this, _1, _2, _3)) {
    client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&LengthCodec::onMessage, &codec_, _1, _2, _3));
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

void ChatClient::onStringMessage(const muduo::net::TcpConnectionPtr& connection, const std::string& message, muduo::Timestamp time) {
    LOG_INFO << "ChatClient - " << connection->localAddress().toIpPort() << " recieved at " << time.toString() << " with " << message.size() << " bytes data";
    printf(">>> %s\n", message.c_str());
}

void ChatClient::write(std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    // 需要检查connecion_是否存在（客户端程序存在未连接的状态）
    // 如果connection_为空指针，会报段错误SIGSEGV
    if (connection_) {
        codec_.send(connection_, message);
    }
}

void ChatClient::connect(){
    client_.connect();
}
void ChatClient::disconnect() {
    client_.disconnect();
}