#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <thread>
#include <condition_variable>

class ThreadPool
{
private:
    std::vector<std::thread>workers_;
    std::queue<std::function<void()>>tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopping_=false;
    
public:
    explicit ThreadPool(size_t thread_count);
    ~ThreadPool();

    ThreadPool(const ThreadPool&)=delete;
    ThreadPool operator=(const ThreadPool&)=delete;

    void enqueue(std::function<void()>task);

private:
    void workerLoop();

};



