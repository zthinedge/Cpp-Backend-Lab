# Git Common Errors

## 1. 这篇文章解决什么问题？

Git 报错时，最容易让人慌的不是错误本身，而是不知道它在说哪一层出了问题。

常见错误包括：

```text
1. not a git repository
2. remote origin already exists
3. failed to push some refs
4. non-fast-forward
5. refusing to merge unrelated histories
6. Your local changes would be overwritten
7. detached HEAD
8. Permission denied (publickey)
```

这篇不追求覆盖所有错误，只整理个人 C++ 后端项目里最常见、最容易卡住的几类。

---

## 2. 先判断问题发生在哪一层

遇到错误时，先不要马上复制一堆命令乱试。

先判断它属于哪一类：

```text
仓库位置问题：当前目录是不是 Git 仓库
工作区问题：本地文件有没有未提交修改
暂存区问题：是不是 add 或 commit 状态不清楚
历史问题：本地和远程历史是否分叉
远程问题：remote、SSH、权限是否正确
```

固定先执行：

```bash
git status
git remote -v
git branch -vv
```

很多问题看完这三个命令就能定位大半。

---

## 3. not a git repository

报错类似：

```text
fatal: not a git repository (or any of the parent directories): .git
```

意思是：

```text
当前目录不是 Git 仓库
Git 往上找也没有找到 .git 目录
```

常见原因：

```text
1. 终端目录进错了
2. 项目还没有 git init
3. 只复制了源码文件，没有复制 .git 目录
```

处理方式：

```bash
pwd
ls -la
```

确认当前目录。

如果这是一个新项目，可以执行：

```bash
git init
```

如果这是 GitHub 上已有项目，更推荐：

```bash
git clone git@github.com:username/repo.git
```

---

## 4. remote origin already exists

报错类似：

```text
error: remote origin already exists.
```

意思是：

```text
当前仓库已经有一个叫 origin 的远程地址
不能重复添加同名 remote
```

先查看：

```bash
git remote -v
```

如果地址是对的，不需要再添加。

如果地址错了，可以改：

```bash
git remote set-url origin git@github.com:username/repo.git
```

如果确实想删掉重新加：

```bash
git remote remove origin
git remote add origin git@github.com:username/repo.git
```

一般更推荐 `set-url`，意图更明确。

---

## 5. Permission denied (publickey)

报错类似：

```text
git@github.com: Permission denied (publickey).
fatal: Could not read from remote repository.
```

意思是：

```text
GitHub 没有通过你的 SSH 身份验证
```

常见原因：

```text
1. 本机还没有生成 SSH key
2. 公钥没有添加到 GitHub
3. remote 地址写成了没有权限的仓库
4. 多个 GitHub 账号使用了错误的 key
```

先测试：

```bash
ssh -T git@github.com
```

如果 SSH 没配置好，回到 `02-github-ssh-remote.md` 按步骤处理。

---

## 6. failed to push some refs

报错类似：

```text
! [rejected] main -> main (fetch first)
error: failed to push some refs to 'github.com:username/repo.git'
```

通常表示：

```text
远程分支上有本地没有的提交
Git 不允许你直接覆盖远程历史
```

先执行：

```bash
git fetch origin
git log --oneline --graph --decorate --all
```

看清楚本地和远程历史。

常见处理：

```bash
git pull --rebase origin main
git push
```

或者：

```bash
git pull origin main
git push
```

具体用 merge 还是 rebase，可以回看 `05-merge-rebase-pull.md`。

不要一看到 push 被拒绝就马上：

```bash
git push --force
```

这可能覆盖别人已经推上去的提交。

---

## 7. non-fast-forward

报错类似：

```text
! [rejected] main -> main (non-fast-forward)
```

意思和上一节很接近：

```text
你的本地分支不能直接快进到远程分支
本地和远程历史已经不一致
```

常见情况：

```text
远程多了 README 或其他初始化提交
另一台电脑已经 push 过新提交
队友先 push 了更新
```

处理思路仍然是：

