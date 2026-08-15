#include "Poller.h"
#include "Channel.h"

#include <cerrno>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

namespace {

void updateEpoll(int epoll_fd, int operation, Channel* channel)
{
    epoll_event event{};
    event.events = channel->events();
    event.data.ptr = channel;

    if (::epoll_ctl(epoll_fd, operation, channel->fd(), &event) == -1) {
        throw std::runtime_error("epoll_ctl failed: " +
                                 std::to_string(errno));
    }
}

} // namespace

Poller::Poller()
    : epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)), events_(16)
{
    if (epoll_fd_ == -1) {
        throw std::runtime_error("epoll_create1 failed");
    }
}

Poller::~Poller()
{
    ::close(epoll_fd_);
}

void Poller::poll(std::vector<Channel*>& active_channels)
{
    int ready_count;
    do {
        ready_count = ::epoll_wait(epoll_fd_, events_.data(),
                                   static_cast<int>(events_.size()), -1);
    } while (ready_count == -1 && errno == EINTR);

    if (ready_count == -1) {
        throw std::runtime_error("epoll_wait failed");
    }

    for (int i = 0; i < ready_count; ++i) {
        auto* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->setReturnedEvents(events_[i].events);
        active_channels.push_back(channel);
    }

    if (ready_count == static_cast<int>(events_.size())) {
        events_.resize(events_.size() * 2);
    }
}

void Poller::updateChannel(Channel* channel)
{
    if (!channel->isAdded()) {
        if (channel->isNoneEvent()) {
            return;
        }
        updateEpoll(epoll_fd_, EPOLL_CTL_ADD, channel);
        channel->setAdded(true);
        return;
    }

    if (channel->isNoneEvent()) {
        removeChannel(channel);
    } else {
        updateEpoll(epoll_fd_, EPOLL_CTL_MOD, channel);
    }
}

void Poller::removeChannel(Channel* channel)
{
    if (!channel->isAdded()) {
        return;
    }

    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel->fd(), nullptr) == -1 &&
        errno != ENOENT && errno != EBADF) {
        throw std::runtime_error("epoll_ctl delete failed: " +
                                 std::to_string(errno));
    }
    channel->setAdded(false);
}
