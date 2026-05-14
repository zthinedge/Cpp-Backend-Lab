# Git Commit Message

## 1. 这篇文章解决什么问题？

`commit message` 看起来只是提交时顺手写的一句话，但它会直接影响后续维护体验。

糟糕的提交信息通常长这样：

```text
update
fix
修改
wip
final
```

当项目过一段时间再回头看，就很难知道：

```text
这次提交到底改了什么
为什么要这么改
影响范围在哪里
能不能安全回退
```

这篇只讲提交信息怎么写得清楚一点。

---

## 2. commit message 的作用

提交信息不是给 Git 看的，Git 只关心提交对象和文件快照。

提交信息主要是给人看的：

```text
给未来的自己看
给队友 code review 时看
给排查 bug 时看
给生成 changelog 时看
给回退提交时看
```

一个好的 commit message 至少应该回答：

```text
这次提交做了什么？
属于哪类改动？
影响哪个模块？
```

---

## 3. 推荐格式

个人项目里可以使用一个简化版格式：

```text
<type>(<scope>): <summary>
```

例如：

```text
feat(redis): add RESP array parser
fix(http): handle keep-alive timeout
docs(git): add rollback note
refactor(net): simplify event loop cleanup
```

三个部分分别是：

```text
type    改动类型
scope   影响范围
summary 简短说明
```

如果项目很小，`scope` 可以省略：

```text
fix: handle empty request body
```

---

## 4. 常见 type 怎么选？

常用类型：

```text
feat      新功能
fix       修复 bug
docs      文档修改
style     代码格式，不影响逻辑
refactor  重构，不新增功能也不修 bug
test      测试相关
chore     构建、脚本、依赖、杂项维护
perf      性能优化
build     构建系统相关
ci        CI 配置相关
```

不用一开始就追求非常复杂的规范。

对个人 C++ 后端项目来说，最常用的通常是：

```text
feat
fix
docs
refactor
test
chore
```

---

## 5. scope 写什么？

`scope` 表示影响范围。

可以写模块名：

```text
feat(parser): add bulk string parser
fix(server): close fd on accept failure
```

也可以写目录或主题：

```text
docs(git): add conflict resolution note
chore(cmake): enable warnings
```

C++ 后端项目里常见 scope：

```text
net
http
redis
parser
storage
thread
cmake
test
docs
git
```

`scope` 不需要特别精确，但应该让人一眼知道这次提交主要影响哪里。

---

## 6. summary 怎么写？

`summary` 是一句短说明。

推荐习惯：

```text
使用动词开头
说明这次提交做了什么
不要写太空泛的话
```

比较好的例子：

```text
fix(http): reject malformed request line
feat(redis): support integer reply encoding
docs(git): explain reset and revert
```

不太好的例子：

```text
fix bug
update code
change files
work in progress
```

原因是后者没有告诉别人到底改了什么。

---

## 7. 一次 commit 应该多大？

一个实用判断是：

```text
一次 commit 只做一件完整的事
```

例如这些可以分开：

```text
1. 新增一个 RESP parser
2. 给 parser 添加测试
3. 调整 README 文档
4. 格式化旧代码
```

不推荐一个 commit 同时包含：

```text
修 bug
改 CMake
重命名目录
格式化全项目
顺手改 README
```

这样的提交以后很难 review，也很难回退。

---

## 8. 提交前先检查什么？

提交前推荐固定流程：

```bash
git status
git diff
git diff --cached
```

然后再提交：

```bash
git add src/parser.cpp tests/parser_test.cpp
git commit -m "feat(parser): support array reply parsing"
```

这里不建议总是无脑：

```bash
git add .
```

因为它可能把临时文件、调试文件、错误修改一起提交进去。

---

## 9. 多行 commit message

如果一次提交比较复杂，可以使用多行说明：

```bash
git commit
```

进入编辑器后写：

```text
fix(http): close connection on parse failure

The previous code kept the socket open after a malformed request.
This could leave idle connections in the event loop longer than needed.

Also add a regression test for an invalid request line.
```

第一行是摘要。

空一行后写更详细的说明：

```text
为什么改
怎么改
有什么影响
```

个人学习项目不一定每次都写多行，但复杂提交值得写清楚。

---

## 10. amend 最近一次提交信息

如果刚提交完发现 message 写错了：

```bash
git commit --amend -m "docs(git): add commit message guide"
```

如果需要打开编辑器修改：

```bash
git commit --amend
```

注意：

```text
amend 会生成新的 commit hash
如果已经 push 到共享分支，不要随便 amend
```

这一点和 `06` 里的回退逻辑一致。

---

## 11. 常见提交信息示例

文档：

```bash
git commit -m "docs(git): add gitignore note"
```

新增功能：

```bash
git commit -m "feat(redis): parse simple string reply"
```

修 bug：

```bash
git commit -m "fix(net): handle epoll wait interruption"
```

重构：

```bash
git commit -m "refactor(server): split request parsing logic"
```

测试：

```bash
git commit -m "test(parser): cover invalid bulk string"
```

构建维护：

```bash
git commit -m "chore(cmake): enable compile warnings"
```

---

## 12. 不要把提交信息写成流水账

不推荐：

```text
modify parser and add some files
```

更推荐：

```text
feat(parser): support RESP bulk string
```

不推荐：

```text
fix
```

更推荐：

```text
fix(server): avoid double close on client fd
```

提交信息不需要很长，但要有信息量。

---

## 13. 一句话总结

好的 commit message 应该让人不用打开 diff，也能大致知道这次提交的类型、范围和目的。

个人项目可以先坚持 `<type>(<scope>): <summary>`，再配合“小而完整”的提交习惯，后续看历史会轻松很多。
