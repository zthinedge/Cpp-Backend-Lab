# Git Conflict Resolution

## 1. 这篇文章解决什么问题？

Git 冲突不是异常情况，而是多人协作和分支开发里的正常现象。

常见问题包括：

```text
1. 为什么会产生冲突？
2. 冲突文件里的 <<<<<<<、=======、>>>>>>> 是什么？
3. merge 冲突怎么解决？
4. rebase 冲突怎么解决？
5. 解决冲突后为什么还要 git add？
6. 如果不想继续 merge 或 rebase，怎么取消？
```

这篇只讲冲突处理流程。

`merge`、`rebase`、`pull` 的概念放在 `05-merge-rebase-pull.md` 里理解。

---

## 2. 冲突为什么会发生？

Git 自动合并的前提是：

```text
两边修改的位置不冲突
或者 Git 能判断应该保留哪些内容
```

如果两个分支都修改了同一个文件的同一块内容，Git 就不敢替你决定。

例如 `main` 上把一行改成：

```cpp
int timeout = 30;
```

功能分支上把同一行改成：

```cpp
int timeout = 60;
```

这时 Git 不知道最终应该是 `30` 还是 `60`，就会产生冲突。

---

## 3. 冲突发生时 Git 在做什么？

假设当前历史是：

```text
A -- B -- C
          |
        main
          \
           D -- E
                |
          feature/cache
```

在 `main` 上执行：

```bash
git merge feature/cache
```

如果两边改了同一块内容，Git 会停下来：

```text
Auto-merging config.cpp
CONFLICT (content): Merge conflict in config.cpp
Automatic merge failed; fix conflicts and then commit the result.
```

这时仓库处于一个“合并进行中”的状态。

需要做三件事：

```text
1. 打开冲突文件
2. 手动决定最终内容
3. git add 标记冲突已解决
```

---

## 4. 冲突标记怎么看？

冲突文件里通常会出现：

```text
marker: <<<<<<< HEAD
int timeout = 30;
marker: =======
int timeout = 60;
marker: >>>>>>> feature/cache
```

真实文件里不会有 `marker:` 前缀，这里只是为了避免 Git 把笔记示例误判成未解决冲突。

含义是：

```text
上半段：<<<<<<< HEAD 到 ======= 之间，是当前分支的内容
下半段：======= 到 >>>>>>> feature/cache 之间，是被合并进来的分支内容
```

如果你当前在 `main` 上执行：

```bash
git merge feature/cache
```

那么：

```text
HEAD 通常表示 main 当前内容
feature/cache 表示正在合并进来的内容
```

---

## 5. 冲突文件应该怎么改？

解决冲突不是删除某一边那么简单，而是决定最终代码应该长什么样。

例如上面的冲突，可以改成：

```cpp
int timeout = 60;
```

也可以改成：

```cpp
int timeout = loadTimeoutFromConfig();
```

关键是：

```text
最终文件里不能留下 <<<<<<<、=======、>>>>>>>
最终代码必须能表达正确业务逻辑
```

解决后执行：

```bash
git status
git add config.cpp
```

`git add` 在这里的含义不是“准备提交新功能”，而是：

```text
告诉 Git：这个文件的冲突我已经处理完了
```

---

## 6. merge 冲突的完整流程

典型流程：

```bash
git checkout main
git merge feature/cache
```

如果出现冲突：

```bash
git status
```

打开冲突文件，修改成最终内容。

然后：

```bash
git add config.cpp
git status
git commit
```

如果这是普通 `merge`，最后的 `git commit` 会生成一个 merge commit。

有时 Git 会自动准备好 commit message，直接保存即可。

---

## 7. rebase 冲突的完整流程

典型流程：

```bash
git checkout feature/cache
git rebase main
```

如果某个提交应用时发生冲突，Git 会停下来。

解决文件后执行：

```bash
git add config.cpp
git rebase --continue
```

如果后面的提交继续冲突，就继续重复：

```text
改冲突文件
git add
git rebase --continue
```

这就是 `05` 里说的：

```text
merge 冲突像一次性结算
rebase 冲突像按提交分期结算
```

---

## 8. ours 和 theirs 怎么理解？

冲突时经常会看到：

```bash
git checkout --ours config.cpp
git checkout --theirs config.cpp
```

在 `merge` 中通常可以这样理解：

```text
ours   当前分支的版本
theirs 被合并进来的分支版本
```

例如当前在 `main` 上：

```bash
git merge feature/cache
```

那么：

```text
ours   main
theirs feature/cache
```

如果你确定保留当前分支版本：

```bash
git checkout --ours config.cpp
git add config.cpp
```

如果你确定保留对方分支版本：

```bash
git checkout --theirs config.cpp
git add config.cpp
```

注意：

```text
不要只因为命令方便就整文件保留一边
很多冲突需要把两边逻辑合在一起
```

---

## 9. rebase 里的 ours 和 theirs 容易反直觉

在 `rebase` 过程中，`ours` 和 `theirs` 容易让人迷糊。

因为 rebase 的过程是：

```text
先切到目标基底
再把你的提交一个一个应用上去
```

所以在 rebase 冲突里，`ours` 往往表示当前基底那边，`theirs` 往往表示正在被重新应用的那个提交。

新手不建议在 rebase 冲突里盲目使用：

```bash
git checkout --ours
git checkout --theirs
```

更稳的方式是：

```text
打开文件
读冲突上下文
手动改成最终想要的代码
```

---

## 10. 不想继续 merge 怎么办？

如果 merge 到一半发现不想继续了：

```bash
git merge --abort
```

它会尝试回到 merge 之前的状态。

常见场景：

```text
冲突太多，想换一种合并方式
发现合错分支了
还没准备好处理这些冲突
```

执行后再看：

```bash
git status
```

确认仓库已经回到正常状态。

---

## 11. 不想继续 rebase 怎么办？

如果 rebase 到一半不想继续：

```bash
git rebase --abort
```

它会回到 rebase 开始之前。

如果已经解决了当前冲突，想继续：

```bash
git rebase --continue
```

如果想跳过当前这个提交：

```bash
git rebase --skip
```

`--skip` 要谨慎，因为它表示：

```text
当前这个提交不要了
```

---

## 12. 冲突解决后要做什么检查？

解决冲突后，不要只看 Git 是否通过，还要确认代码真的正确。

推荐顺序：

```bash
git status
git diff --cached
```

然后根据项目情况运行：

```bash
cmake --build build
ctest --test-dir build
```

或者至少运行当前模块的编译和测试。

冲突解决最容易出问题的地方不是语法，而是：

```text
两边逻辑都保留了，但组合后语义不对
删掉冲突标记了，但误删了一段必要逻辑
代码能编译，但配置、路径、条件判断错了
```

---

## 13. 减少冲突的习惯

冲突不能完全避免，但可以减少。

比较实用的习惯：

```text
1. 功能分支不要拖太久
2. 经常 fetch，然后让自己的分支跟上 main
3. 不要在一个提交里混合太多无关修改
4. 格式化代码和业务修改尽量分开提交
5. 修改公共头文件、配置文件时更谨慎
6. 合并前先看 git status，保证工作区干净
```

尤其是 C++ 项目里，下面这些文件容易引发多人冲突：

```text
CMakeLists.txt
公共头文件
配置文件
README.md
接口定义文件
```

这些文件改动前最好先同步最新主分支。

---

## 14. 总结

冲突的本质是 Git 无法自动判断最终内容。

解决冲突时，先用 `git status` 找到冲突文件，再手动改成正确代码，最后用 `git add` 标记已解决；如果方向错了，用 `merge --abort` 或 `rebase --abort` 退回去。
