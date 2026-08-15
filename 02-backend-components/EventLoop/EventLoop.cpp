#include "EventLoop.h"
#include "Channel.h"

#include <cerrno>
#include <cstdint>
#include <stdexcept>
#include <sys/eventfd.h>
#include <unistd.h>

EventLoop::EventLoop()
    : owner_thread_id_(std::this_thread::get_id()),
      wakeup_fd_(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))
{
    if (wakeup_fd_ == -1) {
        throw std::runtime_error("eventfd failed");
    }

    try {
        wakeup_channel_ = std::make_unique<Channel>(this, wakeup_fd_);
        wakeup_channel_->setReadCallback([this] { handleWakeup(); });
        wakeup_channel_->enableReading();
    } catch (...) {
        ::close(wakeup_fd_);
        throw;
    }
}

EventLoop::~EventLoop()
{
    wakeup_channel_.reset();
    ::close(wakeup_fd_);
}

void EventLoop::loop()
{
    if (!isInLoopThread()) {
        throw std::runtime_error("EventLoop must run in its owner thread");
    }

    while (!quit_.load()) {
        std::vector<Channel*> active_channels;
        poller_.poll(active_channels);

        for (Channel* channel : active_channels) {
            channel->handleEvent();
        }

        executePendingTasks();
    }
}

void EventLoop::quit() noexcept
{
    quit_.store(true);
    wakeup();
}

void EventLoop::runInLoop(Functor task)
{
    if (isInLoopThread()) {
        task();
    } else {
        queueInLoop(std::move(task));
    }
}

void EventLoop::queueInLoop(Functor task)
{
    {
        std::lock_guard<std::mutex> lock(task_mutex_);
        pending_tasks_.push_back(std::move(task));
    }

    if (!isInLoopThread() || executing_pending_tasks_) {
        wakeup();
    }
}

bool EventLoop::isInLoopThread() const noexcept
{
    return owner_thread_id_ == std::this_thread::get_id();
}

void EventLoop::updateChannel(Channel* channel)
{
    poller_.updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    poller_.removeChannel(channel);
}

void EventLoop::wakeup() noexcept
{
    const std::uint64_t value = 1;
    ssize_t written;
    do {
        written = ::write(wakeup_fd_, &value, sizeof(value));
    } while (written == -1 && errno == EINTR);
}

void EventLoop::handleWakeup() noexcept
{
    std::uint64_t value;
    ssize_t read_count;
    do {
        read_count = ::read(wakeup_fd_, &value, sizeof(value));
    } while (read_count == -1 && errno == EINTR);
}

void EventLoop::executePendingTasks()
{
    std::vector<Functor> tasks;
    executing_pending_tasks_ = true;

    {
        std::lock_guard<std::mutex> lock(task_mutex_);
        tasks.swap(pending_tasks_);
    }

    for (auto& task : tasks) {
        task();
    }

    executing_pending_tasks_ = false;
}
