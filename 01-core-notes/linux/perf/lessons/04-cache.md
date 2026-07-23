# 第 4 课：CPU Cache 实验

这一课解释一个常见现象：两个程序做了相同数量的加法，速度仍可能相差很多。

## 1. CPU Cache 是什么？

CPU 计算很快，读取主内存相对较慢。CPU Cache 是靠近 CPU 的小型高速存储，用于
暂存最近可能使用的数据。

可以类比：

```text
桌面上的资料（Cache）：容量小，但伸手就能拿到
书架上的资料（内存）：容量大，但每次拿取更慢
```

CPU 从内存读取数据时，通常一次搬运附近的一小块，而不只是一个数字。因此连续
访问相邻数据通常更友好。

## 2. 先比较运行时间

连续访问二维矩阵：

```bash
./build/perf_lab cache-row
```

跨行访问同一个矩阵：

```bash
./build/perf_lab cache-column
```

两个模式读取相同数量的整数并得到相同结果，只是访问顺序不同。在当前实验环境中，
连续访问通常明显更快。

请把两次毫秒数写下来：

```text
cache-row:    ______ ms
cache-column: ______ ms
```

## 3. 使用 perf 比较 CPU 时间

下面的命令在虚拟机中也可以完成：

```bash
perf stat -r 3 -e task-clock ./build/perf_lab cache-row
```

然后：

```bash
perf stat -r 3 -e task-clock ./build/perf_lab cache-column
```

`-r 3` 会分别运行三次。比较两组 `task-clock` 和 `seconds time elapsed`，确认差距
不是单次运行的偶然波动。

## 4. 可选拓展：观察硬件 Cache 事件

只有当前系统支持硬件计数器时，才执行这一部分：

```bash
perf stat -e task-clock,cycles,instructions,cache-references,cache-misses \
  ./build/perf_lab cache-row
```

然后：

```bash
perf stat -e task-clock,cycles,instructions,cache-references,cache-misses \
  ./build/perf_lab cache-column
```

新增的事件：

- `cache-references`：访问硬件缓存的次数；
- `cache-misses`：需要的数据没有在目标缓存中找到的次数；
- miss 后面的百分比：未命中占相关缓存访问的比例。

不同 CPU 对这些通用事件的映射可能不同，所以这里重点看趋势，不把某个具体数字
当作绝对真理。

在 VMware 中，这些事件很可能显示 `<not supported>`。这是虚拟机没有提供硬件
计数器，不影响前面的耗时比较，也不影响 `perf record/report` 热点实验。

## 5. 从“感觉”变成“证据”

优化前可能只是猜测：“列访问大概不太好。”

实验后形成证据链：

```text
相同数据、相同计算
→ 只改变访问顺序
→ 列访问耗时更长
→ cache miss 指标也更差（硬件支持时）
→ 内存局部性是合理解释
```

性能分析的价值不只是获得数字，而是用可重复实验验证解释。

如果计数器显示 `<not supported>`，仍然可以比较运行时间；以后在原生 Linux
机器上再补做硬件事件实验。

下一步：[第 5 课：分析自己的程序](05-your-program.md)。
