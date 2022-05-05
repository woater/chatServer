#include "lengthCodec.hpp"
#include <muduo/net/Buffer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Endian.h>

LengthCodec::LengthCodec(const StringMessageCallback& cb) 
: messageCallback_(cb){
}

void LengthCodec::onMessage(const muduo::net::TcpConnectionPtr& connection, muduo::net::Buffer* buffer, const muduo::Timestamp& time) {
    while (buffer->readableBytes() >= lengthHeaderLen) {
        int32_t netLength = buffer->peekInt32();
        int32_t length = muduo::net::sockets::networkToHost32(netLength);
        if (length > 65536 || length < 0) {
            LOG_ERROR << "Invalid length: " << length;
            connection->shutdown();
            break;
        } else if (buffer->readableBytes() >= length + lengthHeaderLen) {
            buffer->retrieve(lengthHeaderLen);
            std::string message(buffer->peek(), length);
            buffer->retrieve(length);
            messageCallback_(connection, message, time);
        } else {
            break;
        }
    }
}

void LengthCodec::send(const muduo::net::TcpConnectionPtr& connection, const std::string& message) {
    muduo::net::Buffer buffer;
    buffer.append(message.data(), message.size());
    
    int32_t length = static_cast<int32_t>(message.size());
    int32_t netLength = muduo::net::sockets::hostToNetwork32(length);

    buffer.prependInt32(netLength);
    connection->send(&buffer);
}