```text
先 fetch
再看历史
再决定 merge 或 rebase
最后 push
```

命令示例：

```bash
git fetch origin
git log --oneline --graph --decorate --all
git rebase origin/main
git push
```

如果发生冲突，按 `07-conflict-resolution.md` 处理。

---

## 8. refusing to merge unrelated histories

报错类似：

```text
fatal: refusing to merge unrelated histories
```

意思是：

```text
本地仓库和远程仓库不是同一条历史
Git 找不到共同祖先
```

常见原因：

```text
1. 本地 git init 后提交了一次
2. GitHub 仓库创建时也初始化了 README
3. 然后试图把两个独立历史 pull 到一起
```

更推荐的做法是：

```text
如果 GitHub 仓库已经有 README，优先 git clone
不要本地 git init 后再硬绑定
```

如果你确认就是要合并两段历史，可以执行：

```bash
git pull origin main --allow-unrelated-histories
```

然后解决可能出现的冲突。

但这不是日常首选方案，它更像是补救操作。

---

## 9. Your local changes would be overwritten

报错类似：

```text
error: Your local changes to the following files would be overwritten by checkout:
```

或者：

```text
error: Your local changes would be overwritten by merge
```

意思是：

```text
你当前工作区有未提交修改
如果继续 checkout、merge 或 pull，这些修改可能被覆盖
```

先看：

```bash
git status
```

常见处理方式有三种。

如果修改应该保留并提交：

```bash
git add .
git commit -m "wip: save current changes"
```

如果只是临时保存：

```bash
git stash push -m "save local changes"
```

如果确定不要这些修改：

```bash
git restore .
```

具体选哪种，取决于这些修改还有没有价值。

---

## 10. detached HEAD

看到：

```text
HEAD detached at a8c912e
```

意思是：

```text
你当前没有站在某个分支上
而是直接站在某个 commit 上
```

这通常发生在：

```bash
git checkout a8c912e
```

此时如果你只是想查看旧版本，没问题。

如果你在 detached HEAD 状态下继续提交，新提交可能没有分支名字保护。

如果想保留这些提交，立刻创建分支：

```bash
git switch -c debug/old-version
```

如果只是看完旧版本想回到 main：

```bash
git switch main
```

---

## 11. pathspec did not match

报错类似：

```text
error: pathspec 'feature/cache' did not match any file(s) known to git
```

常见原因：

```text
1. 分支名写错了
2. 本地还没有这个远程分支信息
3. 当前命令把参数当成了文件或分支，但 Git 找不到
```

先看本地分支：

```bash
git branch
```

再看所有分支：

```bash
git branch -a
```

如果远程确实有这个分支，先更新远程信息：

```bash
git fetch origin
```

然后切换：

```bash
git switch feature/cache
```

如果本地还没有这个分支，可以基于远程创建：

```bash
git switch -c feature/cache origin/feature/cache
```

---

## 12. push 时提示 upstream 没设置

报错可能类似：

```text
fatal: The current branch feature/cache has no upstream branch.
```

意思是：

```text
当前本地分支还没有关联远程分支
Git 不知道 git push 默认要推到哪里
```

第一次推送新分支时执行：

```bash
git push -u origin feature/cache
```

`-u` 的作用是建立 upstream 关系。

以后在这个分支上就可以直接：

```bash
git push
git pull
```

---

## 13. 一个通用排查流程

遇到 Git 错误时，可以先按这个顺序来：

```bash
pwd
git status
git branch -vv
git remote -v
git log --oneline --graph --decorate --all
```

然后问自己：

```text
我现在在哪个目录？
我现在在哪个分支？
工作区是否干净？
远程地址是否正确？
本地和远程历史是否分叉？
```

大多数 Git 问题都可以从这几个问题里找到入口。

---

## 14. 一句话总结

Git 报错时，不要先背命令，先判断问题属于目录、工作区、历史还是远程。

`git status`、`git remote -v`、`git branch -vv` 和图形化 `git log` 是最实用的排查起点。
