# Git Branch Management

## 1. 这篇文章解决什么问题？

在实际开发中，不能所有修改都直接在 `main` 分支上完成。

如果所有功能、bug 修复、文档修改都直接堆在 `main` 上，会导致：

```text
1. main 分支不稳定
2. 功能开发和 bug 修复混在一起
3. 出问题时不好回退
4. 多个任务并行开发时容易混乱
5. 提交历史不清晰
```

所以 Git 提供了分支机制。

这篇笔记主要整理：

```text
1. main 分支和功能分支的区别
2. 如何创建、切换、删除、重命名分支
3. 如何把本地分支推送到远程
4. 日常开发时推荐的分支习惯
```

这篇不再重复讲 `commit`、`HEAD`、工作区、远程仓库基础概念。

如果要回看分支本质，统一参考 `01-git-basic-concepts.md`。

---

## 2. branch 是什么？

分支 `branch` 本质上是一个指向 `commit` 的指针。

这里不再像 `01` 那样详细展开，只记住最关键的一句话：

> 分支的作用，是让你在不污染 `main` 的情况下，单独推进一个任务。

例如当前提交历史是：

```text
A -- B -- C
          |
        main
```

表示 `main` 分支当前指向提交 `C`。

如果从 `main` 创建一个新分支：

```bash
git checkout -b feature/resp-parser
```

此时提交历史可以理解为：

```text
A -- B -- C
          |
        main
          |
 feature/resp-parser
```

这表示：

```text
main 和 feature/resp-parser 当前都指向 C
```

如果在 `feature/resp-parser` 分支上继续提交：

```text
A -- B -- C -- D
          |    |
        main  feature/resp-parser
```

这时 `main` 还停在旧位置，新的开发只发生在 `feature/resp-parser` 上。

---

## 3. 查看分支

查看本地分支：

```bash
git branch
```

输出示例：

```text
* main
  feature/resp-parser
```

其中 `*` 表示当前所在分支。

查看本地和远程所有分支：

```bash
git branch -a
```

输出示例：

```text
* main
  feature/resp-parser
  remotes/origin/main
```

查看更详细的分支信息：

```bash
git branch -vv
```

可以看到本地分支对应的远程分支，以及领先或落后情况。

---

## 4. 创建分支

只创建分支，不切换：

```bash
git branch feature/resp-parser
```

创建并切换到新分支：

```bash
git checkout -b feature/resp-parser
```

新版 Git 也可以使用：

```bash
git switch -c feature/resp-parser
```

个人更常用：

```bash
git checkout -b feature/xxx
```

或者：

```bash
git switch -c feature/xxx
```

---

## 5. 切换分支

切换到 `main` 分支：

```bash
git checkout main
```

或者：

```bash
git switch main
```

切换到功能分支：

```bash
git checkout feature/resp-parser
```

或者：

```bash
git switch feature/resp-parser
```

切换分支前，最好先查看当前状态：

```bash
git status
```

如果当前有未提交的修改，切换分支可能会失败，或者把修改带到另一个分支。

---

## 6. 删除分支

删除本地分支：

```bash
git branch -d feature/resp-parser
```

如果分支还没有合并，Git 可能会拒绝删除。

强制删除本地分支：

```bash
git branch -D feature/resp-parser
```

注意：

```text
-D 是强制删除，可能会丢掉该分支上的提交，慎用。
```

删除远程分支：

```bash
git push origin --delete feature/resp-parser
```

---

## 7. 重命名分支

如果当前就在要重命名的分支上：

```bash
git branch -m new-name
```

例如把当前分支改成 `main`：

```bash
git branch -m main
```

如果要把指定分支改名：

```bash
git branch -m old-name new-name
```

例如：

```bash
git branch -m master main
```

---

## 8. 推送本地分支到远程

如果本地新建了一个分支，想推送到 GitHub：

```bash
git push -u origin feature/resp-parser
```

设置后，以后在这个分支上可以直接：

```bash
git push
git pull
```

不用每次都写：

