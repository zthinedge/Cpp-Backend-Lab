# EventLoop 事件循环

## 1. 定义与核心功能

EventLoop 是事件驱动网络程序的调度核心。它在所属线程中等待 I/O 事件，把就绪事件分发给对应的 `Channel`，并执行其他线程提交的任务。

这个示例只关注事件循环本身，不包含 TCP 连接管理和定时器：

- 使用 `epoll` 监听文件描述符；
- 使用 `Channel` 封装 fd、关注事件和回调；
- 使用 `eventfd` 唤醒阻塞在 `epoll_wait` 中的线程；
- 支持跨线程提交任务和安全退出；
- 保证 `loop()` 只能在创建 EventLoop 的线程中运行。

---

## 2. 组件组成

### EventLoop

负责循环等待事件、分发回调和执行待处理任务。每个 EventLoop 只属于一个线程，这样 I/O 状态可以集中在所属线程中修改。

### Poller

对 `epoll_create1`、`epoll_ctl` 和 `epoll_wait` 进行最小封装，负责 Channel 的注册、修改、删除和就绪事件收集。

### Channel

Channel 不拥有文件描述符，只描述：

- 正在监听哪个 fd；
- 关注可读还是可写事件；
- 事件发生后执行哪个回调。

因此，fd 仍由创建它的业务对象负责关闭。

---

## 3. 核心流程

### I/O 事件分发

```text
Channel 注册关注事件
        ↓
Poller 调用 epoll_ctl
        ↓
EventLoop 阻塞在 epoll_wait
        ↓
Poller 返回就绪 Channel
        ↓
Channel 执行读、写或错误回调
```

### 跨线程任务调度

其他线程不能直接执行 EventLoop 所属线程的操作，而是调用 `queueInLoop()`：

1. 使用互斥锁把任务加入待执行队列；
2. 向 `eventfd` 写入数据；
3. `epoll_wait` 被唤醒；
4. EventLoop 读取 `eventfd` 并执行任务队列。

`runInLoop()` 会先判断调用者是否为所属线程：如果是就立即执行，否则转入任务队列。

### 安全退出

`quit()` 使用原子变量记录退出状态，并写入 `eventfd`。即使 EventLoop 此时没有任何网络事件，也能从 `epoll_wait` 中醒来并结束循环。

---

## 4. 为什么暂时不包含定时器

定时器涉及 `timerfd`、到期时间排序、重复任务和取消语义，是一个可以独立学习和测试的组件。当前 EventLoop 先保持职责单一，后续 Timer 完成后再通过 Channel 接入事件循环。

---

## 5. 编译测试

进入当前目录后执行：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread \
  Channel.cpp Poller.cpp EventLoop.cpp test_eventloop.cpp \
  -o test_eventloop
./test_eventloop
```

也可以使用 CMake：

```bash
cmake -S . -B build
cmake --build build
./build/test_eventloop
```

如果测试通过，会输出：

```text
All EventLoop tests passed.
```

测试覆盖：

- pipe 可读事件分发；
- 多线程并发提交任务；
- `runInLoop()` 在所属线程执行；
- 空闲 EventLoop 的跨线程退出；
- 拒绝在错误线程运行 EventLoop。
