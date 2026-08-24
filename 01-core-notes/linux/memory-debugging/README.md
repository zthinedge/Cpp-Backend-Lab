# 从零学习 C++ 内存错误诊断

这个 Lab 不是只教你背一条“查内存泄漏”的命令，而是帮助你建立完整流程：

```text
认识错误类型
    ↓
使用工具得到报告
    ↓
找到第一次非法操作
    ↓
回到源码修复
    ↓
重新运行测试确认错误消失
```

## 为什么同时学习三种工具？

| 工具 | 使用方式 | 主要用途 |
| --- | --- | --- |
| Valgrind Memcheck | 在工具中运行普通程序 | 泄漏、非法访问、未初始化值 |
| AddressSanitizer | 编译时加入 `-fsanitize=address` | 越界、释放后使用、重复释放、泄漏 |
| UBSan | 编译时加入 `-fsanitize=undefined` | 部分 C++ 未定义行为 |

它们不是互相替代的关系。ASan 通常更快，适合日常开发和自动测试；Valgrind 不要求
重新插桩编译，并且擅长追踪未初始化值；UBSan 检查的是另一组语言层面的错误。

## 目录

```text
memory-debugging/
├── README.md
├── README-CN.md
├── Makefile
├── examples/
│   └── memory_errors.cpp
├── lessons/
│   ├── 00-memory-errors.md
│   ├── 01-valgrind.md
│   ├── 02-asan.md
│   ├── 03-ubsan.md
│   └── 04-workflow.md
└── scripts/
    └── check-env.sh
```

## 学习顺序

1. [先认识内存错误](lessons/00-memory-errors.md)
2. [Valgrind 入门](lessons/01-valgrind.md)
3. [AddressSanitizer 入门](lessons/02-asan.md)
4. [UBSan 入门](lessons/03-ubsan.md)
5. [工具选择与实际工作流](lessons/04-workflow.md)

完成后，你应该能看懂报告中的错误类型、源码位置和调用栈，并知道下一步先检查
哪一行代码。

