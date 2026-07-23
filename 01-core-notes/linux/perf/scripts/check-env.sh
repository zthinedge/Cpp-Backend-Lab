#!/usr/bin/env bash
set -u

echo "[1/5] perf"
if command -v perf >/dev/null 2>&1; then
    perf --version
else
    echo "未找到 perf。Ubuntu 可运行："
    echo "sudo apt install linux-tools-common linux-tools-generic linux-tools-\$(uname -r)"
fi

echo
echo "[2/5] compiler"
if command -v g++ >/dev/null 2>&1; then
    g++ --version | head -n 1
else
    echo "未找到 g++。Ubuntu 可运行：sudo apt install build-essential"
fi

echo
echo "[3/5] kernel"
uname -r

echo
echo "[4/5] virtualization"
if command -v systemd-detect-virt >/dev/null 2>&1; then
    virtualization="$(systemd-detect-virt 2>/dev/null || true)"
    if [[ -n "$virtualization" && "$virtualization" != "none" ]]; then
        echo "$virtualization"
        echo "检测到虚拟化环境。cycles、instructions、cache-misses 等硬件事件可能显示 <not supported>；这不是安装错误。"
    else
        echo "未检测到虚拟化环境"
    fi
else
    echo "无法自动检测"
fi

echo
echo "[5/5] perf_event_paranoid"
if [[ -r /proc/sys/kernel/perf_event_paranoid ]]; then
    cat /proc/sys/kernel/perf_event_paranoid
    echo "数值较高时，普通用户可使用的性能事件会受限。先直接做实验；遇到权限错误再看 README 的排错章节。"
else
    echo "当前系统没有暴露 /proc/sys/kernel/perf_event_paranoid"
fi
