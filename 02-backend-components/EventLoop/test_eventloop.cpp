#include "Channel.h"
#include "EventLoop.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <unistd.h>
#include <vector>

void testReadableEvent()
{
    int pipe_fds[2];
    assert(::pipe(pipe_fds) == 0);

    EventLoop loop;
    Channel channel(&loop, pipe_fds[0]);
    bool callback_called = false;

    channel.setReadCallback([&] {
        char value;
        assert(::read(pipe_fds[0], &value, sizeof(value)) == 1);
        assert(value == 'x');
        callback_called = true;
        loop.quit();
    });
    channel.enableReading();

    const char value = 'x';
    assert(::write(pipe_fds[1], &value, sizeof(value)) == 1);
    loop.loop();

    assert(callback_called);
    channel.remove();
    ::close(pipe_fds[0]);
    ::close(pipe_fds[1]);
}

void testQueueTasksFromOtherThreads()
{
    constexpr int producer_count = 4;
    constexpr int tasks_per_producer = 50;
    constexpr int total_tasks = producer_count * tasks_per_producer;

    std::promise<EventLoop*> loop_ready;
    std::atomic<int> executed{0};

    std::thread loop_thread([&] {
        EventLoop loop;
        loop_ready.set_value(&loop);
        loop.loop();
    });

    EventLoop* loop = loop_ready.get_future().get();
    std::vector<std::thread> producers;

    for (int i = 0; i < producer_count; ++i) {
        producers.emplace_back([loop, &executed] {
            for (int j = 0; j < tasks_per_producer; ++j) {
                loop->queueInLoop([loop, &executed] {
                    if (++executed == total_tasks) {
                        loop->quit();
                    }
                });
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }
    loop_thread.join();

    assert(executed == total_tasks);
}

void testRunInLoopUsesOwnerThread()
{
    std::promise<EventLoop*> loop_ready;
    std::promise<std::thread::id> callback_thread;

    std::thread loop_thread([&] {
        EventLoop loop;
        loop_ready.set_value(&loop);
        loop.loop();
    });

    EventLoop* loop = loop_ready.get_future().get();
    loop->runInLoop([&] {
        callback_thread.set_value(std::this_thread::get_id());
        loop->quit();
    });

    assert(callback_thread.get_future().get() == loop_thread.get_id());
    loop_thread.join();
}

void testQuitWakesIdleLoop()
{
    std::promise<EventLoop*> loop_ready;

    std::thread loop_thread([&] {
        EventLoop loop;
        loop_ready.set_value(&loop);
        loop.loop();
    });

    EventLoop* loop = loop_ready.get_future().get();
    const auto start = std::chrono::steady_clock::now();
    loop->quit();
    loop_thread.join();

    const auto elapsed = std::chrono::steady_clock::now() - start;
    assert(elapsed < std::chrono::seconds(1));
}

void testRejectLoopFromWrongThread()
{
    EventLoop loop;
    bool thrown = false;

    std::thread thread([&] {
        try {
            loop.loop();
        } catch (const std::runtime_error&) {
            thrown = true;
        }
    });
    thread.join();

    assert(thrown);
}

int main()
{
    testReadableEvent();
    testQueueTasksFromOtherThreads();
    testRunInLoopUsesOwnerThread();
    testQuitWakesIdleLoop();
    testRejectLoopFromWrongThread();

    std::cout << "All EventLoop tests passed.\n";
    return 0;
}
