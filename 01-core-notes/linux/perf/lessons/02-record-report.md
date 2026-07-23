# 第 2 课：寻找最耗 CPU 的函数

`perf stat` 告诉我们程序整体用了多少资源，但不会直接指出哪一个函数最忙。这一课
学习 `perf record` 和 `perf report`。

## 1. 先理解“采样”

假设程序运行 1 秒，`perf` 每隔一小段时间看一眼 CPU 正在执行哪个函数：

```text
第 1 次观察：hot_function
第 2 次观察：hot_function
第 3 次观察：cold_function
第 4 次观察：hot_function
```

4 次观察中有 3 次落在 `hot_function`，于是我们推测它占用了大约 75% 的 CPU
时间。这就是 **采样**。

它不是给每次函数调用精确计时，而是通过大量样本估计时间分布。

## 2. 录制采样数据

先建立保存结果的目录：

```bash
mkdir -p results
```

- `mkdir` 表示创建目录；
- `-p` 表示目录已经存在时不要报错；
- `results` 是目录名。

开始采样：

```bash
perf record -o results/cpu.data -- ./build/perf_lab cpu
```

逐段解释：

```text
perf record             运行程序并采样
-o results/cpu.data     output，把结果写入这个文件
--                      perf 自己的选项到这里结束
./build/perf_lab cpu    接下来才是被分析的程序及其参数
```

`--` 很重要。它帮助命令行工具区分“给 perf 的参数”和“给目标程序的参数”。

成功后会看到：

```text
[ perf record: Captured and wrote ... results/cpu.data (... samples) ]
```

检查文件：

```bash
ls -lh results/cpu.data
```

`-h` 表示使用 KB、MB 等更容易阅读的单位显示文件大小。

## 3. 打开报告

```bash
perf report -i results/cpu.data
```

逐段解释：

- `report`：查看已经保存的采样报告；
- `-i`：input，指定输入文件；
- `results/cpu.data`：上一条命令产生的文件。

这会打开终端交互界面。先只找两列：

```text
Overhead   Symbol
```

- `Symbol` 通常是函数名；
- `Overhead` 是样本占比，可以近似理解为 CPU 时间占比。

你应该能在前几名看到 `hot_function`。按 `q` 退出。

## 4. 为什么它叫热点？

CPU 经常被采样到正在执行的函数叫 **热点函数（hotspot）**。

热点不等于 bug。一个视频编码程序的编码函数很热可能完全正常。热点的意义是：
如果你确实需要优化程序，它通常是最值得优先调查的位置。

如果一个只占 1% CPU 时间的函数变快一倍，整个程序最多也只改善约 0.5%。这也是
为什么我们不应该凭感觉随便优化。

## 5. 不打开交互界面

```bash
perf report --stdio -i results/cpu.data
```

- `--stdio` 表示把报告作为普通文本打印到终端；
- 这种形式适合复制结果、写脚本或远程操作。

输出太长时可以只看前 30 行：

```bash
perf report --stdio -i results/cpu.data | head -n 30
```

`|` 叫管道：把左边命令的输出交给右边命令。`head -n 30` 只保留开头 30 行。

## 本课检查

请先自己回答，再看答案：

1. `record` 和 `report` 为什么要配合使用？
2. `Overhead 70%` 是否代表精确测量到函数执行了 70% 的时间？
3. `--` 是什么分界线？

答案：`record` 负责采集并保存，`report` 负责阅读；Overhead 是基于采样的估计；
`--` 把 perf 的参数和被分析程序分开。

下一步：[第 3 课：观察调用链](03-call-graph.md)。

