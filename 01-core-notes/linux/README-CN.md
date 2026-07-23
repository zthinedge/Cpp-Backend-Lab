# Linux 核心笔记

这个目录用于整理 C++ 后端开发需要掌握的 Linux 知识，并通过可运行实验理解
操作系统行为。

## 已有内容

| 主题 | 你会学到什么 | 状态 |
| --- | --- | --- |
| [perf 性能分析](perf/README-CN.md) | 程序与进程、`perf stat`、热点函数、调用链、CPU Cache | 可运行 |

## perf 零基础实验室

如果之前没有使用过 `perf`，请按顺序学习：

1. [准备与基础概念](perf/lessons/00-getting-started.md)
2. [第一次性能体检：perf stat](perf/lessons/01-perf-stat.md)
3. [寻找最耗 CPU 的函数](perf/lessons/02-record-report.md)
4. [观察调用链](perf/lessons/03-call-graph.md)
5. [CPU Cache 实验](perf/lessons/04-cache.md)
6. [分析自己的程序](perf/lessons/05-your-program.md)

实验包含一个专门制造 CPU 热点和内存访问差异的 C++ 程序。进入实验目录后运行：

```bash
cd perf
./scripts/check-env.sh
make
```

## 后续计划

- 进程、线程与上下文切换
- 互斥锁、条件变量与 futex
- 文件描述符与 Linux I/O
- epoll 与事件驱动模型
- 内存、缺页与虚拟地址空间
- `strace`、`gdb` 和其他诊断工具

规划中的主题会在内容真正加入仓库后补充链接，避免出现无法访问的空目录导航。

