#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

namespace {

volatile std::uint64_t g_sink = 0;

using Clock = std::chrono::steady_clock;

template <typename Function>
void measure(const std::string& name, Function function) {
    const auto begin = Clock::now();
    function();
    const auto end = Clock::now();
    const auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << name << ": " << elapsed.count() << " ms"
              << " (ignore=" << g_sink << ")\n";
}

__attribute__((noinline)) std::uint64_t hot_function(std::uint64_t rounds) {
    std::uint64_t value = 1;
    for (std::uint64_t i = 1; i <= rounds; ++i) {
        value = value * 1664525ULL + 1013904223ULL;
        value ^= value >> 13;
    }
    return value;
}

__attribute__((noinline)) std::uint64_t cold_function(std::uint64_t rounds) {
    std::uint64_t value = 0;
    for (std::uint64_t i = 0; i < rounds; ++i) {
        value += i * 3 + 1;
    }
    return value;
}

void cpu_hotspot() {
    g_sink = hot_function(350'000'000);
    g_sink ^= cold_function(5'000'000);
}

constexpr std::size_t kMatrixSide = 4096;

void cache_row_major() {
    std::vector<std::uint32_t> matrix(kMatrixSide * kMatrixSide, 1);
    std::uint64_t sum = 0;

    for (std::size_t row = 0; row < kMatrixSide; ++row) {
        for (std::size_t column = 0; column < kMatrixSide; ++column) {
            sum += matrix[row * kMatrixSide + column];
        }
    }
    g_sink = sum;
}

void cache_column_major() {
    std::vector<std::uint32_t> matrix(kMatrixSide * kMatrixSide, 1);
    std::uint64_t sum = 0;

    for (std::size_t column = 0; column < kMatrixSide; ++column) {
        for (std::size_t row = 0; row < kMatrixSide; ++row) {
            sum += matrix[row * kMatrixSide + column];
        }
    }
    g_sink = sum;
}

__attribute__((noinline)) std::uint64_t parse_requests(std::uint64_t count) {
    return hot_function(count);
}

__attribute__((noinline)) std::uint64_t handle_requests(std::uint64_t count) {
    return parse_requests(count);
}

__attribute__((noinline)) std::uint64_t event_loop(std::uint64_t count) {
    return handle_requests(count);
}

void call_graph() {
    g_sink = event_loop(350'000'000);
}

void print_usage(const char* program) {
    std::cerr << "Usage: " << program
              << " <cpu|cache-row|cache-column|callgraph>\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    const std::string mode = argv[1];
    if (mode == "cpu") {
        measure("CPU hotspot", cpu_hotspot);
    } else if (mode == "cache-row") {
        measure("row-major access", cache_row_major);
    } else if (mode == "cache-column") {
        measure("column-major access", cache_column_major);
    } else if (mode == "callgraph") {
        measure("call graph", call_graph);
    } else {
        print_usage(argv[0]);
        return 1;
    }
}

