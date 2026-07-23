# 第 1 课：Valgrind 入门

## 1. Valgrind 是什么？

Valgrind 是一套程序分析工具，其中 **Memcheck** 用于检查内存错误。执行
`valgrind ./program` 时，程序会运行在 Valgrind 的分析环境中，因此通常比直接
运行慢很多。

## 2. 第一次检查内存泄漏

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  ./build/memory_errors leak
```

逐段解释：

- `valgrind`：启动 Valgrind；
- `--leak-check=full`：显示泄漏发生位置的详细调用栈；
- `--show-leak-kinds=all`：显示所有类型的泄漏记录；
- `./build/memory_errors leak`：被检查的程序及其参数。

在输出结尾找到：

```text
LEAK SUMMARY:
   definitely lost: ... bytes
```

`definitely lost` 表示 Valgrind 确认已经没有指针能够释放这块内存。向上寻找
`memory_leak()` 和 `memory_errors.cpp`，那才是需要回到源码检查的位置。

## 3. 检查堆越界

```bash
valgrind --track-origins=yes ./build/memory_errors overflow
```

预期出现：

```text
Invalid write of size 4
```

它表示程序写入了一个不允许写入的位置。报告还会说明错误地址位于已分配内存块的
什么位置。

## 4. 检查释放后使用

```bash
valgrind --track-origins=yes ./build/memory_errors use-after-free
```

预期出现 `Invalid read`。报告通常包含两条重要调用栈：

1. 非法读取发生在哪里；
2. 这块内存之前在哪里被释放。

修复时优先关注第一次非法操作，不要只处理程序最后崩溃的位置。

## 5. 检查未初始化值

```bash
valgrind --track-origins=yes ./build/memory_errors uninitialized
```

`--track-origins=yes` 尝试追踪未初始化值来自哪里，会让分析更慢，但报告更容易
理解。预期看到：

```text
Conditional jump or move depends on uninitialised value(s)
```

## 6. 让自动测试在发现错误时失败

Valgrind 默认即使发现错误，也可能返回退出码 0。自动化测试可以使用：

```bash
valgrind --error-exitcode=1 --leak-check=full \
  ./build/memory_errors leak
```

`--error-exitcode=1` 表示发现错误时使用退出码 1，CI 就能把它识别为测试失败。

下一步：[第 2 课：AddressSanitizer](02-asan.md)。

