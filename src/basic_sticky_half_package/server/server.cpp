#include "server.hpp"
#include <muduo/base/Logging.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <functional>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

// 构造函数
ChatServer::ChatServer(muduo::net::EventLoop* loop,
    const muduo::net::InetAddress& listenAddr)
: server_(loop, listenAddr, "ChatServer"), 
codec_(std::bind(&ChatServer::onStringMessage, this, _1, _2, _3)) {
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    server_.setMessageCallback(std::bind(&LengthCodec::onMessage, &codec_, _1, _2, _3));
}

// 处理连接（新连接或断开）
void ChatServer::onConnection(const muduo::net::TcpConnectionPtr& connection) {
    // log related information
    LOG_INFO << "ChatServer - " << connection->peerAddress().toIpPort()
            << " -> " << connection->localAddress().toIpPort() << " is "
            << (connection->connected() ? "UP" : "DOWN");
    // connect
    if (connection->connected()) {
        connections_.insert(connection);
    // disconnect
    } else {
        connections_.erase(connection);
    }
}


// 处理新接收到的消息
void ChatServer::onStringMessage(const muduo::net::TcpConnectionPtr& connection, const std::string& msg, muduo::Timestamp time) {
    LOG_INFO << "ChatServer - " << "recieved " << " from " << connection->peerAddress().toIpPort()
            << " at " << time.toString() << " "<< msg.size() << " bytes data " ;

    for (auto conn : connections_) {
        codec_.send(conn, msg);
    } 
}

void ChatServer::start() {
    server_.start();
}