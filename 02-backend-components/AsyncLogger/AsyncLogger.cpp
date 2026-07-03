#include "AsyncLogger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
//命名空间里面的函数只能在当前文件调用
namespace {

std::string currentTimeString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
    localtime_r(&time, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

}

AsyncLogger::AsyncLogger(const std::string&filename, size_t queue_capacity)
    : queue_(queue_capacity),
      file_(filename, std::ios::app)//文件打开模式--追加写入
{
    if (!file_.is_open()) {
        throw std::runtime_error("failed to open log file: " + filename);
    }
    //启动后台线程
    worker_ = std::thread(&AsyncLogger::workerLoop, this);
}

AsyncLogger::~AsyncLogger()
{
    queue_.close();

    if (worker_.joinable()) {
        worker_.join();
    }
    //把缓冲区里的日志强制刷到文件
    file_.flush();
}

void AsyncLogger::info(const std::string&msg)
{
    log("INFO", msg);
}

void AsyncLogger::warn(const std::string&msg)
{
    log("WARN", msg);
}

void AsyncLogger::error(const std::string&msg)
{
    log("ERROR", msg);
}

void AsyncLogger::log(const std::string& level, const std::string& msg)
{
    //字符串拼接工具
    std::ostringstream oss;
    oss << '[' << currentTimeString() << "] "
        << '[' << level << "] "
        << msg;

    queue_.push(oss.str());
}

void AsyncLogger::workerLoop()
{
    std::string line;
    while (queue_.pop(line)) {
        //把日志写入文件
        file_ << line << '\n';
    }
}
