# 第 1 课：第一次性能体检

这一课只学习一个命令：`perf stat`。

## 1. 先执行

```bash
perf stat -e task-clock,context-switches,cpu-migrations,page-faults \
  ./build/perf_lab cpu
```

命令虽然很长，但可以拆开：

```text
perf                运行 perf 工具
stat                选择“整体统计”功能
-e                  event，指定想观察的事件
task-clock,...       四个事件的名字，逗号分隔
./build/perf_lab     要观察的程序
cpu                  传给 perf_lab 的参数
```

行尾的 `\` 表示“这条命令还没有结束，下一行仍属于同一条命令”。也可以把整条
命令写在一行中。

## 2. 什么是事件？

程序运行时，CPU 会发生很多事情，例如执行指令、经历时钟周期、遇到分支。
`perf` 把这些可以统计的事情称为 **event（事件）**。

先认识输出中的五项：

| 名称 | 新手理解 |
| --- | --- |
| `task-clock` | 这个进程实际占用 CPU 的时间 |
| `context-switches` | Linux 切换当前运行任务的次数 |
| `cpu-migrations` | 进程从一个 CPU 核心转移到另一个核心的次数 |
| `page-faults` | 进程访问的虚拟内存页需要由内核处理的次数 |

这四项属于软件事件，即使在大多数虚拟机中也能正常统计。

## 3. 怎样阅读输出？

输出大致如下：

```text
470.00 msec task-clock
2 context-switches
0 cpu-migrations
127 page-faults
0.471 seconds time elapsed
```

第一遍只看两处：

1. `seconds time elapsed`：从开始到结束，人实际等待了多久；
2. `task-clock`：这个进程实际使用 CPU 的时间。

如果程序主要做 CPU 计算，两者通常比较接近。如果程序花很多时间等待磁盘、网络或
锁，`task-clock` 可能明显小于实际等待时间。

`# 0.999 CPUs utilized` 表示程序运行期间平均使用了约 0.999 个 CPU 核心。它不是
CPU 使用率 0.999%，而是接近完整占用一个核心。

## 4. 为什么每次数字不完全相同？

把相同命令再运行三次：

```bash
perf stat -r 3 -e task-clock ./build/perf_lab cpu
```

新选项：

- `-r` 是 repeat（重复）；
- `-r 3` 表示重复运行 3 次；
- 这一次只观察 `task-clock`，让输出更容易阅读。

电脑同时还在运行桌面、编辑器和其他进程，CPU 温度和频率也会变化。因此性能
测量天然存在波动，不能只跑一次就下结论。

## 5. 可选拓展：硬件事件

`cycles`、`instructions` 等事件由 CPU 内部的硬件计数器提供：

```bash
perf stat -e cycles,instructions,branches,branch-misses \
  ./build/perf_lab cpu
```

它们的含义：

| 名称 | 新手理解 |
| --- | --- |
| `cycles` | CPU 经历了多少工作节拍 |
| `instructions` | CPU 执行了多少条机器指令 |
| `branches` | 执行了多少次“根据条件选择下一步” |
| `branch-misses` | CPU 猜错下一步的次数 |

如果能够得到数字，输出还可能显示 IPC：

```text
IPC = instructions ÷ cycles
```

IPC 是每个 CPU cycle 平均完成的指令数。它只是一条调查线索，不能单独判断程序
好坏。

### 如果显示 `<not supported>`

这表示当前机器没有向 `perf` 提供对应的硬件计数器，常见于虚拟机、容器和部分
云主机。这不代表命令写错，也不代表 `perf` 安装失败。

本项目当前运行在 VMware 中，出现下面的结果是预期现象：

```text
<not supported> cycles
<not supported> instructions
<not supported> branches
<not supported> branch-misses
```

继续使用本课开头的软件事件即可：

```bash
perf stat -e task-clock,context-switches,cpu-migrations,page-faults \
  ./build/perf_lab cpu
```

后面的热点采样课程通常仍然可以完成。

## 本课检查

不用背数字，尝试回答：

1. `stat` 会不会生成一个供以后阅读的数据文件？
2. `-e` 后面为什么要写事件名称？
3. 为什么性能测试最好重复运行？

答案：`stat` 直接显示整体统计，不生成采样报告；`-e` 用于选择需要统计的事件；
重复运行可以避免被一次偶然波动误导。

下一步：[第 2 课：寻找最耗 CPU 的函数](02-record-report.md)。
