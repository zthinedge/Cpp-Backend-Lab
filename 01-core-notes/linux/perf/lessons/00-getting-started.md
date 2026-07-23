# 第 0 课：准备与基础概念

## 1. 我们究竟在分析什么？

先区分三个词：

- **源代码**：人写的 `.cpp` 文件，例如 `examples/perf_lab.cpp`。
- **程序**：源代码编译后得到的、可以运行的文件，例如 `build/perf_lab`。
- **进程**：程序运行起来以后，Linux 对这次运行的称呼。

可以把它理解为：

```text
菜谱（源代码）→ 做好的菜（程序）→ 正在吃这道菜（进程）
```

`perf` 主要观察“正在运行的进程”，帮助我们判断 CPU 时间花在了哪里。

## 2. 打开正确的目录

在终端输入：

```bash
cd /home/zym/Share/Cpp-Backend-Lab/01-core-notes/linux/perf
```

逐段解释：

- `cd` 是 change directory 的缩写，意思是“切换目录”。
- 后面一长串文字是目标目录的完整路径。
- 命令执行成功时通常没有输出。Linux 中“没有输出”经常代表没有出错。

确认自己在哪里：

```bash
pwd
```

`pwd` 会打印当前目录。预期输出：

```text
/home/zym/Share/Cpp-Backend-Lab/01-core-notes/linux/perf
```

## 3. 检查工具

运行：

```bash
./scripts/check-env.sh
```

逐段解释：

- `.` 表示当前目录。
- `/` 用于分隔目录。
- `./scripts/check-env.sh` 的完整意思是“运行当前目录下 `scripts` 目录中的
  `check-env.sh` 文件”。

这个脚本会检查：

- `perf` 是否存在；
- C++ 编译器 `g++` 是否存在；
- 当前 Linux 内核版本；
- 系统是否限制普通用户使用性能计数器。

看到 `perf version ...` 和 `g++ ...` 就可以继续。此时不需要理解
`perf_event_paranoid`，遇到权限错误时再回来处理。

## 4. 编译实验程序

运行：

```bash
make
```

`make` 是一个构建工具。它读取当前目录中的 `Makefile`，按照里面写好的命令，
把 `examples/perf_lab.cpp` 编译成 `build/perf_lab`。

输出中会出现一条以 `g++` 开头的长命令。暂时只需要知道：

- `g++`：C++ 编译器；
- `-O2`：让编译器做常见的性能优化；
- `-g`：在程序中保留源码、行号和函数名信息，方便 `perf` 展示；
- `-o build/perf_lab`：把最终程序放到这个位置。

确认程序已生成：

```bash
ls -l build/perf_lab
```

`ls` 用于列出文件，`-l` 表示显示详细信息。

## 5. 不使用 perf，先运行一次

```bash
./build/perf_lab cpu
```

逐段解释：

- `./build/perf_lab`：运行刚刚编译的程序；
- `cpu`：传给程序的参数，告诉它运行 CPU 热点实验。

预期看到类似：

```text
CPU hotspot: 470 ms (ignore=...)
```

`470 ms` 表示约 470 毫秒，也就是 0.47 秒。你的数字不同是正常的。

这里故意进行大量无意义计算，是为了制造一个容易被观察到的 CPU 热点。

## 6. perf 是什么？

`perf` 是 Linux 提供的一组性能分析工具。现阶段只认识三个子命令：

```text
perf stat     查看一次运行的整体统计
perf record   运行程序并保存采样数据
perf report   阅读 record 保存的数据
```

类比体检：

```text
stat   = 先看体检指标是否异常
record = 做一次更详细的检查并保存结果
report = 打开详细检查报告
```

下一步：[第 1 课：第一次性能体检](01-perf-stat.md)。

