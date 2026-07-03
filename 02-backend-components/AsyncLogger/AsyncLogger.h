#pragma once
#include "../BlockQueue/BlockQueue.h"
#include <thread>
#include <fstream>
#include <string>

class AsyncLogger
{
public:
    explicit AsyncLogger(const std::string&filename, size_t queue_capacity = 1024);
    ~AsyncLogger();

    AsyncLogger(const AsyncLogger&) = delete;
    AsyncLogger& operator=(const AsyncLogger&) = delete;

    void info(const std::string&msg);
    void warn(const std::string&msg);
    void error(const std::string&msg);

private:
    void log(const std::string& level, const std::string& msg);
    void workerLoop();

private:
    BlockQueue<std::string>queue_;
    std::thread worker_;    //后台消费者线程
    std::ofstream file_;
};

