# 第 2 课：AddressSanitizer

## 1. ASan 与 Valgrind 有什么不同？

AddressSanitizer 简称 ASan。它由编译器在构建时把检查逻辑加入程序，因此必须使用
`-fsanitize=address` 重新编译。

本 Lab 已经生成：

```text
build/memory_errors_asan
```

ASan 通常比 Valgrind Memcheck 快，更适合日常开发、单元测试和 CI，但程序运行时
会使用更多内存。

## 2. 检查堆越界

```bash
./build/memory_errors_asan overflow
```

程序中止并出现：

```text
ERROR: AddressSanitizer: heap-buffer-overflow
```

阅读报告时按顺序找：

1. 错误类型：`heap-buffer-overflow`；
2. 是 `READ` 还是 `WRITE`；
3. 第一处属于自己项目的 `.cpp:行号`；
4. 相关内存最初在哪里分配。

报告后面的大段 Shadow bytes 是 ASan 内部的内存标记。入门阶段不需要逐字阅读。

## 3. 检查释放后使用

```bash
./build/memory_errors_asan use-after-free
```

预期错误：

```text
ERROR: AddressSanitizer: heap-use-after-free
```

报告会分别显示非法读取、释放位置和最初分配位置。它们共同描述了这块内存的生命
周期。

## 4. 检查泄漏

```bash
ASAN_OPTIONS=detect_leaks=1 ./build/memory_errors_asan leak
```

`ASAN_OPTIONS` 是环境变量，`detect_leaks=1` 表示启用泄漏检测。Linux 上 ASan
通常已经默认启用，但显式写出更容易理解实验目的。

预期看到：

```text
ERROR: LeakSanitizer: detected memory leaks
```

## 5. 为什么 ASan 没报告未初始化读取？

```bash
./build/memory_errors_asan uninitialized
```

它可能正常结束，因为 ASan 的核心职责不是追踪每个值是否初始化。这个实验应使用
Valgrind Memcheck。工具没有报告，不等于代码正确；必须根据错误类型选择工具。

不要同时执行 `valgrind ./build/memory_errors_asan ...`。Valgrind 与 ASan 都会
接管内存行为，组合使用可能产生冲突和难以解释的结果。

下一步：[第 3 课：UBSan](03-ubsan.md)。

