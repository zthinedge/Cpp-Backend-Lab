# Git Rollback, Reset and Restore

## 1. 这篇文章解决什么问题？

日常写代码时，Git 最常见的压力不是“怎么提交”，而是：

```text
1. 文件改错了，怎么撤回？
2. 已经 git add 了，怎么从暂存区拿出来？
3. commit 写错了，怎么修改？
4. 已经提交了好几次，怎么回到之前的版本？
5. reset、restore、revert 到底有什么区别？
```

这篇只讲本地回退和撤销。

远程分支、`push` 被拒绝、`pull --rebase` 这类问题，统一放在 `02` 和 `05` 里理解。

---

## 2. 回退前先看清楚当前状态

任何回退操作之前，先执行：

```bash
git status
git log --oneline --graph --decorate --all
```

前者看工作区和暂存区：

```text
哪些文件被改了
哪些文件已经 add
哪些文件还没有被 Git 跟踪
```

后者看提交历史：

```text
当前 HEAD 在哪里
分支指向哪个 commit
想回到哪个 commit
```

不要在没看清状态时直接 `reset --hard`。

---

## 3. 三类“撤销”要分清楚

Git 里的撤销大致分三层：

```text
工作区撤销：文件还没 add，只想丢掉当前修改
暂存区撤销：文件已经 add，但不想让它进入下一次 commit
提交历史撤销：commit 已经生成，需要改历史或新增一个反向提交
```

对应常见命令：

```text
git restore
git restore --staged
git reset
git commit --amend
git revert
```

这些命令解决的问题不一样，不能只靠“撤销”两个字混着用。

---

## 4. 撤销工作区修改

如果文件还没有 `git add`，只是工作区改错了，可以执行：

```bash
git restore README.md
```

这表示：

```text
把 README.md 恢复到当前 HEAD 中记录的版本
丢弃工作区里这个文件的未暂存修改
```

如果要撤销当前目录下所有已跟踪文件的工作区修改：

```bash
git restore .
```

注意：

```text
git restore 不会恢复没有被 Git 跟踪的新文件
```

也就是说，`Untracked files` 里的文件不会因为 `git restore .` 自动消失。

---

## 5. 取消暂存区内容

如果已经执行过：

```bash
git add README.md
```

但后来发现暂时不想提交它，可以执行：

```bash
git restore --staged README.md
```

它的意思是：

```text
把 README.md 从暂存区拿出来
但保留工作区里的修改
```

也就是说，文件内容不会丢，只是下一次 commit 不再包含它。

常见流程：

```bash
git status
git restore --staged README.md
git status
```

这样能清楚看到文件从：

```text
Changes to be committed
```

回到：

```text
Changes not staged for commit
```

---

## 6. 同时撤销暂存区和工作区

如果一个文件已经 `git add`，并且你确定这些修改都不要了，可以分两步：

```bash
git restore --staged README.md
git restore README.md
```

第一步：

```text
从暂存区拿出来
```

第二步：

```text
丢掉工作区修改
```

更推荐新手这样写，因为每一步都能通过 `git status` 看清楚变化。

---

## 7. 修改最近一次 commit

如果刚刚提交完，发现 commit message 写错了：

```bash
git commit --amend -m "docs(git): update rollback note"
```

如果发现漏了一个文件：

```bash
git add README.md
git commit --amend
```

这会把暂存区里的内容合并进最近一次 commit。

需要注意：

```text
amend 会生成一个新的 commit
它不是在原地修改旧 commit
```

如果这次 commit 还没有 push，通常没什么问题。

如果已经 push 到远程，并且别人可能已经基于它继续开发，就不要随便 amend。

---

## 8. git reset 是什么？

`reset` 的核心动作是：

> 移动当前分支指针。

假设当前历史是：

```text
A -- B -- C -- D
               |
             main
             HEAD
```

执行：

```bash
git reset --soft C
```

或者：

```bash
git reset --mixed C
git reset --hard C
```

本质上都是让 `main` 从 `D` 移回 `C`：

```text
A -- B -- C
          |
        main
        HEAD
```

区别在于：

```text
commit 指针移动后，工作区和暂存区怎么处理
```

---

