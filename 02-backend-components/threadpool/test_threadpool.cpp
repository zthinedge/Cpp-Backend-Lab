#include "ThreadPool.h"

#include <atomic>
#include <cassert>
#include <iostream>
void testExecuteAllTasks()
{
    ThreadPool pool(4);

    constexpr int task_count = 100;
    std::atomic<int> counter{0};
    std::mutex mutex;
    std::condition_variable cv;
    int finished = 0;

    for (int i = 0; i < task_count; ++i) {
        pool.enqueue([&] {
            ++counter;

            {
                std::lock_guard<std::mutex> lock(mutex);
                ++finished;
            }
            cv.notify_one();
        });
    }

    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&] {
        return finished == task_count;
    });

    assert(counter == task_count);
}

void testDestructorWaitsForQueuedTasks()
{
    constexpr int task_count = 20;
    std::atomic<int> counter{0};

    {
        ThreadPool pool(2);

        for (int i = 0; i < task_count; ++i) {
            pool.enqueue([&] {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                ++counter;
            });
        }
    }

    assert(counter == task_count);
}

void testRejectZeroThreadCount()
{
    bool thrown = false;

    try {
        ThreadPool pool(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }

    assert(thrown);
}

int main()
{
    testExecuteAllTasks();
    testDestructorWaitsForQueuedTasks();
    testRejectZeroThreadCount();

    std::cout << "All ThreadPool tests passed.\n";
    return 0;
}
