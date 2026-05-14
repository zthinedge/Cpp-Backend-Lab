# Git Cheatsheet

## 1. 状态检查

```bash
git status
git diff
git diff --cached
git log --oneline --graph --decorate --all
git branch -vv
git remote -v
```

常用顺序：

```text
先看状态
再看 diff
最后决定 add、commit、pull、push 或回退
```

---

## 2. 本地提交流程

```bash
git status
git diff
git add README.md
git diff --cached
git commit -m "docs(git): update note"
```

提交后推送：

```bash
git push
```

第一次推送新分支：

```bash
git push -u origin feature/xxx
```

---

## 3. 分支操作

```bash
git branch
git branch -a
git checkout -b feature/xxx
git switch -c feature/xxx
git checkout main
git switch main
git branch -d feature/xxx
git push origin --delete feature/xxx
```

查看分支和 upstream：

```bash
git branch -vv
```

---

## 4. 远程仓库

```bash
git remote -v
git remote add origin git@github.com:username/repo.git
git remote set-url origin git@github.com:username/repo.git
git fetch origin
git pull
git push
```

SSH 测试：

```bash
ssh -T git@github.com
```

---

## 5. merge、rebase、pull

合并功能分支：

```bash
git checkout main
git pull
git merge feature/xxx
git push
```

让功能分支跟上 main：

```bash
git checkout feature/xxx
git fetch origin
git rebase origin/main
```

`pull` 本质：

```text
git pull = git fetch + git merge
git pull --rebase = git fetch + git rebase
```

---

## 6. 撤销和回退

撤销工作区修改：

```bash
git restore README.md
git restore .
```

取消暂存：

```bash
git restore --staged README.md
```

修改最近一次 commit：

```bash
git commit --amend -m "docs(git): update message"
```

回退本地提交：

```bash
git reset --soft HEAD~1
git reset --mixed HEAD~1
git reset --hard HEAD~1
```

撤销已经公开的提交：

```bash
git revert <commit-hash>
```

清理未跟踪文件前先预览：

```bash
git clean -n
git clean -f
```

---

## 7. 冲突处理

merge 冲突：

```bash
git status
# 手动修改冲突文件
git add <file>
git commit
```

rebase 冲突：

```bash
git status
# 手动修改冲突文件
git add <file>
git rebase --continue
```

取消操作：

```bash
git merge --abort
git rebase --abort
```

---

## 8. .gitignore

常见规则：

```gitignore
build/
cmake-build-*/
*.o
*.log
.env
.vscode/
.idea/
```

已经被 Git 跟踪的文件，单独写入 `.gitignore` 不会自动失效。

需要：

```bash
git rm --cached path/to/file
```

排查忽略规则：

```bash
git check-ignore -v path/to/file
```

---

## 9. commit message

推荐格式：

```text
<type>(<scope>): <summary>
```

常见例子：

```text
feat(redis): add RESP array parser
fix(http): handle keep-alive timeout
docs(git): add rollback note
refactor(net): simplify event loop cleanup
test(parser): cover invalid bulk string
chore(cmake): enable compile warnings
```

---

## 10. 常见错误入口

```text
not a git repository
当前目录不是 Git 仓库，先 pwd 和 ls -la

remote origin already exists
origin 已存在，用 git remote -v 查看，用 set-url 修改

Permission denied (publickey)
SSH 没配好或权限不对，先 ssh -T git@github.com

failed to push some refs / non-fast-forward
远程有本地没有的提交，先 fetch，再 merge 或 rebase

Your local changes would be overwritten
工作区有未提交修改，先 commit、stash 或 restore

detached HEAD
当前站在 commit 上，不在分支上，需要保留就 switch -c 新分支
```

---

## 11. 新手最稳的排查命令

```bash
pwd
git status
git branch -vv
git remote -v
git log --oneline --graph --decorate --all
```

先看清楚：

```text
在哪个目录
在哪个分支
工作区是否干净
远程地址是否正确
本地和远程历史是否一致
```