```bash
git push origin feature/resp-parser
```

`-u`、上游分支、`origin/main` 这些远程细节如果想系统理解，统一看 `02-github-ssh-remote.md`。

---

## 9. 拉取远程分支到本地

查看远程分支：

```bash
git branch -a
```

如果看到：

```text
remotes/origin/feature/resp-parser
```

可以在本地创建并切换到对应分支：

```bash
git checkout -b feature/resp-parser origin/feature/resp-parser
```

新版 Git 可以使用：

```bash
git switch -c feature/resp-parser origin/feature/resp-parser
```

---

## 10. main 分支应该放什么？

`main` 分支一般应该保持稳定。

适合放：

```text
1. 已经能正常运行的代码
2. 已经测试通过的功能
3. 稳定的文档版本
4. 可以随时展示给别人的项目状态
```

不建议在 `main` 上直接做大量实验性修改。

比如 TinyRedis 中，不建议直接在 `main` 上乱改：

```text
RESP parser
dict rehash
AOF persistence
TTL expire
network module
```

这些都应该单独拉分支开发。

---

## 11. 常见分支命名方式

个人项目可以使用下面几类分支：

```text
feature/xxx   新功能
fix/xxx       修复 bug
docs/xxx      文档修改
test/xxx      测试相关
refactor/xxx  代码重构
perf/xxx      性能优化
build/xxx     构建系统
```

例如：

```bash
git checkout -b feature/resp-parser
git checkout -b feature/dict-rehash
git checkout -b feature/aof
git checkout -b fix/ttl-expire
git checkout -b docs/git-notes
git checkout -b test/resp-parser
git checkout -b perf/resp-parser
```

---

## 12. 个人项目推荐工作流

以 TinyRedis 为例，开发一个 RESP parser，可以这样做：

```bash
# 1. 切回 main
git checkout main

# 2. 拉取远程最新代码
git pull

# 3. 创建功能分支
git checkout -b feature/resp-parser

# 4. 开发代码
# 修改 src/resp/ 下的文件

# 5. 查看修改
git status
git diff

# 6. 提交
git add src/resp/
git commit -m "feat(resp): implement incremental parser"

# 7. 推送功能分支
git push -u origin feature/resp-parser
```

如果开发完成，并且测试通过，可以合并回 `main`：

```bash
git checkout main
git pull
git merge feature/resp-parser
git push
```

合并完成后，可以删除本地功能分支：

```bash
git branch -d feature/resp-parser
```

如果远程也推送过这个分支，可以删除远程分支：

```bash
git push origin --delete feature/resp-parser
```

这里先把 `merge` 当成“把功能分支结果收回 main”的动作即可。

`merge`、`rebase`、`pull` 的细节对比，后面更适合单独写在 `05-merge-rebase-pull.md`。

---

## 13. 文档修改也可以单独开分支

例如整理 Git 笔记：

```bash
git checkout main
git pull

git checkout -b docs/git-notes
```

修改文档后：

```bash
git add 09-engineering-tools/git/
git commit -m "docs(git): add branch management note"
git push -u origin docs/git-notes
```

合并回 main：

```bash
git checkout main
git merge docs/git-notes
git push
```

---

## 14. bug 修复分支示例

假设 TinyRedis 的 dict rehash 查询有 bug，可以这样：

```bash
git checkout main
git pull

git checkout -b fix/dict-rehash-lookup
```

修改代码并测试后：

```bash
git add src/dict/
git commit -m "fix(dict): handle lookup during rehash"
git push -u origin fix/dict-rehash-lookup
```

合并回 main：

```bash
git checkout main
git merge fix/dict-rehash-lookup
git push
```

---

## 15. 分支合并先记最常见流程

把功能分支合并回 main：

```bash
git checkout main
git merge feature/resp-parser
```

如果没有冲突，Git 会自动完成合并。

合并后推送：

```bash
git push
```

如果出现冲突，需要手动解决冲突，然后：

```bash
git add .
git commit
git push
```

