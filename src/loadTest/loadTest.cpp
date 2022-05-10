#include "muduo/net/TcpClient.h"
#include "muduo/base/Logging.h"
#include <memory>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/EventLoop.h>
#include <mutex>
#include <atomic>
#include "lengthCodec.hpp"
#include <functional>
#include <unistd.h>

int g_connections = 0;
std::atomic<int32_t> g_aliveConnections;
std::atomic<int32_t> g_messageRecieved;
muduo::Timestamp g_startTime;
std::vector<muduo::Timestamp> g_recieveTime;
muduo::net::EventLoop* g_loop;
std::function<void()> g_statistic;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class ChatClient {
public:
    ChatClient(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& serverAddress)
    :   loop_(loop),
        client_(loop, serverAddress, "loadTestClient"),
        codec_(std::bind(&ChatClient::onStringMessage, this, _1, _2, _3)) 
    {
        client_.setConnectionCallback(
            std::bind(&ChatClient::onConnection, this, _1));
        client_.setMessageCallback(
            std::bind(&LengthCodec::onMessage, &codec_, _1, _2, _3));
    }

    ChatClient(const ChatClient&) = delete;
    ChatClient(const ChatClient&&) = delete;
    ChatClient& operator=(const ChatClient&) = delete;
    ChatClient& operator=(const ChatClient&&) = delete;

    void connect() {
        client_.connect();
    }
    void disconnect() {
        // client_.disconnect();
    }

    void onConnection(const muduo::net::TcpConnectionPtr& connection) {
        LOG_INFO << "connection from " << connection->peerAddress().toIpPort()
                << " is " << (connection->connected() ? "UP" : "DOWN");
        if (connection->connected()) {
            connection_ = connection;
            if (g_aliveConnections.fetch_add(1, std::memory_order_acq_rel) == g_connections - 1) {
                LOG_INFO << "connection all developed";
                loop_->runAfter(10, std::bind(&ChatClient::send, this));
            }
        }
        
    }
    void onStringMessage(const muduo::net::TcpConnectionPtr& connection, const std::string& message, muduo::Timestamp time) {
        (void)message;
        receiveTime_ = loop_->pollReturnTime();
        int received = g_messageRecieved.fetch_add(1, std::memory_order_acq_rel);
        if (received == g_connections - 1) {
            muduo::Timestamp endTime = muduo::Timestamp::now();
            LOG_INFO << "all received " << g_connections << " in "
                    << muduo::timeDifference(endTime, g_startTime);
            g_loop->queueInLoop(g_statistic);
        } else if (received % 1000 == 0) {
            LOG_DEBUG << received;
        }
    }

    void send()
    {
        g_startTime = muduo::Timestamp::now();
        codec_.send(connection_, "hello");
        LOG_DEBUG << "sent";
    }

    muduo::Timestamp receiveTime() const { 
        return receiveTime_; 
    }

    
private:
    muduo::net::EventLoop* loop_;
    muduo::net::TcpClient client_;
    LengthCodec codec_;
    std::mutex mutex_;
    muduo::net::TcpConnectionPtr connection_;
    muduo::Timestamp receiveTime_;
};

void statistic(const std::vector<std::unique_ptr<ChatClient>>& clients) {
    LOG_INFO << "statistic " << clients.size();
    std::vector<double> seconds(clients.size());
    for (size_t i = 0; i < clients.size(); ++i) {
        seconds[i] = muduo::timeDifference(clients[i]->receiveTime(), g_startTime);
    }

    std::sort(seconds.begin(), seconds.end());
    for (size_t i = 0; i < clients.size(); i += std::max(static_cast<size_t>(1), clients.size() / 10)) {
        printf("%6zd%% %.6lf\n", i * 100 / clients.size(), seconds[i]);
    }
    if (clients.size() >= 100) {
        printf("%6d%% %.6lf\n", 99, seconds[clients.size() - clients.size()/100]);
    }
    printf("%6d%% %.6lf\n", 100, seconds.back());
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        fprintf(stderr, "usage: %s <ip> <port> <connections> [threads]\n", argv[0]);
        return -1;
    }
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    muduo::net::InetAddress serverAddress(argv[1], port);
    g_connections = atoi(argv[3]);
    int threadNum = 0;
    if (argc > 4) {
        threadNum = atoi(argv[4]);
    }

    muduo::net::EventLoop loop;
    g_loop = &loop;
    muduo::net::EventLoopThreadPool loopPool(&loop, "loadTest");
    loopPool.setThreadNum(threadNum);
    loopPool.start();

    g_recieveTime.reserve(g_connections);
    std::vector<std::unique_ptr<ChatClient>> clients(g_connections);
    g_statistic = std::bind(statistic, std::ref(clients));

    for (int i = 0; i < g_connections; ++i) {
        clients[i].reset(new ChatClient(loopPool.getNextLoop(), serverAddress));
        clients[i]->connect();
        usleep(200); // 防止大量SYN导致丢包
    }
    
    loop.loop();
}
