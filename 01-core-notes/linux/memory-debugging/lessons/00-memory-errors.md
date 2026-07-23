# 第 0 课：先认识内存错误

## 1. 内存泄漏不是唯一的内存问题

常见错误包括：

| 错误 | 含义 | 可能表现 |
| --- | --- | --- |
| 内存泄漏 | 申请内存后失去指针，无法再释放 | 长时间运行后内存不断增加 |
| 堆越界 | 访问 `new`/`malloc` 分配范围之外 | 崩溃、数据损坏或暂时没有现象 |
| 释放后使用 | `delete`/`free` 后继续使用指针 | 偶发崩溃、读到错误数据 |
| 未初始化读取 | 使用尚未赋值的数据 | 随机结果、分支行为不稳定 |
| 未定义行为 | C++ 标准不规定结果的操作 | 不同优化等级出现不同结果 |

最危险的情况不是每次都崩溃，而是程序看起来正常。C++ 的非法内存访问属于未定义
行为，不能用“这次运行成功”证明代码正确。

## 2. 检查环境

```bash
# 从 Cpp-Backend-Lab 仓库根目录开始
cd 01-core-notes/linux/memory-debugging
chmod +x scripts/check-env.sh
./scripts/check-env.sh
```

脚本会检查 G++、Valgrind，以及编译器是否支持 ASan/UBSan。

## 3. 构建三个版本

```bash
make
```

将生成：

```text
build/memory_errors         普通版本，交给 Valgrind 分析
build/memory_errors_asan    编译时加入了 AddressSanitizer
build/memory_errors_ubsan   编译时加入了 UBSan
```

为什么需要不同版本？Valgrind 是外部分析工具；Sanitizer 需要编译器在程序中加入
检查逻辑，这个过程叫“插桩”。

## 4. 认识实验参数

```text
leak             内存泄漏
overflow         堆缓冲区越界
use-after-free   释放后使用
uninitialized    未初始化读取
undefined        有符号整数溢出
```

不要把这些示例复制进真实项目。它们是专门用于观察诊断报告的错误代码。

下一步：[第 1 课：Valgrind 入门](01-valgrind.md)。
