# C++ 后端基础组件

这里通过独立、可运行的实现学习 C++ 后端常见组件。每个组件都包含源码、测试和
中文说明，重点关注并发控制、资源生命周期与安全停止。

## 已实现组件

| 组件 | 核心能力 | 状态 |
| --- | --- | --- |
| [ThreadPool](ThreadPool/README-CN.md) | 固定工作线程、任务提交、条件变量唤醒、安全停止 | 测试通过 |
| [BlockQueue](BlockQueue/README-CN.md) | 有界缓冲、生产者消费者、关闭后唤醒 | 测试通过 |
| [AsyncLogger](AsyncLogger/README-CN.md) | 业务线程与文件写入解耦、后台消费日志 | 测试通过 |
| [ConnectionPool](ConnectionPool/README-CN.md) | 固定容量、阻塞获取、连接自动归还 | 测试通过 |
| [EventLoop](EventLoop/README-CN.md) | epoll 事件分发、跨线程任务调度、安全退出 | 测试通过 |

## 规划中的组件

| 组件 | 计划内容 |
| --- | --- |
| Timer | 延时任务、周期任务和超时处理 |

## 如何运行

每个已实现组件都可以独立编译。以阻塞队列为例：

```bash
cd BlockQueue
g++ -std=c++17 -Wall -Wextra -pedantic -pthread \
  BlockQueue.cpp test_blockqueue.cpp -o test_blockqueue
./test_blockqueue
```

其他组件的准确命令和预期输出请查看对应目录中的 `README-CN.md`。

## 设计目标

- 组件职责清晰，尽量保持低耦合。
- 用最小实现突出核心机制，避免过早引入复杂抽象。
- 示例能够独立编译和运行。
- 文档不仅说明“怎么用”，也解释“为什么这样设计”。
- 通过测试覆盖正常流程、阻塞唤醒和资源回收。

## 后续工作

- [ ] 添加统一的 CMake 构建入口
- [ ] 接入自动化测试
- [ ] 补充边界条件和并发压力测试
- [ ] 使用 perf 对线程池和异步日志进行性能分析
- [ ] 将组件组合进完整后端项目
