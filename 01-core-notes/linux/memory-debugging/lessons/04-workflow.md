# 第 4 课：工具选择与实际工作流

## 1. 怎样选择？

| 情况 | 优先工具 | 原因 |
| --- | --- | --- |
| 日常开发和单元测试 | ASan + UBSan | 速度较快，容易接入构建和 CI |
| 怀疑未初始化值 | Valgrind Memcheck | 能追踪未初始化值的使用与来源 |
| 无法重新编译目标程序 | Valgrind | 不要求加入 Sanitizer 编译选项 |
| 检查泄漏 | ASan/LSan 或 Valgrind | 两者都可以，结合环境选择 |
| 性能热点 | perf | 这是性能问题，不是内存正确性问题 |

Valgrind 较慢，不适合直接分析高负载生产服务。Sanitizer 版本也会改变程序性能和
内存布局，通常用于开发、测试或专门的诊断环境。

## 2. 推荐开发流程

```text
普通 Debug/Release 构建
        ↓
单元测试使用 ASan + UBSan
        ↓
复杂未初始化问题使用 Valgrind
        ↓
修复报告中第一处属于项目的错误
        ↓
重新运行相同测试
        ↓
再运行完整测试，防止回归
```

## 3. 分析自己的单文件程序

Sanitizer 版本：

```bash
g++ -std=c++17 -O1 -g -fno-omit-frame-pointer \
  -fsanitize=address,undefined your_program.cpp -o your_program_san
./your_program_san
```

Valgrind 版本：

```bash
g++ -std=c++17 -O0 -g your_program.cpp -o your_program
valgrind --leak-check=full --track-origins=yes ./your_program
```

`-g` 很重要，它让报告能够展示源码文件和行号。

## 4. CMake 项目

学习阶段可以单独创建 Sanitizer 构建目录：

```bash
cmake -S . -B build-asan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
```

不要把 Sanitizer 编译选项无条件加到所有正式发布构建中。更完善的项目通常通过
CMake 选项或独立 preset 控制。

## 5. 看到报告时先做什么？

1. 确认错误类型，而不是先看最后一行；
2. 找到调用栈中第一处属于自己项目的源码；
3. 查清相关内存在哪里分配、在哪里释放；
4. 修复根因，不要通过隐藏报告来“解决”；
5. 使用相同输入重新运行，确认报告消失；
6. 再运行完整测试，检查是否引入新问题。

完成这组课程后，再考虑把 Sanitizer 构建加入项目的自动化测试。

