# 第 3 课：观察调用链

找到热点函数后，还需要知道“是谁调用了它”。

## 1. 什么是调用链？

实验程序故意设计了下面的调用关系：

```text
main
└── call_graph
    └── event_loop
        └── handle_requests
            └── parse_requests
                └── hot_function
```

程序从 `main` 开始，逐层调用，最后进入 `hot_function`。这条路径叫调用链，某个
时刻尚未返回的这些函数共同组成调用栈。

实际服务器中，同一个底层函数可能被登录请求、查询请求等不同路径调用。只看底层
热点函数，可能还不知道真正应该优化哪条业务路径。

## 2. 带调用栈采样

```bash
perf record -g -o results/callgraph.data -- \
  ./build/perf_lab callgraph
```

这一课只新增了一个选项：

- `-g`：在采样时同时记录 call graph（调用图/调用链）；
- `callgraph`：告诉实验程序运行调用链模式。

## 3. 阅读调用链

```bash
perf report -i results/callgraph.data
```

在报告中找到 `hot_function`：

1. 用上下方向键选中它；
2. 按 `Enter` 展开；
3. 观察 `event_loop → handle_requests → parse_requests → hot_function`；
4. 按 `q` 返回或退出。

如果想直接打印文本：

```bash
perf report --stdio -i results/callgraph.data
```

## 4. Self 和 Children 是什么？

报告可能显示两种占比：

- `Self`：样本直接落在这个函数自身的比例；
- `Children`：函数自身以及它调用的所有子函数合计的比例。

例如 `event_loop` 自己只做了一次函数调用，它的 `Self` 可能接近 0；但所有耗时
工作都从它进入，因此 `Children` 可以很高。

不要看到 `Children` 高就断定该函数自身代码很慢。

## 5. 调用链不完整怎么办？

本实验编译时使用了 `-fno-omit-frame-pointer`，意思是保留用于回溯调用栈的
“路标”。

真实项目仍可能出现不完整的栈，可以尝试：

```bash
perf record --call-graph dwarf -o results/callgraph.data -- \
  ./build/perf_lab callgraph
```

这里先不深入 DWARF。只需要知道它是另一种还原调用栈的方式，数据文件通常更大，
采样成本也更高。

下一步：[第 4 课：CPU Cache 实验](04-cache.md)。

