#include "Channel.h"
#include "EventLoop.h"

#include <sys/epoll.h>

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd)
{
}

Channel::~Channel()
{
    if (added_) {
        try {
            remove();
        } catch (...) {
            // 析构函数不能让清理阶段的异常终止程序。
        }
    }
}

void Channel::handleEvent() const
{
    if ((returned_events_ & (EPOLLERR | EPOLLHUP)) && error_callback_) {
        error_callback_();
    }

    if ((returned_events_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) &&
        read_callback_) {
        read_callback_();
    }

    if ((returned_events_ & EPOLLOUT) && write_callback_) {
        write_callback_();
    }
}

void Channel::setReadCallback(EventCallback callback)
{
    read_callback_ = std::move(callback);
}

void Channel::setWriteCallback(EventCallback callback)
{
    write_callback_ = std::move(callback);
}

void Channel::setErrorCallback(EventCallback callback)
{
    error_callback_ = std::move(callback);
}

void Channel::enableReading()
{
    events_ |= EPOLLIN | EPOLLPRI;
    update();
}

void Channel::enableWriting()
{
    events_ |= EPOLLOUT;
    update();
}

void Channel::disableWriting()
{
    events_ &= ~static_cast<unsigned int>(EPOLLOUT);
    update();
}

void Channel::disableAll()
{
    events_ = 0;
    update();
}

void Channel::remove()
{
    loop_->removeChannel(this);
}

int Channel::fd() const noexcept
{
    return fd_;
}

unsigned int Channel::events() const noexcept
{
    return events_;
}

void Channel::setReturnedEvents(unsigned int events) noexcept
{
    returned_events_ = events;
}

bool Channel::isNoneEvent() const noexcept
{
    return events_ == 0;
}

bool Channel::isAdded() const noexcept
{
    return added_;
}

void Channel::setAdded(bool added) noexcept
{
    added_ = added;
}

void Channel::update()
{
    loop_->updateChannel(this);
}
