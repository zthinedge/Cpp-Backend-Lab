# 连接池

## 1. 定义与核心功能

连接池是一种**维持**和**管理固定数量数据库连接**的池式结构，用于复用数据库连接，避免频繁创建和销毁连接。

- **维持**：启动时创建一定数量的连接，保存在连接队列中。
- **管理**：业务线程需要访问数据库时从池中借连接，用完后自动归还。
- **阻塞等待**：当连接都被借出时，新的 `acquire` 调用会阻塞等待，直到有连接归还。

---

## 2. 连接池的组成

连接池当前采用固定容量设计，核心成员包括：

1. **连接队列 (`connections_`)**：保存当前可用的连接。
2. **互斥锁 (`mutex_`)**：保护连接队列和停止标志。
3. **条件变量 (`cv_`)**：当没有可用连接时，让调用 `acquire()` 的线程阻塞等待。
4. **停止标志 (`stopping_`)**：用于连接池析构时唤醒等待线程并停止继续获取连接。
5. **连接对象 (`MySqlConnection`)**：当前是模拟连接，用于验证连接池逻辑。

---

## 3. 核心流程分析

### 连接池的构造

1. 检查 `pool_size` 是否大于 0。
2. 创建指定数量的 `MySqlConnection`。
3. 调用 `connect()` 模拟建立连接。
4. 将连接放入 `connections_` 队列。

### 获取连接

1. 调用 `acquire()`。
2. 获取互斥锁。
3. 如果队列为空且连接池未停止，则通过 `condition_variable` 阻塞等待。
4. 如果连接池已经停止，返回 `nullptr`。
5. 从队列中取出一个连接。
6. 返回一个带自定义删除器的 `shared_ptr`。

### 自动归还连接

`acquire()` 返回的不是普通 `shared_ptr`，而是带自定义删除器的 `shared_ptr`。

当外部连接对象离开作用域或调用 `reset()` 时，自定义删除器会执行归还逻辑：

```cpp
return std::shared_ptr<MySqlConnection>(
    conn.get(),
    [this, conn](MySqlConnection*) {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.push(conn);
        cv_.notify_one();
    }
);
```

---
## 4.编译测试
进入当前目录后执行：

```bash
g++ ConnectionPool.cpp test_connectionpool.cpp -o test -pthread
./test
```

如果测试通过，会输出：

```text
All ThreadPool tests passed.
```