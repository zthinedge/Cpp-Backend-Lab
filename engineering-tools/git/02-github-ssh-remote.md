# GitHub SSH and Remote

## 1. 这篇文章解决什么问题？

在使用 GitHub 管理项目时，本地 Git 仓库和远程 GitHub 仓库之间需要建立连接。

常见问题包括：

```text
1. SSH key 是什么？
2. 为什么一台电脑只需要配置一次 SSH key？
3. git remote add origin 是什么？
4. 本地空文件夹怎么绑定 GitHub 仓库？
5. GitHub 仓库已经有 README 时，为什么应该用 git clone？
6. push 被拒绝和 remote 配置错误该怎么处理？
```

这篇笔记主要记录 GitHub SSH 配置、远程仓库绑定和常见远程操作流程。

---

## 2. SSH key 是什么？

SSH key 可以理解为本地电脑连接 GitHub 的身份凭证。

使用 SSH 方式操作 GitHub 仓库时，远程地址通常长这样：

```text
git@github.com:username/repo.git
```

如果本地没有配置 SSH key，GitHub 就无法确认这台电脑是否有权限访问当前账号下的仓库。

配置 SSH key 后，本地就可以不用每次输入账号密码，直接执行：

```bash
git clone git@github.com:username/repo.git
git push
git pull
```

---

## 3. 公钥和私钥

生成 SSH key 后，一般会得到两个文件：

```text
~/.ssh/id_ed25519
~/.ssh/id_ed25519.pub
```

它们的区别是：

```text
id_ed25519      私钥，保存在本地，不能泄露
id_ed25519.pub  公钥，可以添加到 GitHub
```

可以简单理解为：

```text
公钥：交给 GitHub，用来识别你的电脑
私钥：留在本地，用来证明你就是这台电脑的使用者
```

注意：

> 私钥不能发给别人，也不要上传到 GitHub。

---

## 4. 一台电脑需要配置几次 SSH key？

通常情况下：

```text
一台电脑配置一次 SSH key 即可。
```

不需要每创建一个 GitHub 仓库就重新生成一次公钥。

只要这台电脑的公钥已经添加到 GitHub 账号中，那么以后访问这个账号下的多个仓库，都可以直接使用 SSH 地址。

例如下面这些仓库都可以使用同一个 SSH key：

```text
git@github.com:zthinedge/Cpp-Backend-Lab.git
git@github.com:zthinedge/TinyRedis.git
git@github.com:zthinedge/HTTPServer.git
```

只有在这些情况下，才可能需要重新配置 SSH key：

```text
1. 换了一台电脑
2. 重装了系统
3. 删除了 ~/.ssh/id_ed25519
4. 想为不同 GitHub 账号配置不同 key
```

---

## 5. 生成 SSH key