## 9. reset --soft

执行：

```bash
git reset --soft HEAD~1
```

表示：

```text
撤销最近一次 commit
但把这次 commit 的内容保留在暂存区
```

适合场景：

```text
commit message 写错了
几个 commit 想重新整理成一个
刚提交完发现还想补一点内容
```

执行后通常会看到：

```text
Changes to be committed
```

说明修改还在暂存区，下一次可以重新 commit。

---

## 10. reset --mixed

`reset` 默认就是 `--mixed`：

```bash
git reset HEAD~1
```

等价于：

```bash
git reset --mixed HEAD~1
```

它表示：

```text
撤销最近一次 commit
把这次 commit 的内容放回工作区
暂存区清空
```

适合场景：

```text
刚才提交太急了
想重新选择哪些文件进入下一次提交
```

执行后通常会看到：

```text
Changes not staged for commit
```

---

## 11. reset --hard

执行：

```bash
git reset --hard HEAD~1
```

表示：

```text
撤销最近一次 commit
丢掉对应的工作区和暂存区内容
让文件状态直接回到目标 commit
```

这是最危险的一种。

适合场景很少：

```text
你非常确定最近的提交和修改都不要了
当前修改没有任何保留价值
你已经确认没有未保存的重要内容
```

执行前至少先看一眼：

```bash
git status
git log --oneline
```

如果只是想取消暂存或撤销某个文件，通常不需要 `reset --hard`。

---

## 12. HEAD~1 是什么意思？

常见写法：

```bash
git reset --soft HEAD~1
```

这里的 `HEAD~1` 表示：

```text
当前提交的上一个提交
```

类似地：

```text
HEAD~2  当前提交往前数两个提交
HEAD~3  当前提交往前数三个提交
```

也可以直接写 commit hash：

```bash
git reset --mixed a8c912e
```

意思是回到 `a8c912e` 这个提交。

---

## 13. git revert 是什么？

`revert` 和 `reset` 很不一样。

`reset` 是移动分支指针，可能改写历史。

`revert` 是新增一个提交，用来抵消之前某个提交的修改。

例如历史是：

```text
A -- B -- C -- D
               |
             main
```

执行：

```bash
git revert C
```

可能得到：

```text
A -- B -- C -- D -- E
                    |
                  main
```

其中 `E` 是一个新的提交，它的内容用来撤销 `C` 带来的修改。

---

## 14. reset 和 revert 怎么选？

简单判断：

```text
本地提交，还没 push，想整理历史：可以 reset
已经 push 到远程，别人可能看到了：优先 revert
多人共享分支上要撤销某次提交：优先 revert
```

原因是：

```text
reset 会改变分支历史
revert 不改旧历史，只追加新历史
```

在团队协作里，`revert` 往往更稳。

---

## 15. 删除未跟踪文件

`git restore` 不会删除未跟踪文件。

如果想查看哪些未跟踪文件会被删除，可以先执行：

```bash
git clean -n
```

真正删除：

```bash
git clean -f
```

删除未跟踪目录：

```bash
git clean -fd
```

注意：

```text
clean 删除的是 Git 还没有跟踪的文件
删除后通常不能靠 Git 找回来
```

所以新手更推荐先用 `git clean -n` 预览。

---

## 16. 一个比较稳的回退流程

当你不知道该用哪个命令时，按这个顺序判断：

```text
1. 先 git status 看文件状态
2. 如果只是没 add 的文件改错了，用 git restore <file>
3. 如果已经 add 但不想提交，用 git restore --staged <file>
4. 如果最近一次 commit 写错了，并且还没 push，用 git commit --amend
5. 如果本地几个 commit 想重来，用 git reset --soft 或 --mixed
6. 如果已经 push 到共享分支，优先 git revert
```

不要一上来就：

```bash
git reset --hard
```

这个命令不是“万能撤销键”，它更像是“我确认这些修改都不要了”。

---

## 17. 一句话总结

`restore` 主要处理工作区和暂存区，`reset` 主要移动分支指针并可能改写历史，`revert` 通过新增提交来撤销旧提交。

回退前先 `git status`，再决定要动的是文件、暂存区，还是提交历史。
