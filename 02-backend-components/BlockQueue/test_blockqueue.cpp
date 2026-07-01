#include "BlockQueue.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

void testPushAndPop()
{
    BlockQueue<int> queue(2);

    queue.push(1);
    queue.push(2);

    int value = 0;
    assert(queue.pop(value));
    assert(value == 1);

    assert(queue.pop(value));
    assert(value == 2);

    assert(queue.empty());
}

void testPopBlocksUntilPush()
{
    BlockQueue<int> queue(1);
    std::atomic<bool> popped{false};

    std::thread consumer([&] {
        int value = 0;
        assert(queue.pop(value));
        assert(value == 42);
        popped = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    assert(!popped);

    queue.push(42);

    consumer.join();
    assert(popped);
}

void testPushBlocksUntilPop()
{
    BlockQueue<int> queue(1);
    queue.push(1);

    std::atomic<bool> pushed{false};

    std::thread producer([&] {
        queue.push(2);
        pushed = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    assert(!pushed);

    int value = 0;
    assert(queue.pop(value));
    assert(value == 1);

    producer.join();
    assert(pushed);

    assert(queue.pop(value));
    assert(value == 2);
}

void testMultiProducerConsumer()
{
    BlockQueue<int> queue(5);

    constexpr int producer_count = 4;
    constexpr int items_per_producer = 25;
    constexpr int total_items = producer_count * items_per_producer;

    std::atomic<int> produced{0};
    std::atomic<int> consumed{0};
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 0; i < 2; ++i) {
        consumers.emplace_back([&] {
            int value = 0;
            while (queue.pop(value)) {
                ++consumed;
            }
        });
    }

    for (int i = 0; i < producer_count; ++i) {
        producers.emplace_back([&] {
            for (int j = 0; j < items_per_producer; ++j) {
                queue.push(j);
                ++produced;
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }

    queue.close();

    for (auto& consumer : consumers) {
        consumer.join();
    }

    assert(produced == total_items);
    assert(consumed == total_items);
}

void testCloseWakesPop()
{
    BlockQueue<int> queue(1);
    std::atomic<bool> returned{false};

    std::thread consumer([&] {
        int value = 0;
        assert(!queue.pop(value));
        returned = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    assert(!returned);

    queue.close();

    consumer.join();
    assert(returned);
}

void testPushAfterCloseThrows()
{
    BlockQueue<int> queue(1);
    queue.close();

    bool thrown = false;
    try {
        queue.push(1);
    } catch (const std::runtime_error&) {
        thrown = true;
    }

    assert(thrown);
}

void testRejectZeroCapacity()
{
    bool thrown = false;

    try {
        BlockQueue<int> queue(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }

    assert(thrown);
}

int main()
{
    testPushAndPop();
    testPopBlocksUntilPush();
    testPushBlocksUntilPop();
    testMultiProducerConsumer();
    testCloseWakesPop();
    testPushAfterCloseThrows();
    testRejectZeroCapacity();

    std::cout << "All BlockQueue tests passed.\n";
    return 0;
}
