#pragma once
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <stdexcept>
#include "Connection.h"
class MySqlConnectionPool{
public:
    MySqlConnectionPool(size_t pool_size);
    ~MySqlConnectionPool();

    MySqlConnectionPool(const MySqlConnectionPool&)=delete;
    MySqlConnectionPool& operator=(const MySqlConnectionPool&)=delete;
    std::shared_ptr<MySqlConnection> acquire();

private:
    std::queue<std::shared_ptr<MySqlConnection>>connections_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopping_ =false;
};