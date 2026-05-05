# Git Basic Concepts

## 1. Git 到底在管理什么？

刚开始学 Git 的时候，很容易把 Git 理解成“把本地文件上传到 GitHub 的工具”。

但这个理解不太准确。

Git 真正管理的是：

> 项目的提交历史，而不仅仅是文件本身。

每一次提交 `commit`，都可以理解为项目在某个时刻的一次快照。Git 会通过这些提交记录，把项目从最初状态到当前状态的变化过程保存下来。

例如：

```text
A -- B -- C
```

可以理解为项目经历了三次提交：

- `A`：第一次提交
- `B`：第二次提交
- `C`：第三次提交

Git 关心的是这些提交之间的关系，而不只是当前文件夹里有哪些文件。

---

## 2. commit 是什么？

`commit` 是 Git 里最核心的概念。

当我们执行：

```bash
git add .
git commit -m "message"
```

Git 就会生成一个新的提交记录。

每个 commit 通常包含这些信息：

```text
1. 当前项目文件的快照
2. 提交说明 message
3. 作者信息
4. 提交时间
5. 父提交 parent commit
6. 一个唯一的 commit hash
```

commit hash 可以理解为这个提交的身份证。

例如：

```bash
git log --oneline
```

可能会看到：

```text
164b0cf init
a8c912e docs: update README
```

其中 `164b0cf`、`a8c912e` 就是 commit hash 的简写。

---

## 3. branch 是什么？

分支 `branch` 本质上是一个指针，它指向某一个 commit。

例如：

```text
A -- B -- C
          |
        main
```

这表示：

```text
main 分支当前指向 C 这个提交
```

当我们在 main 分支上继续提交一次：

```bash
git add .
git commit -m "docs: update README"
```

历史会变成：

```text
A -- B -- C -- D
               |
             main
```

也就是说，`main` 分支这个指针从 `C` 移动到了 `D`。

所以分支并不是很神秘的东西，它本质上就是一个指向 commit 的名字。如果是是多个分支，也只是多个指针指向不同的快照commit。

---

## 4. HEAD 是什么？

`HEAD` 表示当前所在的位置。

通常情况下，`HEAD` 指向当前分支，而当前分支再指向某个 commit。

例如：

```text
HEAD -> main -> C
```

表示：

```text
当前位于 main 分支
main 分支当前指向 C 这个提交
```

如果继续提交一次：

```text
A -- B -- C -- D
               |
             main
             ↑
            HEAD
```

此时 `HEAD` 仍然指向 `main`，而 `main` 指向最新提交 `D`。

---

## 5. 工作区、暂存区、本地仓库

Git 本地大致可以分成三个区域：

```text
工作区 working directory
暂存区 staging area
本地仓库 local repository
```

### 5.1 工作区

工作区就是我们平时看到的项目文件夹。

例如：

```text
Cpp-Backend-Lab/
├── README.md
└── 09-engineering-tools/
```

当我们修改文件时，最开始只是改了工作区。

---

### 5.2 暂存区

执行：

```bash
git add README.md
```

文件修改会进入暂存区。

暂存区可以理解为：

> 下一次 commit 准备提交的内容。

---

### 5.3 本地仓库

执行：

```bash
git commit -m "docs: update README"
```

暂存区的内容会被提交到本地仓库，形成一个新的 commit。

整个流程可以理解为：

```text
修改文件
  ↓
git add
  ↓
进入暂存区
  ↓
git commit
  ↓
进入本地仓库，生成 commit
```

---

## 6. remote 是什么？

`remote` 是远程仓库地址。

例如：

```bash
git remote add origin git@github.com:zthinedge/Cpp-Backend-Lab.git
```

这里：

```text
origin
```

只是远程仓库地址的别名。

真正的远程地址是：

```text
git@github.com:zthinedge/Cpp-Backend-Lab.git
```

查看当前仓库绑定了哪些远程地址：

```bash
git remote -v
```

可能会看到：

```text
origin  git@github.com:zthinedge/Cpp-Backend-Lab.git (fetch)
origin  git@github.com:zthinedge/Cpp-Backend-Lab.git (push)
```

这表示本地仓库已经和 GitHub 上的这个远程仓库建立了连接。

---

## 7. push 的本质

很多人会以为：

> git push 就是把本地文件上传到 GitHub。

但更准确地说：

> git push 是把本地分支上的 commit 推送到远程，并尝试移动远程分支指针。

例如远程仓库现在是：

```text
A
|
main
```

本地仓库是：

```text
A -- B
     |
   main
```

这说明本地是在远程已有提交 `A` 的基础上，又新增了一个提交 `B`。

这时候执行：

```bash
git push origin main
```

GitHub 会接受，因为这是在远程已有历史后面继续追加提交。

push 成功后，远程也会变成：

```text
A -- B
     |
   main
```

这种情况叫做 fast-forward，可以理解为远程分支指针从 `A` 向前移动到了 `B`。

---

## 8. 为什么远程有 README，本地又 git init 会出问题？

这是我之前遇到的一个典型问题。

在 GitHub 创建仓库时，如果勾选了：

```text
Add a README file
```

