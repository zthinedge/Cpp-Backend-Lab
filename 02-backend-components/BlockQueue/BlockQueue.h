#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class BlockQueue
{
public:
    explicit BlockQueue(size_t capacity);
    ~BlockQueue();

    void push(T value);
    bool pop(T &value);
    void close();
    bool empty() const;
    size_t size() const;

private:
    std::queue<T> queue_;
    size_t capacity_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    bool closed_ = false;
};

template <typename T>
BlockQueue<T>::BlockQueue(size_t capacity)
    : capacity_(capacity)
{
    if (capacity_ == 0)
    {
        throw std::invalid_argument("capacity must be greater than 0");
    }
}

template <typename T>
BlockQueue<T>::~BlockQueue()
{
    close();
}

template <typename T>
void BlockQueue<T>::push(T value)
{
    std::unique_lock<std::mutex> lock(mutex_);

    not_full_.wait(lock, [this]
                   { return closed_ || queue_.size() < capacity_; });

    if (closed_)
    {
        throw std::runtime_error("push on closed BlockQueue");
    }

    queue_.push(std::move(value));
    not_empty_.notify_one();
}

template <typename T>
bool BlockQueue<T>::pop(T &value)
{
    std::unique_lock<std::mutex> lock(mutex_);

    not_empty_.wait(lock, [this]
                    { return closed_ || !queue_.empty(); });

    if (queue_.empty())
    {
        return false;
    }

    value = std::move(queue_.front());
    queue_.pop();
    not_full_.notify_one();

    return true;
}

template <typename T>
void BlockQueue<T>::close()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }

    not_empty_.notify_all();
    not_full_.notify_all();
}

template <typename T>
bool BlockQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

template <typename T>
size_t BlockQueue<T>::size() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}
