---
abbrlink: 3993342442
date: 2024-01-14 11:43
categories:
  - 个人研究
  - 软件开发
---

# 【软件开发】Git 概念与常用命令

## Git 概念

### 存储方式

Git 是分布式存储，每一个 clone 下来的仓库都可以看成独立的个体，只是 Git 有提供同步功能，因此 Git 支持离线使用，因为本质上本地和云端是两个仓库。

### 仓库构成

#### 提交（commit）

提交是存储文件的最基本元素，记录了每次对文件的修改信息，同时也是仓库文件状态判断的基点。多个提交对文件的修改可能是顺序进行也可能是同步进行，同步的结果往往会导致冲突。

#### 分支（branch）

分支引用了提交并将其串成一条时间线，外界也由此得以查看提交。不同分支引用的时间线可能重叠也可能分离，形成一个树状结构，但最终分支都是要合并的，这也意味着所有提交最终都会形成一条唯一的时间线。

#### 子模块（submodule）

子模块是对其他仓库中的引用，具体而言是引用了分支-提交，对本仓库而言它也是一种受版本控制的文件，文件中存放着引用信息，所以和正常文件一样，修改时要添加到暂存区，删除时也要从仓库区中移除。

子模块所引用的分支版本信息可以从.git/index 文件中找到。

#### 标签（tag）

标签实质是给提交起一个别名从而方便识别，这是一个可选功能，通常人们会借此把一些关键的提交打上标签，如记录版本号和发布信息。

#### 远端（remote）

远端是用于同步的其他仓库，以便备份或多人协助，因为是分布式存储，每个仓库都是独立的个体，所以不提供远端也是可以的。

### 工作区与暂存区

这两个区域是本地工作用仓库独有的内容，在云端的裸仓库中没有这两者的存在。这主要是为了安全和便于工作使用的临时区域，每一次的文件修改都要按顺序提交到每个区域，当仓库指向的提交变更时它们便会被重置。

- 工作区：用户编写代码的地方，文件资源管理器能直接看到的部分。
- 暂存区：提交代码前必须先将工作区的所有的文件添加到暂存区。
- 仓库区：将暂存区的代码修改变成提交，真正的存储到仓库中。

## 常用命令

### 通用项

#### 选项

- HEAD：表示最新的一次提交。
- HEAD~1：表示最新提交的前一次提交。
- -h：任何命令加该选项，可以快速查看使用方式。
- --help：任何命令加该选项，可以查看命令的详细手册。

#### 规则

- commit、tag、branch 互换：

  tag 是 commit 的别名，两者等价。使用 branch 则会使用其 HEAD。

- file 通配：

  file 支持 glob 通配符，也可用“.”表示全部可处理文件。

### 仓库初始化和配置

```bash
git init [--bare] #在当前文件夹创建一个空仓库
git clone --recursive <url> #从远端克隆一个仓库到当前目录


git config <name> #查看仓库的指定项配置
git config [--global] <name> <value> #添加或修改[全局]仓库配置
git config [--global] --unset <name> #删除[全局]仓库配置
git config [--local] --list #查看[本地]仓库所有配置
```

### 仓库信息查看

```bash
git branch -a #查看仓库中的所有分支
git submodule #查看当前分支的子模块
git log #查看当前分支的所有提交记录
git tag #查看当前仓库所有标签
git status #查看存储区状态
git diff #查看文件差异
git reflog #查看当前仓库操作记录（可用于还原reset）
```

### 存储区处理

```bash
git ls-files [--cached] #查看工作区[暂存区]文件

git add <file> #提交工作区文件到暂存区
git rm [--cached] <file>... #删除工作区[暂存区]文件
git restore [--staged] <file> #复原工作区[暂存区]文件
git commit -m '<message>' #提交暂存区文件到仓库区

git stash #暂存工作区修改，使工作区看起来干净了，从而安全执行切换分支等命令
git stash pop #取出暂存的工作区修改

git reset [--hard] <commit> #将当前分支[包含工作区]重置到某次提交的状态
```

### 分支处理

```bash
git branch -a #查看所有分支（包括远端）
git branch <new-branch> #创建一个基于当前分支最新提交的新分支
git branch <new-branch> <commit> #创建一个基于目标提交的新分支
git branch -m <new-name> #重命名当前分支
git branch -d <branch> #删除目标分支
git branch -u <up-branch> <branch> #设置上流分支

git checkout -b <new-branch> <branch> #创建并切换到一个基于目标分支创建的新分支
git checkout --orphan temp #保持工作区不变，创建并切换到一个空分支，
git switch -c <new-branch> <commit> #创建并切换到基于某次提交的新分支
git switch <branch> #切换到目标分支
git switch --detach <commit> #创建并切换到以某次提交为头的临时匿名分支

git rebase -i <commit> #进入提交融并功能，可融并指定提交之后的所有提交（使用空融并配置可以退出该功能）
git rebase <branch> #将目标分支覆盖到当前分支
git merge <branch> #将目标分支合并到当前分支
```

