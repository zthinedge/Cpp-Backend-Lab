#!/usr/bin/env bash
set -u

echo "[1/3] C++ compiler"
if command -v g++ >/dev/null 2>&1; then
    g++ --version | head -n 1
else
    echo "未找到 g++。Ubuntu 可运行：sudo apt install build-essential"
fi

echo
echo "[2/3] Valgrind"
if command -v valgrind >/dev/null 2>&1; then
    valgrind --version
else
    echo "未找到 Valgrind。Ubuntu 可运行：sudo apt install valgrind"
fi

echo
echo "[3/3] Sanitizer"
temporary_source="$(mktemp --suffix=.cpp)"
temporary_binary="$(mktemp)"
trap 'rm -f "$temporary_source" "$temporary_binary"' EXIT
printf 'int main() { return 0; }\n' >"$temporary_source"

if g++ -x c++ -fsanitize=address,undefined "$temporary_source" \
    -o "$temporary_binary" >/dev/null 2>&1; then
    echo "G++ 支持 AddressSanitizer 和 UndefinedBehaviorSanitizer"
else
    echo "当前 G++ 无法构建 ASan/UBSan 测试程序"
fi

