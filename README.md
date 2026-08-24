# Cpp-Backend-Lab

一个面向 C++ 后端开发的长期学习与实践仓库。

这里不只记录知识点，也会把线程池、阻塞队列、异步日志、连接池等常见后端组件
真正实现出来，并通过可运行示例、测试和性能实验理解它们的工作原理。

> 当前仓库仍在持续建设中，代码以学习和验证设计为主要目标，不建议未经评估直接
> 用于生产环境。

## 为什么建立这个仓库？

学习 C++ 后端时，很容易停留在“看懂概念”，但真正写代码时仍然不知道：

- 多线程组件怎样安全停止？
- 条件变量为什么需要配合谓词使用？
- 连接借出后怎样自动归还？
- 异步日志怎样让业务线程避免直接执行文件 I/O？
- 程序变慢时，如何使用工具找到真正的 CPU 热点？

这个仓库用“知识笔记 → 最小实现 → 测试验证 → 性能分析”的方式逐步回答这些问题。

## 当前内容

### 核心笔记与实验

| 方向 | 当前内容 | 状态 |
| --- | --- | --- |
| C++ | 语言特性、并发编程与资源管理 | 规划中 |
| [Linux](01-core-notes/linux/README-CN.md) | 操作系统基础与性能分析实验 | 持续整理 |
| MySQL | 存储、索引、事务与连接管理 | 规划中 |
| Redis | 数据结构、持久化与高可用 | 规划中 |
| 消息队列 | 消息模型、可靠性与顺序性 | 规划中 |

### C++ 后端基础组件

| 组件 | 核心知识 | 文档与测试 |
| --- | --- | --- |
| [ThreadPool](02-backend-components/ThreadPool/README-CN.md) | 任务队列、工作线程、条件变量、安全停止 | 已实现、测试通过 |
| [BlockQueue](02-backend-components/BlockQueue/README-CN.md) | 有界队列、生产者消费者、阻塞唤醒 | 已实现、测试通过 |
| [AsyncLogger](02-backend-components/AsyncLogger/README-CN.md) | 前后台线程解耦、日志队列、批量写入 | 已实现、测试通过 |
| [ConnectionPool](02-backend-components/ConnectionPool/README-CN.md) | 连接复用、阻塞获取、自定义删除器自动归还 | 已实现、测试通过 |
| [EventLoop](02-backend-components/EventLoop/README-CN.md) | epoll 事件分发与跨线程任务调度 | 已实现、测试通过 |
| Timer | 延时任务、周期任务与超时管理 | 规划中 |

## 仓库结构

```text
Cpp-Backend-Lab/
├── 01-core-notes/          C++、Linux、MySQL、Redis、消息队列等基础笔记
├── 02-backend-components/  独立实现的 C++ 后端基础组件
├── 03-projects-showcase/   综合项目展示（规划中）
├── 04-middleware-lab/      中间件原理与实验（规划中）
├── 05-system-design/       系统设计笔记（规划中）
└── 06-interview/           面试知识整理（规划中）
```

## 快速开始

以线程池为例：

```bash
cd 02-backend-components/ThreadPool
g++ -std=c++17 -Wall -Wextra -pedantic -pthread \
  ThreadPool.cpp test_threadpool.cpp -o test_threadpool
./test_threadpool
```

预期输出：

```text
All ThreadPool tests passed.
```

其他组件的编译命令写在各自的 `README-CN.md` 中。

## 学习路线

```text
语言与系统基础
      ↓
独立实现后端组件
      ↓
通过测试验证正确性
      ↓
组合组件完成项目
      ↓
学习中间件与系统设计
```

每个主题的详细教程和运行方式放在对应目录中，根目录只负责展示整个仓库的结构与
进度。

## 设计原则

- **先正确，再优化**：先保证并发语义和资源生命周期正确，再讨论性能。
- **最小可运行**：每个主题尽量提供能够独立编译的示例。
- **用测试验证理解**：不仅描述正常流程，也关注关闭、阻塞和资源回收。
- **用数据代替猜测**：性能优化前先建立基线、定位热点，修改后重新测量。
- **如实展示进度**：未完成的模块明确标记为规划中。


## 技术环境

- C++17
- Linux
- GCC / G++
- POSIX Threads
- GDB、perf 等调试与性能分析工具

## 说明

这是一个持续演进的学习仓库。文档会随着理解的深入不断修正，代码也会逐步加入
更完整的异常处理、边界测试、构建系统和性能基准。
