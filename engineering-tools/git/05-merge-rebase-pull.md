# Git Merge, Rebase and Pull

## 1. 这篇文章解决什么问题？

在实际开发中，分支一旦多起来，就一定会遇到下面这些问题：

```text
1. 功能分支开发完，怎么并回 main？
2. main 已经前进了，功能分支怎么跟上最新代码？
3. git pull 到底做了什么？
4. merge 和 rebase 看起来都能“合并代码”，到底该怎么选？
```

这些问题如果只背命令，很容易越用越乱。

更稳的方式是先把三件事分清楚：

```text
merge：把两条历史接到一起
rebase：把一条历史“搬到”另一条历史后面
pull：先 fetch，再把远程变化整合到当前分支
```

---

## 2. 为什么会有“整合历史”这个问题？

假设当前项目历史是：

```text
A -- B -- C
          |
        main
```

这时你从 `main` 拉出一个功能分支开始开发：

```text
A -- B -- C
          |
        main
          |
 feature/auth
```

你在 `feature/auth` 上提交了两次：

```text
A -- B -- C -- D -- E
          |         |
        main   feature/auth
```

与此同时，`main` 上别人又提交了一个修复：

```text
A -- B -- C -- D -- E
          |         |
          F   feature/auth
          |
        main
```

这时问题就出现了：

```text
feature/auth 和 main 都向前走了
它们不再是同一条线
```

无论是 `merge` 还是 `rebase`，本质上都在解决这个问题。

---

## 3. merge 是什么？

`merge` 的思路很直接：

> 不改写原来的历史，而是把两条历史接到一起。

例如当前在 `main` 上执行：

```bash
git merge feature/auth
```

Git 可能会生成这样的结果：

```text
A -- B -- C -- D -- E
          |         \
          F --------- M
                    |
                  main
```

这里的 `M` 就是新的 merge commit。

它表示：

```text
main 和 feature/auth 两边的修改都保留
Git 新建了一个提交，把两边历史汇总到一起
```

这就是 `merge` 最重要的特点：

```text
历史真实
信息完整
不会改写已有提交
```

---

## 4. fast-forward merge 是什么？

并不是每次 `merge` 都会产生新的 merge commit。

如果 `main` 自从拉出功能分支后一直没有新提交，历史可能是这样：

```text
A -- B -- C -- D -- E
               |
             feature/auth
```

而 `main` 还停在 `C`：

```text
A -- B -- C
          |
        main
```

这时执行：

```bash
git merge feature/auth
```

Git 只需要把 `main` 指针直接移动到 `E`：

```text
A -- B -- C -- D -- E
                    |
                  main
```

这种情况叫 `fast-forward`。

它的特点是：

```text
没有新的 merge commit
只是分支指针往前移动
历史仍然是一条直线
```

---

## 5. rebase 是什么？

`rebase` 的思路和 `merge` 不一样。

它不是把两条历史接在一起，而是：

> 把当前分支上的提交，重新放到另一条历史后面。

继续用上面的例子。

当前历史是：

```text
A -- B -- C -- D -- E
          |         |
          F   feature/auth
          |
        main
```

如果切到 `feature/auth` 执行：

```bash
git rebase main
```

Git 会把 `feature/auth` 上的 `D`、`E` 拿下来，重新应用到 `main` 最新的 `F` 后面：

```text
A -- B -- C -- F -- D' -- E'
                    |
                  feature/auth
```

这里的 `D'`、`E'` 不是原来的 `D`、`E`，而是“重新生成”的新提交。

这就是 `rebase` 的核心：

```text
历史更直
阅读体验更顺
但会改写提交历史
```

---

## 6. merge 和 rebase 的区别到底在哪？

最容易混淆的点是：

```text
两者看起来都像“把代码合到一起”
```

但它们处理历史的方式完全不同。

### 6.1 merge 的特点

```text
保留真实分叉历史
不会改写已有提交
更适合共享分支和公共历史
日志里会看到 merge commit
```

### 6.2 rebase 的特点

```text
把历史整理成一条线
会重新生成提交
更适合整理自己的本地功能分支
日志更干净
```

### 6.3 一个简单判断方法

如果你更看重：

```text
安全
不改历史
团队协作时少踩坑
```

优先用 `merge`。

如果你更看重：

```text
提交历史线性
提交记录更好读
把自己的开发分支整理干净
```

可以在自己的功能分支上用 `rebase`。

---

## 7. 什么情况下更适合 merge？

下面这些场景更适合直接 `merge`：

```text
1. 准备把功能分支并回 main
2. 这是一个多人共用的分支
3. 你不想改写已经存在的提交
4. 你希望保留“这个功能分支曾经独立开发过”的历史痕迹
```

例如：

```bash
git checkout main
git pull
git merge feature/auth
git push
```

这种流程简单、稳妥，也更不容易在团队里制造麻烦。

