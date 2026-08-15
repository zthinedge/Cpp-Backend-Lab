#pragma once

#include <sys/epoll.h>
#include <vector>

class Channel;

class Poller
{
public:
    Poller();
    ~Poller();

    Poller(const Poller&) = delete;
    Poller& operator=(const Poller&) = delete;

    void poll(std::vector<Channel*>& active_channels);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

private:
    int epoll_fd_;
    std::vector<struct epoll_event> events_;
};
