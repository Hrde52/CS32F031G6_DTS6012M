## 推送
```bash
git add .
git commit -m "完成flash参数写入与读取"
git push -u origin test
```

## 克隆仓库：

```bash
git clone git@github.com:Hrde52/CS32F031G6_DTS6012M.git
cd CS32F031G6_DTS6012M  # 进入克隆下来的项目目录
```
获取并切换到远程的 test 分支：
现在使用 git switch 来创建本地 test 分支并跟踪远程的 origin/test：

```bash
git switch -c test origin/test
```
这个命令会自动拉取 test 分支的最新代码。

