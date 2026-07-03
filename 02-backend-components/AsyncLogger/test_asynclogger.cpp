#include "AsyncLogger.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

std::string readFile(const std::string& filename)
{
    std::ifstream file(filename);
    std::string content;
    std::string line;

    while (std::getline(file, line)) {
        content += line;
        content += '\n';
    }

    return content;
}

int countLines(const std::string& filename)
{
    std::ifstream file(filename);
    int count = 0;
    std::string line;

    while (std::getline(file, line)) {
        ++count;
    }

    return count;
}

void testWriteBasicLogs()
{
    const std::string filename = "asynclogger_basic_test.log";
    std::remove(filename.c_str());

    {
        AsyncLogger logger(filename, 8);

        logger.info("server started");
        logger.warn("queue almost full");
        logger.error("mysql query failed");
    }

    const std::string content = readFile(filename);

    assert(content.find("[INFO] server started") != std::string::npos);
    assert(content.find("[WARN] queue almost full") != std::string::npos);
    assert(content.find("[ERROR] mysql query failed") != std::string::npos);

    std::remove(filename.c_str());
}

void testConcurrentLogging()
{
    const std::string filename = "asynclogger_concurrent_test.log";
    std::remove(filename.c_str());

    constexpr int thread_count = 4;
    constexpr int logs_per_thread = 25;
    constexpr int total_logs = thread_count * logs_per_thread;

    {
        AsyncLogger logger(filename, 5);
        std::vector<std::thread> threads;

        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back([&logger, i] {
                for (int j = 0; j < logs_per_thread; ++j) {
                    logger.info("thread " + std::to_string(i) +
                                " log " + std::to_string(j));
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    assert(countLines(filename) == total_logs);

    std::remove(filename.c_str());
}

void testOpenInvalidFileThrows()
{
    bool thrown = false;

    try {
        AsyncLogger logger("/not_exist_dir/asynclogger.log");
    } catch (const std::runtime_error&) {
        thrown = true;
    }

    assert(thrown);
}

int main()
{
    testWriteBasicLogs();
    testConcurrentLogging();
    testOpenInvalidFileThrows();

    std::cout << "All AsyncLogger tests passed.\n";
    return 0;
}