使用下面命令生成 SSH key：

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```

其中：

```text
-t ed25519
```

表示使用 ed25519 算法生成密钥。

```text
-C "your_email@example.com"
```

表示给这个 key 添加一个备注，一般写 GitHub 绑定邮箱即可。

执行后一路回车即可。

生成成功后，默认会在下面目录生成文件：

```text
~/.ssh/id_ed25519
~/.ssh/id_ed25519.pub
```

---

## 6. 查看公钥

执行：

```bash
cat ~/.ssh/id_ed25519.pub
```

终端会输出一长串内容，类似：

```text
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAI... your_email@example.com
```

把这一整行复制下来。

注意复制的是 `.pub` 文件内容，也就是公钥。

不要复制：

```text
~/.ssh/id_ed25519
```

这个是私钥。

---

## 7. 把公钥添加到 GitHub

进入 GitHub 后，按下面路径操作：

```text
右上角头像
→ Settings
→ SSH and GPG keys
→ New SSH key
```

然后填写：

```text
Title：随便写一个名字，例如 VMware Ubuntu / Windows Laptop
Key：粘贴刚才 cat ~/.ssh/id_ed25519.pub 输出的内容
```

保存即可。

---

## 8. 测试 SSH 是否配置成功

执行：

```bash
ssh -T git@github.com
```

第一次连接时，可能会提示：

```text
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```

输入：

```bash
yes
```

如果配置成功，会看到类似：

```text
Hi username! You've successfully authenticated, but GitHub does not provide shell access.
```

这句话的意思是：

```text
身份验证成功，但 GitHub 不提供 SSH shell 登录。
```

这是正常现象，不是报错。

---

## 9. remote 是什么？

`remote` 表示远程仓库地址。

比如本地仓库要关联 GitHub 上的仓库，可以执行：

```bash
git remote add origin git@github.com:username/repo.git
```

这里的：

```text
origin
```

是远程仓库的默认别名。

真正的远程地址是：

```text
git@github.com:username/repo.git
```

也就是说：

```text
origin = git@github.com:username/repo.git
```

以后执行：

```bash
git push origin main
```

就表示：

```text
把本地 main 分支推送到 origin 这个远程仓库
```

---

## 10. 查看远程仓库地址

执行：

```bash
git remote -v
```

可能看到：

```text
origin  git@github.com:zthinedge/Cpp-Backend-Lab.git (fetch)
origin  git@github.com:zthinedge/Cpp-Backend-Lab.git (push)
```

其中：

```text
fetch
```

表示拉取代码时使用的地址。

```text
push
```

表示推送代码时使用的地址。

通常情况下，这两个地址一样。

---

## 11. 添加远程仓库

如果本地仓库还没有绑定远程地址，可以执行：

```bash
git remote add origin git@github.com:username/repo.git
```

例如：

```bash
git remote add origin git@github.com:zthinedge/Cpp-Backend-Lab.git
```

添加完成后可以检查：

```bash
git remote -v
```

---

## 12. 修改远程仓库地址

如果 GitHub 仓库改名，或者本地绑定错了远程地址，可以使用：

```bash
git remote set-url origin git@github.com:username/new-repo.git
```

例如：

```bash
git remote set-url origin git@github.com:zthinedge/Cpp-Backend-Lab.git
```

修改后检查：

```bash
git remote -v
```

---

## 13. 删除远程仓库绑定

如果想删除当前绑定的 origin，可以执行：

```bash
git remote remove origin
```

然后重新添加：

```bash
git remote add origin git@github.com:username/repo.git
```

常见场景：

```text
1. remote origin already exists
2. 远程仓库地址写错了
3. 想重新绑定另一个 GitHub 仓库
```

---

## 14. 本地空文件夹绑定 GitHub 空仓库

适用场景：

```text
本地已经有一个空文件夹
GitHub 上刚创建了一个空仓库
GitHub 创建仓库时没有勾选 README / LICENSE / .gitignore
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

例如：

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

---

## 15. GitHub 仓库已经有 README 时的流程

适用场景：

```text
GitHub 创建仓库时勾选了 README
或者勾选了 LICENSE
或者勾选了 .gitignore
```

这说明远程仓库已经不是空仓库了，它已经有初始 commit。

这时推荐直接 clone：

```bash
git clone git@github.com:username/repo.git
cd repo
```

例如：

```bash
git clone git@github.com:zthinedge/Cpp-Backend-Lab.git
cd Cpp-Backend-Lab
```

然后再修改文件并提交：

```bash
echo "# Cpp-Backend-Lab" > README.md
git add README.md
git commit -m "docs: update README"
git push
```

---

## 16. 为什么远程已有 README 时推荐 clone？

因为 GitHub 创建 README 时，会自动生成一个 commit。

远程仓库历史类似：

```text
A  add README
|
main
```

如果本地又自己 `git init` 并提交一次：

```text
B  init
|
main
```

这时远程和本地是两条不同的提交历史：

```text
远程：A

本地：B
```

直接 push 会被拒绝，因为 Git 不允许你直接覆盖远程已有提交。

而 `git clone` 会先把远程已有的 `A` 下载到本地。

然后你再提交新的修改：

```text
A -- B
```

这时 push 就是正常追加提交。

---

## 17. push -u 是什么？

第一次推送本地分支到远程时，常用：

```bash
git push -u origin main
```

其中：

```text
-u
```

表示设置上游分支。

也就是把：

```text
本地 main
```

和：

```text
远程 origin/main
```

建立默认关联。

