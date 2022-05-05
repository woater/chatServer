#include "server.hpp"
#include <cassert>
#include <muduo/base/Logging.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <functional>
#include <mutex>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

// 构造函数
ChatServer::ChatServer(muduo::net::EventLoop* loop,
    const muduo::net::InetAddress& listenAddr)
: server_(loop, listenAddr, "ChatServer"), 
codec_(std::bind(&ChatServer::onStringMessage, this, _1, _2, _3)),
connections_(new ConnectionSet) {
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    server_.setMessageCallback(std::bind(&LengthCodec::onMessage, &codec_, _1, _2, _3));
}

// 处理连接（新连接或断开）
void ChatServer::onConnection(const muduo::net::TcpConnectionPtr& connection) {
    // log related information
    LOG_INFO << "ChatServer - " << connection->peerAddress().toIpPort()
            << " -> " << connection->localAddress().toIpPort() << " is "
            << (connection->connected() ? "UP" : "DOWN");

    {
    std::lock_guard<std::mutex> lock(mutexOfConnections_);
        if (! connections_.unique()) {
            connections_.reset(new ConnectionSet(*connections_));
        }

        if (connection->connected()) {
            connections_->insert(connection);
        } else {
            connections_->erase(connection);
        }
    }
    
}


// 处理新接收到的消息
void ChatServer::onStringMessage(const muduo::net::TcpConnectionPtr& connection, const std::string& msg, muduo::Timestamp time) {
    LOG_INFO << "ChatServer - " << "recieved " << " from " << connection->peerAddress().toIpPort()
            << " at " << time.toString() << " "<< msg.size() << " bytes data " ;
    {
        ConnectionSetPtr connections = getConnectionSet();
        for (auto conn : *connections) {
            codec_.send(conn, msg);
        }
    } 
}

ChatServer::ConnectionSetPtr ChatServer::getConnectionSet() {
    std::lock_guard<std::mutex> lock(mutexOfConnections_);
    return connections_;
}

void ChatServer::setThreadNum(int numThreads) {
    server_.setThreadNum(numThreads);
}

void ChatServer::start() {
    server_.start();
}