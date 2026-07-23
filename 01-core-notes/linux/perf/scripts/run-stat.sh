#!/usr/bin/env bash
set -euo pipefail

make

echo
echo "===== CPU 热点：整体指标 ====="
perf stat -e task-clock,context-switches,cpu-migrations,page-faults \
    ./build/perf_lab cpu

echo
echo "===== 连续内存访问（耗时） ====="
perf stat -e task-clock ./build/perf_lab cache-row

echo
echo "===== 跨行内存访问（耗时） ====="
perf stat -e task-clock ./build/perf_lab cache-column
