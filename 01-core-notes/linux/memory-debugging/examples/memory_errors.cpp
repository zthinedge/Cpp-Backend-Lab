#include <cstdint>
#include <iostream>
#include <limits>
#include <string>

namespace {

volatile std::int64_t g_sink = 0;

void memory_leak() {
    int* numbers = new int[1024];
    numbers[0] = 42;
    g_sink = numbers[0];

    // 故意不调用 delete[]，用于演示内存泄漏。
}

void heap_buffer_overflow() {
    int* numbers = new int[4];

    // 有效下标是 0、1、2、3；写入 numbers[4] 会越过堆缓冲区边界。
    for (int index = 0; index <= 4; ++index) {
        numbers[index] = index;
    }

    g_sink = numbers[0];
    delete[] numbers;
}

void use_after_free() {
    int* number = new int(42);
    delete number;

    // number 指向的内存已经释放，此后再次读取是未定义行为。
    g_sink = *number;
}

void uninitialized_read() {
    int* number = new int;

    // 分配的 int 没有初始化。Valgrind 可以追踪这个未定义值。
    if (*number == 42) {
        g_sink = 42;
    }

    delete number;
}

void signed_integer_overflow() {
    volatile int value = std::numeric_limits<int>::max();

    // 有符号整数超过 int 可表示范围，属于未定义行为。
    g_sink = value + 1;
}

void print_usage(const char* program) {
    std::cerr << "Usage: " << program
              << " <leak|overflow|use-after-free|uninitialized|undefined>\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    const std::string mode = argv[1];
    if (mode == "leak") {
        memory_leak();
    } else if (mode == "overflow") {
        heap_buffer_overflow();
    } else if (mode == "use-after-free") {
        use_after_free();
    } else if (mode == "uninitialized") {
        uninitialized_read();
    } else if (mode == "undefined") {
        signed_integer_overflow();
    } else {
        print_usage(argv[0]);
        return 1;
    }

    std::cout << "Program finished (sink=" << g_sink << ")\n";
}

