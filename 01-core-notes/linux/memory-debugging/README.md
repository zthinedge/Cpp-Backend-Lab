# C++ 内存错误诊断实验室

通过故意包含错误的 C++ 程序，从零学习 Valgrind、AddressSanitizer（ASan）和
UndefinedBehaviorSanitizer（UBSan）。

## 你会学到什么？

- 区分内存泄漏、越界访问、释放后使用和未初始化读取；
- 用 Valgrind 检查普通编译的程序；
- 用 ASan 快速定位越界、释放后使用和泄漏；
- 用 UBSan 检查部分 C++ 未定义行为；
- 根据开发阶段和运行环境选择工具。

## 课程

| 顺序 | 课程 | 内容 |
| --- | --- | --- |
| 0 | [先认识内存错误](lessons/00-memory-errors.md) | 五类错误与实验安全说明 |
| 1 | [Valgrind 入门](lessons/01-valgrind.md) | Memcheck、泄漏摘要和错误栈 |
| 2 | [AddressSanitizer 入门](lessons/02-asan.md) | 编译插桩与 ASan 报告 |
| 3 | [UBSan 入门](lessons/03-ubsan.md) | 检查部分未定义行为 |
| 4 | [工具选择与实际工作流](lessons/04-workflow.md) | 对比工具并分析自己的项目 |

## 快速检查

```bash
./scripts/check-env.sh
make
```

请从第 0 课开始，不要直接批量运行所有错误示例。每个示例都会故意触发一种错误，
Sanitizer 中止程序或 Valgrind 返回错误报告都是预期结果。

完整说明见[中文课程入口](README-CN.md)。