---

## 8. 什么情况下更适合 rebase？

下面这些场景更适合 `rebase`：

```text
1. 你正在整理自己的本地功能分支
2. main 已经更新了，你想先把功能分支挪到最新 main 后面
3. 你希望提交历史更线性
4. 这些提交还没有被别人基于它继续开发
```

常见操作：

```bash
git checkout feature/auth
git fetch origin
git rebase origin/main
```

这样做完之后，再看日志会比较整齐：

```text
main 上的新提交在前
你的功能提交接在后面
整个分支像是基于最新 main 开发出来的
```

---

## 9. rebase 最容易踩的坑

`rebase` 最危险的地方不是命令本身，而是误用。

最常见的问题有两个。

### 9.1 不要随便 rebase 公共分支

如果一个分支已经被 push 出去，而且别人已经基于它继续开发，这时你再 `rebase`，等于改写了公共历史。

结果通常是：

```text
别人本地的提交基线变了
再次 pull 或 merge 时出现混乱
需要强推
协作成本陡增
```

### 9.2 rebase 后 push 可能被拒绝

因为 rebase 会生成新的提交，远程仓库看到的会是：

```text
本地分支历史已经不是原来的那条历史了
```

这时普通 `git push` 可能失败。

常见做法是：

```bash
git push --force-with-lease
```

这里更推荐 `--force-with-lease`，而不是裸 `--force`。

原因很简单：

```text
--force 只管覆盖
--force-with-lease 会先检查远程是不是还是你预期的状态
```

---

## 10. git pull 到底做了什么？

`git pull` 不是神秘黑盒，它本质上是两步：

```bash
git fetch
git merge
```

也就是说，执行：

```bash
git pull origin main
```

大致等价于：

```bash
git fetch origin main
git merge origin/main
```

它做的事情是：

```text
先把远程最新提交拿到本地
再把这些提交整合进当前分支
```

所以 `pull` 本身不是新的底层机制，它只是把两个常用动作打包在了一起。

---

## 11. git pull --rebase 又是什么？

默认情况下，`git pull` 拉下来之后会做 `merge`。

如果你执行：

```bash
git pull --rebase
```

它的思路会变成：

```text
先 fetch
再把你当前分支的本地提交，rebase 到远程最新提交后面
```

这种方式很适合下面的场景：

```text
你在自己的功能分支上已经做了几个本地提交
远程同一个分支也有了新的提交
你希望历史更线性，不想多出一个 merge commit
```

例如：

```bash
git pull --rebase origin feature/auth
```

这样做通常能避免“只是同步远程，却多出一个没必要的 merge commit”。

---

## 12. 日常开发里更实用的工作流

如果是个人项目或者小团队，下面这套习惯比较稳：

### 12.1 功能分支开发期间

```bash
git checkout feature/auth
git fetch origin
git rebase origin/main
```

作用是：

```text
让功能分支跟上 main
历史保持线性
提前暴露冲突
```

### 12.2 功能完成并回 main

```bash
git checkout main
git pull
git merge feature/auth
git push
```

作用是：

```text
回收功能成果
在稳定分支上完成正式合并
不轻易改写公共历史
```

这套组合很常见：

```text
自己的分支内部可以 rebase
真正进入主分支时用 merge
```

---

## 13. 冲突在 merge 和 rebase 里有什么区别？

无论是 `merge` 还是 `rebase`，只要两边改了同一块内容，就可能冲突。

差别在于处理时机不同。

### 13.1 merge 冲突

你在执行：

```bash
git merge feature/auth
```

时一次性处理冲突。

### 13.2 rebase 冲突

你在执行：

```bash
git rebase main
```

时，Git 会把你的提交一个一个重新应用。

如果中途某个提交冲突了，就会停下来，要求你先解决：

```bash
git add .
git rebase --continue
```

所以很多人会感觉：

```text
merge 冲突像“一次性结算”
rebase 冲突像“按提交分期结算”
```

---

## 14. 新手最容易犯的几个误区

### 14.1 以为 rebase 更高级，所以到处用

不是。

`rebase` 不是“高级版 merge”，它只是另一种整理历史的方法。

如果分支已经公开、多人共用，盲目 `rebase` 往往比 `merge` 更危险。

### 14.2 以为 merge 会让历史变脏，所以一定不要 merge

也不是。

很多团队本来就希望保留真实分支历史，`merge commit` 本身并不是坏事。

### 14.3 以为 git pull 出问题时，先 force 再说

这通常不是好习惯。

你首先应该确认：

```text
远程多了什么提交
本地又多了什么提交
当前分支到底想 merge 还是想 rebase
```

---

## 15. 一句话总结

`merge` 是把两条历史接到一起，`rebase` 是把一条历史搬到另一条历史后面，`pull` 则是获取远程更新并整合进当前分支的快捷操作。
