# Git Ignore

## 1. 这篇文章解决什么问题？

一个 C++ 后端项目里，经常会产生很多不应该提交到 Git 仓库的文件。

例如：

```text
编译产物
临时文件
IDE 配置
本地环境文件
日志文件
第三方依赖缓存
```

如果这些文件都被提交，会带来很多问题：

```text
1. 仓库变大
2. diff 里充满无关内容
3. 不同机器的本地配置互相污染
4. 编译产物和源码混在一起
5. 有泄露本地路径或密钥的风险
```

这篇只讲 `.gitignore` 的使用习惯。

---

## 2. .gitignore 是什么？

`.gitignore` 是一个普通文本文件，用来告诉 Git：

```text
哪些未跟踪文件不需要出现在 git status 里
哪些文件不应该被 git add 自动加入仓库
```

例如：

```gitignore
build/
*.log
.env
```

表示：

```text
忽略 build 目录
忽略所有 .log 文件
忽略 .env 文件
```

注意这里有一个关键词：

```text
未跟踪文件
```

`.gitignore` 对已经被 Git 跟踪的文件不会自动生效。

---

## 3. 最常见的忽略规则

忽略某个文件：

```gitignore
.env
```

忽略某个目录：

```gitignore
build/
```

忽略某种后缀：

```gitignore
*.o
*.log
```

忽略任意层级下的某个目录：

```gitignore
.cache/
```

取消忽略某个文件：

```gitignore
*.conf
!important.conf
```

其中 `!` 表示反向规则。

---

## 4. 路径规则怎么理解？

假设项目结构是：

```text
project/
  .gitignore
  build/
  src/
  logs/
```

如果 `.gitignore` 里写：

```gitignore
build/
```

表示忽略项目根目录下的 `build` 目录。

如果写：

```gitignore
*.log
```

表示忽略任意目录下的 `.log` 文件。

如果写：

```gitignore
/logs/
```

开头的 `/` 表示从 `.gitignore` 所在目录开始匹配。

大多数个人项目里，不需要把规则写得太复杂，先覆盖常见产物即可。

---

## 5. C++ 项目通常应该忽略什么？

C++ 项目最常见的是编译目录和中间产物：

```gitignore
build/
cmake-build-*/
CMakeFiles/
CMakeCache.txt
compile_commands.json
Makefile
*.o
*.obj
*.a
*.so
*.dll
*.dylib
*.exe
```

其中：

```text
build/                常见 out-of-source build 目录
cmake-build-*/        CLion 常见构建目录
CMakeFiles/           CMake 内部目录
CMakeCache.txt        CMake 缓存
compile_commands.json 编译数据库，有些项目会选择保留
```

`compile_commands.json` 是否忽略要看项目习惯。

如果团队希望给 clangd 使用统一编译数据库，也可以不忽略，或者用软链接方式处理。

---

## 6. IDE 和编辑器文件

不同编辑器会生成自己的配置。

常见规则：

```gitignore
.vscode/
.idea/
*.swp
*.swo
.DS_Store
```

但这里要稍微谨慎。

例如 `.vscode/settings.json` 有时可能包含团队共享配置，有时又只是个人机器配置。

比较稳的做法是：

```text
项目通用配置可以提交
个人本地配置不要提交
```

如果不确定，先不要把整个目录一股脑提交进仓库。

---

## 7. 日志、缓存和临时文件

常见规则：

```gitignore
*.log
logs/
.cache/
tmp/
temp/
```

这些文件通常有几个特点：

```text
会频繁变化
对别人复现项目没有帮助
容易让 git diff 变得很吵
```

如果某个目录需要保留结构，但不提交里面的内容，可以这样写：

```gitignore
logs/*
!logs/.gitkeep
```

然后提交一个空的：

```text
logs/.gitkeep
```

这样仓库里能保留目录，运行时日志文件又不会被提交。

---

## 8. 敏感信息一定不要提交

下面这些文件通常应该忽略：

```gitignore
.env
.env.local
*.pem
*.key
*.crt
```

原因很简单：

```text
它们可能包含数据库密码
API token
私钥
本地服务地址
个人账号配置
```

如果需要给别人参考配置格式，更推荐提交模板文件：

```text
.env.example
config.example.yaml
```

模板里只放占位内容：

```text
DB_HOST=127.0.0.1
DB_PASSWORD=change_me
```

不要把真实密码写进去。

---

## 9. 为什么 .gitignore 对已提交文件没生效？

这是新手最常见的问题。

如果某个文件已经被 Git 跟踪，例如：

```text
config/local.yaml
```

后来你在 `.gitignore` 里加：

```gitignore
config/local.yaml
```

它仍然会出现在 `git status` 中。

原因是：

```text
.gitignore 只阻止未跟踪文件进入 Git
不会让已经被跟踪的文件自动退出 Git
```

这时需要执行：

```bash
git rm --cached config/local.yaml
```

含义是：

```text
从 Git 暂存区和后续版本管理中移除
但保留本地工作区文件
```

然后提交：

```bash
git add .gitignore
git commit -m "chore(git): ignore local config"
```

---

## 10. git rm --cached 和 rm 的区别

普通删除文件：

```bash
rm config/local.yaml
```

表示：

```text
从本地磁盘删除文件
```

而：

```bash
git rm --cached config/local.yaml
```

表示：

```text
让 Git 不再跟踪这个文件
但文件仍然留在本地目录里
```

所以处理“已经提交过，但以后不想跟踪”的配置文件时，通常用 `git rm --cached`。

---

## 11. 检查某个文件为什么被忽略

如果你不知道某个文件为什么被忽略，可以执行：

```bash
git check-ignore -v path/to/file
```

它会告诉你是哪条规则匹配了这个文件。

例如：

```text
.gitignore:12:build/ build/server
```

表示：

```text
.gitignore 第 12 行的 build/ 规则忽略了 build/server
```

这个命令很适合排查 `.gitignore` 写多了之后的困惑。

---

## 12. 一个适合 C++ 后端项目的示例

可以从下面这个版本开始：

```gitignore
# Build output
build/
cmake-build-*/
CMakeFiles/
CMakeCache.txt
Makefile

# Object and binary files
*.o
*.obj
*.a
*.so
*.dll
*.dylib
*.exe

# Logs and runtime files
*.log
logs/
tmp/

# Local environment
.env
.env.local
*.pem
*.key

# Editor and OS files
.vscode/
.idea/
*.swp
*.swo
.DS_Store
```

然后根据项目真实情况慢慢调整。

`.gitignore` 不需要一开始就写得特别完美，关键是不要把明显不该进仓库的东西提交进去。

---

## 13. 一句话总结

`.gitignore` 用来阻止未跟踪文件进入仓库，最适合忽略编译产物、日志、缓存、本地配置和敏感信息。

如果文件已经被 Git 跟踪，单独写进 `.gitignore` 不够，还需要 `git rm --cached` 让它退出版本管理。