GitHub 会自动在远程仓库生成一个初始提交。

远程仓库会变成：

```text
A  add README
|
main
```

然后如果我在本地空文件夹里又执行：

```bash
git init
echo "# Cpp-Backend-Lab" > README.md
git add .
git commit -m "init"
```

本地会产生另一个新的初始提交：

```text
B  init
|
main
```

这时远程和本地的历史关系是：

```text
远程：A

本地：B
```

虽然它们绑定的是同一个 GitHub 地址，但是 `A` 和 `B` 不是同一条提交历史。

它们没有共同祖先。

所以直接执行：

```bash
git push -u origin main
```

GitHub 会拒绝。

因为 GitHub 看到的是：

```text
远程 main 当前指向 A
本地 main 当前指向 B
```

如果直接接受本地的 `B`，远程的 `A` 就会被覆盖掉。

为了防止误覆盖别人的代码或者远程已有历史，Git 默认不允许这样 push。

---

## 9. README 文件本身特殊吗？

README 文件本身并不特殊。

真正特殊的是：

> GitHub 创建 README 时，会自动帮远程仓库生成一个 commit。

如果创建仓库时勾选了下面这些内容，也会产生同样的问题：

```text
README.md
LICENSE
.gitignore
```

因为它们都会让远程仓库不再是空仓库。

所以问题不是 README 文件，而是：

```text
远程仓库已经有提交了
```

---

## 10. 什么时候用 git init？

当 GitHub 上的远程仓库是空仓库时，可以使用 `git init`。

也就是说，GitHub 仓库里没有：

```text
README.md
LICENSE
.gitignore
任何 commit
```

这种情况下，本地可以这样操作：

```bash
cd Cpp-Backend-Lab

git init
git branch -M main
git remote add origin git@github.com:zthinedge/Cpp-Backend-Lab.git

echo "# Cpp-Backend-Lab" > README.md
git add .
git commit -m "init"
git push -u origin main
```

这时候远程仓库是空的，所以不会发生历史冲突。

---

## 11. 什么时候用 git clone？

当 GitHub 远程仓库已经有提交时，应该使用 `git clone`。

比如创建仓库时已经勾选了 README：

```text
远程：
A  add README
|
main
```

这时应该先 clone：

```bash
git clone git@github.com:zthinedge/Cpp-Backend-Lab.git
cd Cpp-Backend-Lab
```

clone 之后，本地就会先拿到远程已有的提交 `A`。

然后再修改文件并提交：

```bash
echo "# Cpp-Backend-Lab" > README.md
git add README.md
git commit -m "docs: update README"
git push
```

此时提交历史是：

```text
A -- B
```

本地是在远程已有的 `A` 后面新增了 `B`，所以 push 可以成功。

---

## 12. git pull 的本质

`git pull` 可以理解为两步操作：

```bash
git fetch
git merge
```

也就是说：

```bash
git pull origin main
```

大致等价于：

```bash
git fetch origin main
git merge origin/main
```

它的作用是：

```text
先把远程最新提交拿下来
再和本地当前分支合并
```

如果本地和远程是正常的同一条历史，pull 一般不会有什么问题。

但如果本地和远程是两条独立历史，比如：

```text
远程：A

本地：B
```

这时 Git 可能会报：

```text
fatal: refusing to merge unrelated histories
```

意思是：

```text
拒绝合并两条没有共同祖先的历史
```

这时可以使用：

```bash
git pull origin main --allow-unrelated-histories
```

不过对于刚创建的仓库，如果远程已经有 README，更推荐直接重新 clone，这样更干净。

---

## 13. 常见场景总结

### 13.1 GitHub 是空仓库

适合使用：

```bash
git init
```

完整流程：

```bash
cd repo

git init
git branch -M main
git remote add origin git@github.com:username/repo.git

echo "# repo" > README.md
git add .
git commit -m "init"
git push -u origin main
```

---

### 13.2 GitHub 已经有 README / LICENSE / .gitignore

适合使用：

```bash
git clone
```

完整流程：

```bash
git clone git@github.com:username/repo.git
cd repo

# 修改文件
git add .
git commit -m "docs: update notes"
git push
```

---

### 13.3 本地已经 git init，但远程也有 README

这时可能会报：

```text
Updates were rejected because the remote contains work that you do not have locally.
```

最简单的处理方式是重新 clone：

```bash
cd ..
rm -rf repo

git clone git@github.com:username/repo.git
cd repo
```

如果确认远程内容不重要，也可以强制覆盖：

```bash
git push -u origin main --force
```

但 `--force` 会覆盖远程历史，正式项目不要随便使用。

---

## 14. 一句话记忆

Git 不是按仓库地址判断能不能 push，而是按提交历史判断。

可以记成：

```text
远程空仓库：git init
远程已有提交：git clone
```

或者更具体一点：

```text
GitHub 创建仓库时没勾 README：本地 git init
GitHub 创建仓库时勾了 README：直接 git clone
```

本质原因是：

```text
push 不是简单上传文件，而是把远程分支指针移动到本地 commit。
如果远程已经有本地没有的 commit，Git 会拒绝直接 push，防止覆盖远程历史。
```