### 子模块处理

```bash
git submodule add <url> <path> #添加一个子模块到目标路径
git submodule init #初始化子模块，将子模块信息填入config中
git submodule update #检出父项目期望版本的子模块内容，使用前需确保子模块已初始化
git submodule status #查看子模块的状态，例如是否有修改后未提交的内容
git submodule set-url <path> <url> #重新设置子模块的源仓库地址
git submodule deinit <path> #移除子模块，恢复到未初始化的状态（这不是删除，彻底删除见后续的“常用操作”）
```

### 标签处理

```bash
git tag -f <tagname> [<commit>] #给当前[指定]提交打上标签
git tag -d <tagname> #删除指定标签
```

### 远端处理

```bash
git remote #查看当前所有的远端仓库
git remote show <name> #查看指定远端仓库的具体信息，包括pull、push的目标设置
git remote add <name> <url> #添加一个远端仓库
git remote rename <name> <new-name> #重命名远端仓库
git remote remove <name> <new-name> #删除远端仓库
git remote set-url <name> <url> #修改远端仓库地址
git remote set-url {--add|--delete} <name> <newurl> #为远端仓库额外添加或删除地址，借此实现多端提交。
git remote set-head <name> {-a|<branch>} #修改远程默认分支为{自动识别|指定}的分支
git remote prune <name> #删除远端仓库已不存在但本地残留的分支

git fetch #获取所有远端分支
git pull #获取并合并远端分支到当前状态，等同于 fetch + merge
git push [--force] #将当前本地分支状态[强制]推送到远端分支
git push <remote> -d <branch> #删除目标远程上的分支
```

## 常用操作

### 处理网络代理

```bash
# 添加网络代理，url例如http://localhost:10809，具体根据不同的代理软件可能有所不同。
git config --global http.proxy <url>
git config --global https.proxy <url>
# 清除网络代理
git config --global --unset http.proxy
git config --global --unset https.proxy
```

### 彻底删除子模块

```bash
git submodule deinit <path> #将子模块转为未初始化状态
git rm <path> #在子模块未初始化时调用，能彻底删除子模块引用，且自动修改.gitmodules文件
rm -rf .git/modules/<path> #删除失效的子模块仓库
```

### 撤回上次提交版本

```bash
git reset HEAD~1 #将仓库区和暂存区撤回到上次提交之前的状态
git push --force #强制覆盖远端分支以丢弃上次提交版本
```

### 清除所有历史记录

```bash
git checkout --orphan temp #创建并切换到一个新的空分支，保留工作区文件
git branch -D <branch> #删除旧分支
git branch -m <branch> #空分支继续使用旧分支的名称
git add . #提交当前所有文件到空分支暂存区
git commit -m '-' #提交暂存区文件作为初始提交
git push --force #强制覆盖到远程仓库，分支记录将重新开始
```

### 清除工作区中所有未被跟踪的文件

无法撤销，用前一定要备份！

```bash
git clean -dfx #清除工作区中所有未被跟踪的文件
```

### Unity项目默认忽略文件

```text
/*/*
/*.*
!/Assets/*
!/Packages/*
!/ProjectSettings/*
!/.gitignore
```

## gitignore 文件

### 书写格式

- 每行一个忽略项。

- 支持 glob 通配符。

- 限定忽略项类型：

  - 文件或目录：只填完全名称，如`test`、`text.txt`。
  - 仅目录：添加`/`后缀，如`test/`。

- 限定忽略项位置：

  - 从当前目录匹配：带`/`前缀，如`/test`。
  - 从任意目录匹配：不带任何前缀，如`test`。

- 添加`!`前缀取消忽略，如`!test.txt`。

  注意！如果是整个目录被忽略，那该目录的子目录无法取消忽略。解决方法也很简单，不要直接忽略目录，而是用通配符忽略其中的文件。例如实现忽略除指定文件夹外的其他文件：

  ```bash
  /*/* # 忽略当前目录所有子目录文件
  /*.* # 忽略当前目录所有文件
  !/CMakeLists.txt # 取消忽略文件
  !/Assets/* # 取消忽略子目录文件
  ```

### 取消跟踪

gitignore 只能忽略未跟踪文件，如果文件已被跟踪需先从仓库中删去。

```bash
git rm --cached <file> # 删除暂存区中的某文件
```

## 参考资料

- [Git Submodule 命令与理解](https://zhuanlan.zhihu.com/p/156615362)
- [Git 合并多个提交的正确方式](https://zhuanlan.zhihu.com/p/139321091)
- [如何在 Git 中忽略文件和文件夹](https://www.freecodecamp.org/chinese/news/gitignore-file-how-to-ignore-files-and-folders-in-git/)
