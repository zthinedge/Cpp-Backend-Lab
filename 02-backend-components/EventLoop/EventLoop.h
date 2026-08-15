#pragma once

#include "Poller.h"

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class Channel;

class EventLoop
{
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;

    void loop();
    void quit() noexcept;
    void runInLoop(Functor task);
    void queueInLoop(Functor task);
    bool isInLoopThread() const noexcept;

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

private:
    void wakeup() noexcept;
    void handleWakeup() noexcept;
    void executePendingTasks();

    Poller poller_;
    std::atomic<bool> quit_{false};
    const std::thread::id owner_thread_id_;
    int wakeup_fd_;
    std::unique_ptr<Channel> wakeup_channel_;

    std::mutex task_mutex_;
    std::vector<Functor> pending_tasks_;
    bool executing_pending_tasks_ = false;
};
