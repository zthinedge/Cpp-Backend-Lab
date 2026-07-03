# 异步日志
## 1.定义与核心功能

异步日志是一种将**日志生成和日志写入解耦**的日志组件。    
在同步日志中，业务线程调用日志接口后，需要直接完成格式化和文件写入；而异步日志中，业务线程只负责把日志消息放入队列，真正的文件写入**由后台线程完成**。

---
## 2.异步日志的组成
- 阻塞队列(queue_)
- 后台线程(worker_)
- 文件输出流(file_)

---
## 3.核心流程分析
主要的流程就是业务线程调用写日志的函数时，会将要写入的字符串放入队列，后台线程读取字符串并异步写入到文件缓冲区中，最后析构时`flush`将内容强制刷入文件中
### 异步日志的构造

构造函数主要做三件事：

1. 初始化阻塞队列 `queue_`
2. 以追加写入模式打开日志文件
3. 启动后台线程 `worker_`

```cpp
AsyncLogger::AsyncLogger(const std::string&filename, size_t queue_capacity)
    : queue_(queue_capacity),
      file_(filename, std::ios::app)
{
    if (!file_.is_open()) {
        throw std::runtime_error("failed to open log file: " + filename);
    }

    worker_ = std::thread(&AsyncLogger::workerLoop, this);
}
```

这里的 `std::ios::app` 表示追加写入。如果日志文件已经存在，新日志会写到文件末尾；如果文件不存在，会尝试创建文件。

### 写入日志
业务线程通过 `info`、`warn`、`error` 写日志：

```cpp
logger.info("server started");
logger.warn("queue almost full");
logger.error("mysql query failed");
```

这些接口内部都会调用 `log`。`log` 负责把时间、日志级别和日志内容拼成一行字符串，然后放入阻塞队列。

```cpp
void AsyncLogger::log(const std::string& level, const std::string& msg)
{
    std::ostringstream oss;
    oss << '[' << currentTimeString() << "] "
        << '[' << level << "] "
        << msg;

    queue_.push(oss.str());
}
```

也就是说，业务线程不直接写文件，只负责生产日志消息。

### 后台线程写文件
后台线程执行 `workerLoop`，不断从阻塞队列中取出日志，并写入文件。

```cpp
void AsyncLogger::workerLoop()
{
    std::string line;
    while (queue_.pop(line)) {
        file_ << line << '\n';
    }
}
```

当队列为空时，后台线程会阻塞等待；当业务线程 push 新日志后，后台线程会被唤醒并继续写文件。

### 异步日志的析构
析构函数负责安全退出后台线程：

1. 调用 `queue_.close()` 关闭阻塞队列
2. 唤醒后台线程
3. 等待后台线程把队列中剩余日志写完
4. 调用 `join()` 回收后台线程
5. 调用 `file_.flush()` 刷新文件流缓冲区

```cpp
AsyncLogger::~AsyncLogger()
{
    queue_.close();

    if (worker_.joinable()) {
        worker_.join();
    }

    file_.flush();
}
```

需要注意的是，`file_ << line` 不代表日志已经物理写入磁盘。它通常会先进入 C++ 文件流缓冲区或操作系统页缓存。当前版本只调用 `flush()` 刷新文件流缓冲区，不做 `fsync` 级别的强制落盘。

---
## 4.编译测试
进入当前目录后执行：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -pthread AsyncLogger.cpp test_asynclogger.cpp -o test_asynclogger
./test_asynclogger
```

如果测试通过，会输出：

```text
All AsyncLogger tests passed.
```
---
## 5.后续扩展方向
- 日志级别过滤
- 同时支持控制台输出和文件输出
- 日志文件按大小或日期滚动
- 记录线程 id、源码文件名和行号
- 队列满时支持不同策略，比如阻塞、丢弃或同步写入
- 支持多 sink 输出
- 支持 `fsync` 或更严格的落盘策略
