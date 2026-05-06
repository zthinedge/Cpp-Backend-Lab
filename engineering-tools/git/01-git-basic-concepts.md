# Git Basic Concepts

## 1. 这篇文章只讲什么？

这篇文章只负责建立 Git 的基础心智模型：

```text
commit
branch
HEAD
工作区
暂存区
本地仓库
```

---

## 2. Git 到底在管理什么？

刚开始学 Git 时，很容易把它理解成“把本地文件上传到 GitHub 的工具”。

这个说法不够准确。

Git 真正管理的是：

> 项目的提交历史，而不仅仅是当前文件夹里的文件。

每一次提交 `commit`，都可以理解为项目在某个时刻的一次快照。

例如：

```text
A -- B -- C
```

可以理解为项目经历了三次提交：

- `A`：第一次提交
- `B`：第二次提交
- `C`：第三次提交

Git 真正在意的，是这些提交之间怎样连接，而不是单独某一个文件现在长什么样。

---

## 3. commit 是什么？

`commit` 是 Git 最核心的对象。

当我们执行：

```bash
git add .
git commit -m "message"
```

Git 会生成一条新的提交记录。

一个 `commit` 通常包含：

```text
1. 当前项目文件的快照
2. 提交说明 message
3. 作者信息
4. 提交时间
5. 父提交 parent commit
6. 一个唯一的 commit hash
```

例如执行：

```bash
git log --oneline
```

可能会看到：

```text
164b0cf init
a8c912e docs: update README
```

其中 `164b0cf`、`a8c912e` 就是 commit hash 的简写，可以理解成每次提交的身份证。

---

## 4. branch 是什么？

分支 `branch` 本质上是一个指向某个 `commit` 的名字。

例如：

```text
A -- B -- C
          |
        main
```

这表示 `main` 当前指向提交 `C`。

如果继续提交一次：

```text
A -- B -- C -- D
               |
             main
```

本质上只是 `main` 这个指针从 `C` 移动到了 `D`。

所以分支并不神秘，它不是一份“完全独立的新仓库”，而只是提交历史上的一个可移动指针。

---

## 5. HEAD 是什么？

`HEAD` 表示“我现在在哪儿”。

通常情况下，`HEAD` 会指向当前分支，而当前分支再指向某个提交：

```text
HEAD -> main -> C
```

这表示：

```text
当前在 main 分支上
main 当前指向提交 C
```

如果继续提交一次：

```text
A -- B -- C -- D
               |
             main
             ↑
            HEAD
```

此时 `HEAD` 仍然跟着 `main`，而 `main` 已经前进到了 `D`。

---

## 6. 工作区、暂存区、本地仓库

Git 本地最重要的三个区域是：

```text
工作区 working directory
暂存区 staging area
本地仓库 local repository
```

### 6.1 工作区

工作区就是我们平时看到的项目文件夹。

你改代码、改文档、删文件，最开始都只发生在工作区。

---

### 6.2 暂存区

执行：

```bash
git add README.md
```

文件修改会进入暂存区。

暂存区可以理解为：

> 下一次 commit 准备提交的内容。

---

### 6.3 本地仓库

执行：

```bash
git commit -m "docs: update README"
```

暂存区中的内容会进入本地仓库，形成一个新的 `commit`。

整个过程可以理解成：

```text
修改文件
  ↓
工作区
  ↓
git add
  ↓
暂存区
  ↓
git commit
  ↓
本地仓库
```


## 7.总结

Git 不是简单在“管文件”，而是在“管理项目的提交历史”,是通过提交历史、分支指针和暂存区来管理项目变化的。

`commit` 是快照，`branch` 是指针，`HEAD` 表示当前位置，工作区和暂存区决定了哪些修改会进入下一次提交。
