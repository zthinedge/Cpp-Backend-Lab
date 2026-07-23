# 第 5 课：分析自己的 C++ 程序

现在把前面的知识迁移到真实项目。

## 1. 准备一个适合分析的程序

编译时建议开启优化，同时保留调试信息和栈帧：

```bash
g++ -O2 -g -fno-omit-frame-pointer your_program.cpp -o your_program
```

- `-O2`：让代码形态接近实际发布版本；
- `-g`：让 perf 能把地址对应到函数名、源码和行号；
- `-fno-omit-frame-pointer`：更容易得到完整调用栈；
- `-o your_program`：输出程序名。

如果项目使用 CMake，不需要改成上面的单文件命令。核心是让当前构建类型同时具备
优化和调试信息，常见选择是：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

## 2. 使用有代表性的负载

不要只让服务器启动后立刻退出。分析 HTTP 服务时，应该在 `perf` 采样期间发送
接近真实情况的请求；分析数据库组件时，应该准备足够的数据和操作。

错误输入会产生正确但没有价值的报告。

## 3. 固定分析流程

第一步，运行程序确认功能正常：

```bash
./your_program
```

第二步，查看整体统计：

```bash
perf stat -e task-clock,cycles,instructions,branches,branch-misses \
  ./your_program
```

第三步，采集函数和调用链：

```bash
perf record -g -o results/my-program.data -- ./your_program
```

第四步，阅读报告：

```bash
perf report -i results/my-program.data
```

第五步，验证优化结果：修改代码后，在相同机器、相同输入、相同编译配置下重复
测量。既要看热点是否下降，也要看整个程序的完成时间是否改善。

## 4. 长时间运行的服务器

服务器已经运行时，先找 PID：

```bash
pgrep -a your_program
```

PID 是 Linux 分配给进程的数字编号。假设输出中的 PID 是 `12345`，采样 10 秒：

```bash
perf record -g -p 12345 -- sleep 10
```

逐段解释：

- `-p 12345`：只观察这个 PID；
- `sleep 10`：等待 10 秒后结束采样；
- 采样期间需要给服务器发送请求，否则报告可能只显示它在等待。

然后：

```bash
perf report
```

## 5. 新手最容易犯的错误

- 使用 Debug 版本分析，得到与生产优化版本差异很大的热点；
- 程序没有真实工作负载，采样到的只有等待；
- 只测一次，把偶然波动当作优化效果；
- 只看到某个函数名就改代码，没有查看调用链；
- 微观指标变好，却没有确认整体耗时是否下降；
- 同时改变代码、输入和编译选项，导致无法判断是谁产生了效果。

## 6. 你已经学会了什么？

```text
perf stat
   ↓ 发现整体问题或建立基线
perf record -g
   ↓ 保存函数和调用链样本
perf report
   ↓ 找到热点及其来源
修改一个因素
   ↓
使用相同条件重新测量
```

你不需要背下所有命令。真正重要的是形成“先测量、再解释、最后验证”的习惯。

