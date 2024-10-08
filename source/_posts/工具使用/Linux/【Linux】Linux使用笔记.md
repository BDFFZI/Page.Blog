---
abbrlink: 2845903494
date: 2024-5-31 17:30
categories:
  - 工具使用
  - Linux
---

# 【Linux】Linux 使用笔记

## 权限属性

权限属性共 10 位，如 drwxrwxrwx。

### 文件类型

第一位表示文件类型：

- d：目录。
- l：目录映射。
- -：文件。

### 权限控制

其他位分别是面向不同用户的读写权限：

- 区间一：文件所属者。
- 区间二：与所属者同组的用户。
- 区间三：其他用户。

读写权限分三种类型，对目录和文件作用各不同：

- 对于文件：
  - r：可读。
  - w：可写。
  - x：可执行（对二进制程序外没意义）。
- 对于目录：
  - r：可查看目录下文件。
  - w：可创建删除目录下文件。
  - x：可 cd 进目录，查看目录下文件的详细属性和文件内容。
