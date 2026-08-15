#pragma once

#include <functional>

class EventLoop;

class Channel
{
public:
    using EventCallback = std::function<void()>;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;

    void handleEvent() const;
    void setReadCallback(EventCallback callback);
    void setWriteCallback(EventCallback callback);
    void setErrorCallback(EventCallback callback);

    void enableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();
    void remove();

    int fd() const noexcept;
    unsigned int events() const noexcept;
    void setReturnedEvents(unsigned int events) noexcept;
    bool isNoneEvent() const noexcept;
    bool isAdded() const noexcept;
    void setAdded(bool added) noexcept;

private:
    void update();

    EventLoop* loop_;
    const int fd_;
    unsigned int events_ = 0;
    unsigned int returned_events_ = 0;
    bool added_ = false;
    EventCallback read_callback_;
    EventCallback write_callback_;
    EventCallback error_callback_;
};
