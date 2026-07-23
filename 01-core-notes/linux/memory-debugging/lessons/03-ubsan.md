# 第 3 课：UndefinedBehaviorSanitizer

## 1. 什么是未定义行为？

未定义行为（Undefined Behavior，UB）表示 C++ 标准不规定程序接下来必须发生
什么。程序可能崩溃，也可能暂时正常，编译优化后还可能出现完全不同的结果。

UBSan 用于发现其中一部分错误，例如：

- 有符号整数溢出；
- 某些非法移位；
- 对齐错误；
- 部分空指针或类型使用错误。

它不能发现所有 C++ 未定义行为。

## 2. 运行实验

```bash
./build/memory_errors_ubsan undefined
```

预期看到类似：

```text
runtime error: signed integer overflow
```

报告会指向 `memory_errors.cpp` 中执行 `INT_MAX + 1` 的位置。

本 Lab 使用了：

```text
-fno-sanitize-recover=all
```

它让 UBSan 发现错误后立即结束程序，适合在测试和 CI 中快速暴露失败。真实项目也
可以不使用该选项，让程序继续执行并收集更多报告，但继续运行的结果不能再被认为
可靠。

## 3. 可以组合 ASan 和 UBSan 吗？

可以。实际项目常用：

```bash
g++ -O1 -g -fno-omit-frame-pointer \
  -fsanitize=address,undefined your_program.cpp -o your_program_san
```

这里的逗号表示编译时同时启用两种 Sanitizer。它们覆盖的错误类型不同，组合通常
比只开启一种更全面。

下一步：[第 4 课：工具选择与实际工作流](04-workflow.md)。

