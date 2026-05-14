# Git Notes

本目录用于整理 Git 在个人 C++ 后端项目开发中的常用知识点。

为了避免不同文章反复讲同一件事，每篇文章的分工如下：

- [01-git-basic-concepts.md](./01-git-basic-concepts.md)：只讲 Git 的核心心智模型，重点是 `commit`、`branch`、`HEAD`、工作区、暂存区、本地仓库。
- [02-github-ssh-remote.md](./02-github-ssh-remote.md)：只讲 GitHub、SSH、公钥、`remote`、`origin`、`git init` 和 `git clone` 的远程场景。
- [03-local-workflow.md](./03-local-workflow.md)：只讲本地日常提交流程，重点是 `status`、`diff`、`add`、`commit`、`log`。
- [04-branch-management.md](./04-branch-management.md)：只讲分支管理和日常分支工作流，重点是创建、切换、删除、命名和合并前后的习惯。
- [05-merge-rebase-pull.md](./05-merge-rebase-pull.md)：只讲 `merge`、`rebase` 和 `pull` 的区别，以及功能分支如何跟上主分支。
- [06-rollback-reset-restore.md](./06-rollback-reset-restore.md)：只讲本地撤销和回退，重点是 `restore`、`reset`、`revert`、`amend`。
- [07-conflict-resolution.md](./07-conflict-resolution.md)：只讲冲突处理，重点是冲突标记、`merge --abort`、`rebase --continue` 和解决后的检查。
- [08-gitignore.md](./08-gitignore.md)：只讲 `.gitignore`，重点是 C++ 项目中哪些文件不应该进仓库，以及 `git rm --cached`。
- [09-commit-message.md](./09-commit-message.md)：只讲提交信息怎么写，重点是 `<type>(<scope>): <summary>` 和小而完整的提交习惯。
- [10-common-errors.md](./10-common-errors.md)：只讲常见报错排查，重点是先判断目录、工作区、历史还是远程问题。
- [cheatsheet.md](./cheatsheet.md)：常用命令速查表。

推荐阅读顺序：

```text
01 先建立 Git 心智模型
02 再理解 GitHub 和远程仓库
03 然后练习本地提交流程
04 进入分支管理和功能分支习惯
05 理解 merge、rebase、pull 的区别
06 学会撤销、回退和恢复
07 学会处理冲突
08 配好 .gitignore，避免提交无关文件
09 规范 commit message，让历史更好读
10 遇到报错时按类型排查
```

如果只是临时查命令，可以直接看 `cheatsheet.md`。