冲突处理可以单独放到：

```text
07-conflict-resolution.md
```

---

## 16. 分支开发时的注意事项

### 16.1 不要长期不合并 main

如果功能分支开发时间太长，而 main 一直在变，最后合并时容易产生冲突。

建议开发前先：

```bash
git checkout main
git pull
```

再创建分支。

开发过程中也可以定期同步 main。

---

### 16.2 不要在 main 上乱做实验

如果只是临时尝试一个功能，建议开分支：

```bash
git checkout -b experiment/new-parser
```

实验失败了，可以直接删除：

```bash
git branch -D experiment/new-parser
```

这样不会污染 main。

---

### 16.3 提交前确认当前分支

提交前先执行：

```bash
git branch
```

或者：

```bash
git status
```

确认自己没有在错误的分支上提交。

很多时候误提交到 `main`，都是因为没有先检查当前分支。

---

## 17. 如果不小心在 main 上提交了怎么办？

假设本来应该在 `feature/resp-parser` 上提交，但不小心在 `main` 上提交了。

如果这个 commit 还没有 push，可以先创建新分支保留当前提交：

```bash
git checkout -b feature/resp-parser
```

这时新分支会包含刚才的提交。

后续再决定怎么把 `main` 调整回去。

更详细的回退操作，单独放到：

```text
06-rollback-reset-restore.md
```

---

## 18. 我的推荐分支策略

对于个人 C++ 后端项目，推荐简单一点：

```text
main：稳定分支
feature/xxx：新功能开发
fix/xxx：bug 修复
docs/xxx：文档修改
test/xxx：测试补充
refactor/xxx：代码重构
perf/xxx：性能优化
```

不需要一开始搞太复杂的 Git Flow。

个人项目中，够用的流程是：

```text
main
  ↓
feature/xxx
  ↓
开发、测试、提交
  ↓
merge 回 main
```

---

## 19. TinyRedis 分支规划示例

TinyRedis 可以这样拆分分支：

```bash
# RESP 协议解析
git checkout -b feature/resp-parser

# SDS 字符串
git checkout -b feature/sds

# dict 渐进式 rehash
git checkout -b feature/dict-rehash

# TTL 过期机制
git checkout -b feature/ttl-expire

# AOF 持久化
git checkout -b feature/aof

# INFO / metrics
git checkout -b feature/info-metrics

# 压测
git checkout -b perf/benchmark

# 文档
git checkout -b docs/tinyredis-design
```

对应 commit message 可以写：

```bash
git commit -m "feat(resp): implement incremental parser"
git commit -m "feat(dict): support incremental rehash"
git commit -m "feat(ttl): add expire command"
git commit -m "feat(aof): implement append only file"
git commit -m "docs(tinyredis): add architecture overview"
git commit -m "perf(redis): add benchmark report"
```

---

## 20. 常用命令总结

```bash
# 查看本地分支
git branch

# 查看所有分支
git branch -a

# 创建分支
git branch feature/xxx

# 创建并切换分支
git checkout -b feature/xxx

# 切换分支
git checkout main

# 新版切换分支
git switch main

# 新版创建并切换分支
git switch -c feature/xxx

# 删除本地分支
git branch -d feature/xxx

# 强制删除本地分支
git branch -D feature/xxx

# 重命名当前分支
git branch -m new-name

# 重命名指定分支
git branch -m old-name new-name

# 推送当前分支到远程
git push -u origin feature/xxx

# 删除远程分支
git push origin --delete feature/xxx

# 合并分支
git checkout main
git merge feature/xxx
```

---

## 21. 总结

分支的本质是给不同任务拆出独立的开发轨道。

日常开发中，不要把所有修改都直接堆到 `main` 分支。

更推荐的习惯是：

```text
main 保持稳定
新功能开 feature/xxx
bug 修复开 fix/xxx
文档修改开 docs/xxx
测试补充开 test/xxx
开发完成后再合并回 main
```

这样项目历史更清晰，出问题时也更容易定位和回退。
