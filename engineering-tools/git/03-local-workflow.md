# Git Local Workflow

## 1. 这篇文章解决什么问题？

这篇文章只讲 Git 在本地的日常提交流程。


日常开发中，最常见的流程是：

```text
修改代码
  ↓
查看修改状态
  ↓
查看具体改了什么
  ↓
添加到暂存区
  ↓
提交到本地仓库
  ↓
必要时再推送到远程
```

对应的 Git 命令是：

```bash
git status
git diff
git add .
git commit -m "message"
git push
```

其中前四步是这篇的重点。

如果是第一次绑定远程、第一次推送、或者 `git push -u origin main` 这类远程问题，统一看 `02-github-ssh-remote.md`。

---

## 2. 查看当前仓库状态

最常用命令：

```bash
git status
```

它可以告诉我们：

```text
1. 当前在哪个分支
2. 哪些文件被修改了
3. 哪些文件还没有被 Git 跟踪
4. 哪些文件已经加入暂存区
5. 当前分支是否领先或落后远程分支
```

例如：

```text
Untracked files:
  03-local-workflow.md
```

表示这个文件还没有被 Git 管理。

如果看到：

```text
Changes not staged for commit:
  modified: README.md
```

表示文件已经被修改，但还没有加入暂存区。

如果看到：

```text
Changes to be committed:
  modified: README.md
```

表示文件已经加入暂存区，下一次 commit 会提交这些内容。

---

## 3. 查看具体修改内容

查看工作区和上一次提交之间的区别：

```bash
git diff
```

这个命令可以看到自己当前到底改了哪些内容。

例如修改了 README.md，执行：

```bash
git diff README.md
```

可以只查看这个文件的修改。

如果文件已经 `git add` 到暂存区，想看暂存区里的修改，可以使用：

```bash
git diff --cached
```

也可以写成：

```bash
git diff --staged
```

常用习惯：

```bash
git status
git diff
```

先看有哪些文件改了，再看具体改了什么。

---

## 4. 添加文件到暂存区

添加单个文件：

```bash
git add README.md
```

添加某个目录：

```bash
git add 09-engineering-tools/git/
```

添加当前目录下所有修改：

```bash
git add .
```

暂存区可以理解为：

```text
下一次 commit 准备提交的内容
```

所以 `git add` 并不是最终提交，只是先把修改放进暂存区。

更推荐的习惯是：

```bash
git status
git add README.md
git commit -m "message"
```

也就是说，优先按文件或目录选择性暂存，而不是无脑把所有改动一起塞进去。

---

## 5. 提交到本地仓库

执行：

```bash
git commit -m "message"
```

例如：

```bash
git commit -m "docs(git): add local workflow note"
```

执行 commit 后，Git 会在本地生成一个新的提交记录。

需要注意：

```text
commit 只是提交到本地仓库，还没有同步到 GitHub。
```

如果想同步到远程，还需要执行：

```bash
git push
```


---

## 6. 查看提交历史

查看完整提交历史：

```bash
git log
```

查看简洁提交历史：

```bash
git log --oneline
```

例如：

```text
a8c912e docs(git): add local workflow note
164b0cf init
```

如果想看图形化分支历史，可以使用：

```bash
git log --oneline --graph --decorate --all
```

这个命令可以更清楚地看到分支和提交关系。

---

## 7. 推送到远程仓库

提交到本地后，执行：

```bash
git push
```

如果这里报错，不要在这篇里硬记各种远程参数，直接回到 `02` 看：

```text
remote 绑定
push -u
origin/main
fetch / pull / push
```

---

## 8. 日常开发标准流程

在个人项目中，最常用的一套流程是：

```bash
# 1. 查看当前状态
git status

# 2. 查看具体修改
git diff

# 3. 添加修改到暂存区
git add path/to/file

# 4. 提交到本地仓库
git commit -m "message"

# 5. 推送到 GitHub
git push
```

可以简化理解为：

```text
status → diff → add → commit → push
```

---

## 9. 为什么不要一上来就 git add .？

`git add .` 很方便，但也最容易把不该提交的内容一起带进去。

例如：

```text
build/
*.o
*.log
.vscode/
```

这些一般不应该提交到 GitHub。

所以更稳妥的顺序是：

```bash
git status
git diff
git add path/to/file
```

如果某些文件不想被 Git 管理，应该写入 `.gitignore`。

---

## 10. 只提交部分文件

有时候一次修改了很多文件，但只想提交其中一部分。

例如只提交 Git 笔记：

```bash
git add 09-engineering-tools/git/03-local-workflow.md
git commit -m "docs(git): add local workflow note"
```

如果只想提交 README：

```bash
git add README.md
git commit -m "docs: update README"
```

这样可以让每次 commit 更清晰。

如果一次改了代码、文档、测试三类内容，尽量拆成多个 commit，而不是一次性全提交。

---

## 11. 一次 commit 应该包含什么？

一个好的 commit 应该尽量只做一件事。

不推荐：

```text
一个 commit 里同时修改 README、实现 RESP parser、修复 dict bug、添加测试
```

推荐：

```text
一个 commit 只做一类修改
```

例如：

```bash
git commit -m "feat(resp): implement array parser"
git commit -m "fix(dict): handle lookup during rehash"
git commit -m "docs(ttl): add expire design note"
git commit -m "test(resp): add incomplete packet cases"
```

这样以后查看历史、回退代码、定位问题都会更清楚。

---

## 12. commit message 只先记一个轻量规则

推荐格式：

```text
type(scope): description
```

例如：

```bash
git commit -m "docs(git): add local workflow note"
git commit -m "feat(resp): implement parser"
git commit -m "fix(dict): fix rehash lookup"
git commit -m "test(aof): add recovery cases"
```

常见 type：

```text
feat      新功能
fix       修复 bug
docs      文档
test      测试
refactor  重构
perf      性能优化
build     构建相关
chore     杂项
```

这一篇只先记到够日常使用的程度。

更详细的 commit message 规范，后面可以单独整理到：

```text
09-commit-message.md
```

---

## 13. 本地开发示例流程

假设现在要写 RESP parser 文档。

可以这样操作：

```bash
# 查看当前状态
git status

# 新建或修改文档
vim 03-tinyredis/docs/resp-parser.md

# 查看修改
git diff

# 添加文件
git add 03-tinyredis/docs/resp-parser.md

# 提交
git commit -m "docs(resp): add parser design note"

# 推送
git push
```

如果是实现功能：

```bash
git status
git diff

git add src/resp/
git commit -m "feat(resp): implement incremental parser"

git push
```

如果是补测试：

```bash
git add tests/resp/
git commit -m "test(resp): add incomplete packet cases"
git push
```

---

## 14. 常用命令总结

```bash
# 查看状态
git status

# 查看工作区修改
git diff

# 查看暂存区修改
git diff --cached

# 添加单个文件
git add README.md

# 添加当前目录所有修改
git add .

# 提交
git commit -m "message"

# 查看历史
git log
git log --oneline

# 推送
git push
```

---

## 15. 我的日常使用习惯

个人项目中，推荐每次提交前都按下面顺序检查：

```bash
git status
git diff
git add path/to/file
git status
git commit -m "message"
git push
```

其中第二次 `git status` 是为了确认：

```text
1. 哪些文件已经进入暂存区
2. 有没有不该提交的文件
3. commit 是否只包含当前这次想提交的内容
```

---

## 16. 总结

Git 本地开发流程可以记成：

```text
看状态 → 看修改 → 加入暂存区 → 本地提交 → 推送远程
```

对应命令是：

```bash
git status
git diff
git add path/to/file
git commit -m "message"
git push
```
