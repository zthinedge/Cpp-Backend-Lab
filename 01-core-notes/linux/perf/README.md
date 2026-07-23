# Linux perf 零基础实验室

一组为初学者设计的 Linux 性能分析实验。

你不需要提前了解 `perf`、CPU 性能计数器或汇编。课程会先解释源代码、程序、
进程和常见终端命令，再逐步进入性能分析。

## 你会学到什么？

- 使用 `perf stat` 查看程序的整体运行数据；
- 使用 `perf record` 保存性能样本；
- 使用 `perf report` 寻找最耗 CPU 的函数；
- 阅读函数调用链，判断热点从哪里进入；
- 通过内存访问实验理解 CPU Cache；
- 把同样的分析流程应用到自己的 C++ 程序。

## 从这里开始

| 顺序 | 课程 | 内容 |
| --- | --- | --- |
| 0 | [准备与基础概念](lessons/00-getting-started.md) | `cd`、`pwd`、`./`、编译、程序与进程 |
| 1 | [第一次性能体检](lessons/01-perf-stat.md) | 学习 `perf stat` 和软件事件 |
| 2 | [寻找最耗 CPU 的函数](lessons/02-record-report.md) | 学习采样、`record` 和 `report` |
| 3 | [观察调用链](lessons/03-call-graph.md) | 理解热点函数由谁调用 |
| 4 | [CPU Cache 实验](lessons/04-cache.md) | 比较连续访问和跨行访问 |
| 5 | [分析自己的程序](lessons/05-your-program.md) | 将流程迁移到真实项目 |

第一次学习时，请严格按照顺序完成，不要直接跳到调用链或 Cache。

## 运行环境

```bash
./scripts/check-env.sh
make
./build/perf_lab cpu
```

环境检查脚本会识别虚拟机。如果 `cycles`、`instructions` 等硬件事件显示
`<not supported>`，通常是虚拟机没有提供硬件计数器，并不是安装错误。课程的
软件事件、热点函数和调用链实验仍然可以完成。

更完整的学习说明见[中文课程说明](README-CN.md)。
