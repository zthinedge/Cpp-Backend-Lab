#include "ConnectionPool.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

void testAcquireConnection()
{
    MySqlConnectionPool pool(2);

    auto conn = pool.acquire();

    assert(conn != nullptr);
    assert(conn->isConnected());
    assert(conn->execute("select 1"));
}

void testConnectionAutoReturn()
{
    MySqlConnectionPool pool(1);

    std::size_t first_id = 0;
    {
        auto conn = pool.acquire();
        assert(conn != nullptr);
        first_id = conn->id();
    }

    auto conn = pool.acquire();
    assert(conn != nullptr);
    assert(conn->id() == first_id);
}

void testConcurrentAcquire()
{
    MySqlConnectionPool pool(3);

    constexpr int thread_count = 10;
    std::atomic<int> success_count{0};
    std::vector<std::thread> threads;

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&] {
            auto conn = pool.acquire();
            assert(conn != nullptr);
            assert(conn->execute("select 1"));

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            ++success_count;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    assert(success_count == thread_count);
}

void testAcquireBlocksUntilConnectionReturned()
{
    MySqlConnectionPool pool(1);

    auto first_conn = pool.acquire();
    assert(first_conn != nullptr);

    std::atomic<bool> acquired{false};
    std::thread waiter([&] {
        auto second_conn = pool.acquire();
        assert(second_conn != nullptr);
        acquired = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    assert(!acquired);

    first_conn.reset();

    waiter.join();
    assert(acquired);
}

void testRejectZeroPoolSize()
{
    bool thrown = false;

    try {
        MySqlConnectionPool pool(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }

    assert(thrown);
}

int main()
{
    testAcquireConnection();
    testConnectionAutoReturn();
    testConcurrentAcquire();
    testAcquireBlocksUntilConnectionReturned();
    testRejectZeroPoolSize();

    std::cout << "All ConnectionPool tests passed.\n";
    return 0;
}
