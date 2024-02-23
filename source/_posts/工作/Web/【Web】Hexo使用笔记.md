---
abbrlink: 3410375722
date: "2024-2-3 21:17"
categories:
  - 工作
  - Web
---

# 【Web】Hexo 使用笔记

## 基本概念

### 项目结构

| 目录            | 描述           |
| --------------- | -------------- |
| \_config.yml    | 网站的配置信息 |
| themes          | 主题文件夹     |
| scaffolds       | 模板文件夹     |
| scripts         | 脚本文件夹     |
| source          | 资源文件夹     |
| source/\_drafts | 草稿文件夹     |
| source/\_posts  | 发布文件夹     |

### 文章存储

文章一般存放在“source/\_drafts”和“source/\_posts”文件夹，两者分别对应文章的两种状态：草稿和发布。

存储为草稿状态的文章默认是不在网站显示的，可通过修改网站配置“render_drafts”更改。

只要将 md 文件放入这些文件夹即可被 Hexo 识别到。

### 布局

布局即是创建文章所用的模板文件，其统一存放在 scaffolds 文件夹，可用\_config.yml 文件中 default_layout 参数指定默认布局。

Hexo 支持三种默认布局，用其创建的文章会自动存放到不同的文件夹，如果是自定义布局则和 post 相同。

| 布局       | 路径            |
| ---------- | --------------- |
| page       | source          |
| draft      | source/\_drafts |
| post       | source/\_posts  |
| 自定义布局 | source/\_posts  |

编写布局文件时还可以利用 Hexo 的变量功能，使创建文章时自动编写 Front-matter，变量需要用两个大括号括起。

| 变量   | 描述         |
| ------ | ------------ |
| layout | 布局         |
| title  | 标题         |
| date   | 文件建立日期 |

### Front-matter

Front-matter 是每个文章开头处用“---”或“;;;”分割的区域，两者分别对应着“YAML”和“JSON”两种写法，它用于指定当前文章的属性信息，Hexo 中部分功能的运行依赖其中的信息。

常用的一些文章属性如下。

| 参数       | 描述                                     |
| ---------- | ---------------------------------------- |
| title      | 标题                                     |
| categories | 分类                                     |
| date       | 建立时间                                 |
| published  | 是否发布                                 |
| layout     | 布局（主题可能识别该项从而影响页面效果） |
| updated    | 更新时间                                 |
| comments   | 开启评论                                 |

## 常用命令

使用命令前需要全局安装 hexo-cli，这样才能使用 hexo 命令。

```bash
npm install hexo-cli -g
```

### 项目

```bash
hexo init [folder] # 创建一个博客网站
hexo {g|generate} # 编译网站静态资源（很多pages服务自带构建功能，所以该命令一般不用）
hexo clean # 清除项目的缓存文件
```

### 后台

```bash
hexo config [key] [value] # 查看或修改网站配置
hexo list <type> # 列出网站数据
hexo server [--draft] # 启动本地服务器以预览网站[包括草稿文章]
hexo {d|deploy} # 将网站部署到托管平台
```

### 写作

```bash
hexo new [layout] <title> # 新建一篇文章
hexo publish <filename> #发布一篇文章
```