设置成功后，以后就可以直接使用：

```bash
git push
git pull
```

不用每次都写：

```bash
git push origin main
git pull origin main
```

---

## 18. origin/main 是什么？

`origin/main` 是远程 main 分支在本地的记录。

可以理解为：

```text
origin/main 表示：上一次从 origin 拉取时，远程 main 分支的位置。
```

例如：

```text
A -- B
     |
 origin/main
```

表示本地记录中，远程 main 分支当前在 B。

当执行：

```bash
git fetch origin
```

Git 会更新 `origin/main`，但不会自动修改当前本地分支代码。

---

## 19. fetch、pull、push 的区别

### 19.1 git fetch

```bash
git fetch origin
```

作用：

```text
只把远程最新提交下载到本地，但不自动合并到当前分支。
```

---

### 19.2 git pull

```bash
git pull
```

作用：

```text
拉取远程提交，并合并到当前分支。
```

大致等价于：

```bash
git fetch
git merge
```

---

### 19.3 git push

```bash
git push
```

作用：

```text
把本地提交推送到远程仓库。
```

---

## 20. GitHub 仓库改名后，本地怎么办？

如果 GitHub 仓库改名，本地仓库不会自动更新远程地址。

先查看当前地址：

```bash
git remote -v
```

然后修改：

```bash
git remote set-url origin git@github.com:username/new-repo-name.git
```

例如：

```bash
git remote set-url origin git@github.com:zthinedge/Cpp-Backend-Lab.git
```

最后测试：

```bash
git push
```

---

## 21. 常见错误：Permission denied (publickey)

报错：

```text
Permission denied (publickey).
```

常见原因：

```text
1. 没有生成 SSH key
2. GitHub 没有添加本机公钥
3. 使用了 SSH 地址，但当前 key 没有权限
4. 复制的是私钥或者复制公钥不完整
```

检查方式：

```bash
ssh -T git@github.com
```

重新查看公钥：

```bash
cat ~/.ssh/id_ed25519.pub
```

把完整输出复制到 GitHub SSH keys 中。

---

## 22. 常见错误：remote origin already exists

报错：

```text
remote origin already exists
```

原因：

```text
当前本地仓库已经添加过 origin。
```

查看：

```bash
git remote -v
```

如果地址错了，直接修改：

```bash
git remote set-url origin git@github.com:username/repo.git
```

如果想删除重来：

```bash
git remote remove origin
git remote add origin git@github.com:username/repo.git
```

---

## 23. 常见错误：Updates were rejected

报错：

```text
Updates were rejected because the remote contains work that you do not have locally.
```

原因：

```text
远程仓库已经有提交，但本地没有这些提交。
```

常见场景：

```text
GitHub 创建仓库时勾选了 README
本地又重新 git init 并提交
```

推荐解决方式：

```bash
git clone git@github.com:username/repo.git
```

如果本地内容不重要，直接重新 clone 最干净。

如果想合并两边历史，可以使用：

```bash
git pull origin main --allow-unrelated-histories --no-rebase
git push -u origin main
```

刚创建的仓库，如果确认远程内容不重要，也可以强制覆盖：

```bash
git push -u origin main --force
```

但正式项目不要随便使用 `--force`。

---

## 24. 常用流程总结

### 24.1 第一次配置 SSH

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
cat ~/.ssh/id_ed25519.pub
ssh -T git@github.com
```

---

### 24.2 GitHub 空仓库 + 本地空文件夹

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

### 24.3 GitHub 已有 README

```bash
git clone git@github.com:username/repo.git
cd repo
```

---

### 24.4 本地已有仓库，绑定远程

```bash
git remote add origin git@github.com:username/repo.git
git push -u origin main
```

如果已经有 origin：

```bash
git remote set-url origin git@github.com:username/repo.git
```

---

## 25. 一句话总结

```text
SSH key 是本地电脑连接 GitHub 的身份凭证，一台电脑通常配置一次即可。
remote 是本地仓库绑定的远程仓库地址，origin 是最常用的默认远程别名。
GitHub 空仓库适合 git init，GitHub 已有 README / LICENSE / .gitignore 时适合 git clone。
```