# 阻塞队列
## 1.定义与核心功能
阻塞队列是一种常用于**生产者-消费者**模型中的线程安全队列。主要用于在**生产者线程和消费者线程**之间传递数据，起到**任务暂存**和**缓冲**和作用。 


---
## 2.阻塞队列的组成
1. 任务队列
2. 同步机制：主要是互斥锁和条件变量，两个条件变量`not_full_`和`not_empty_`

---
## 3.核心流程分析
### 阻塞队列的构造
引入**模板类**以匹配不同类型，构造函数只要传参容量大小即可
```cpp
template <typename T>
BlockQueue<T>::BlockQueue(size_t capacity)
    : capacity_(capacity)
{
    if (capacity_ == 0)
    {
        throw std::invalid_argument("capacity must be greater than 0");
    }
}
```
### 生产者消费者模型的运用
因为对于一个任务来说，生产者生产一个任务，那么需要有对应的消费者来进行消费。但是存在可能生产者生产出任务，但是消费者还没有执行完，所以就需要有阻塞队列来进行一个缓冲。然后生成者主要的操作就是`push`,消费者是`pop`。
- `push`:生产者线程往阻塞队列中放入任务，需要加锁，然后条件变量`not_full_`进行休眠直到阻塞队列关闭或者说当前队列还有可以存放的位置，之后放入任务，唤醒一个挂在`not_empty_`上的消费者线程进行处理
```cpp
template <typename T>
void BlockQueue<T>::push(T value)
{
    std::unique_lock<std::mutex> lock(mutex_);

    not_full_.wait(lock, [this]
                   { return closed_ || queue_.size() < capacity_; });

    if (closed_)
    {
        throw std::runtime_error("push on closed BlockQueue");
    }

    queue_.push(std::move(value));
    //唤醒一个消费者线程
    not_empty_.notify_one();
}
```
- `pop`:消费者线程拿出阻塞队列中的任务，`not_empty_`休眠直到阻塞队列非空或者阻塞队列关闭，`pop`取走任务后，队列腾出空间，唤醒一个等待在 `not_full_ `上的生产者线程
```cpp
//针对消费者
template <typename T>
bool BlockQueue<T>::pop(T &value)
{
    std::unique_lock<std::mutex> lock(mutex_);

    not_empty_.wait(lock, [this]
                    { return closed_ || !queue_.empty(); });

    if (queue_.empty())
    {
        return false;
    }

    value = std::move(queue_.front());
    queue_.pop();
    not_full_.notify_one();

    return true;
}
```
### 阻塞队列的析构
因为是多线程，析构函数会调用 `close()`，`close()` 内部加锁设置 `closed_ = true`，挂在条件变量`not_empty_`和`not_full_`上的线程会被唤醒 读到`closed_`后会进行退出
```cpp
template <typename T>
void BlockQueue<T>::close()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }
    //唤醒所有休眠的线程
    not_empty_.notify_all();
    not_full_.notify_all();
}
```

---
## 4.编译测试
进入当前目录后执行：

```bash
g++ test_blockqueue.cpp -o test_blockqueue -pthread
./test_blockqueue
```

如果测试通过，会输出：

```text
All BlockQueue tests passed.
```