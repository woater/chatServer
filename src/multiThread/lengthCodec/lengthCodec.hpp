#ifndef CHATSERVER_BASIC_STICKY_HALF_PACKAGE_LENGTHCODEC_HPP__
#define CHATSERVER_BASIC_STICKY_HALF_PACKAGE_LENGTHCODEC_HPP__

#include <functional>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>

class LengthCodec {
public:
    typedef std::function<void (const muduo::net::TcpConnectionPtr&, const std::string&, muduo::Timestamp)> StringMessageCallback;
    explicit LengthCodec(const StringMessageCallback& cb);
    
    LengthCodec(const LengthCodec&) = delete;
    LengthCodec(const LengthCodec&&) = delete;
    LengthCodec& operator=(const LengthCodec&) = delete;
    LengthCodec& operator=(const LengthCodec&&) = delete;

    void onMessage(const muduo::net::TcpConnectionPtr& connection, muduo::net::Buffer* buffer, const muduo::Timestamp& time);
    void send(const muduo::net::TcpConnectionPtr& connection, const std::string message); // pass message by value for thread-safety
private:
    StringMessageCallback messageCallback_;
    const static size_t lengthHeaderLen = sizeof(int32_t);
};


#endif