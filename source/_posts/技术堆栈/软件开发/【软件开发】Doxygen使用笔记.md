---
abbrlink: 1794241118
date: 2024-6-11 16:38
categories:
  - 个人研究
  - 软件开发
---

# 【软件开发】Doxygen 使用笔记

Doxygen 是通过代码注释生成文档的事实标准，借用该工具可以将文档内容与代码写在一起方便维护。

https://github.com/doxygen/doxygen

## 注释语法

采用特殊的注释形式，以便 Doxygen 识别为文档内容。

文档内容支持使用一些基本 Markdown 语法编写。

- 多行注释（放在注释目标的上面）

  ```c
  /*!
  注释内容
  */
  ```

- 单行注释

  - 放在注释目标的上面
    ```c
    //! 注释内容
    ```
  - 放在注释目标的后面
    ```c
    //!< 注释内容
    ```

- 文件注释（需放在文件开头）
  ```c
  /*! @file
  注释内容
  */
  ```

## 生成方式

Doxygen 必须通过读取配置文件来运行构建程序。

```bash
doxygen -g # 创建默认配置文件（Doxyfile）
doxygen [<file>] # 以默认[指定]配置文件运行构建
```

## 配置文件

一些常用的配置文件参数，构建前一般需要调整：

### 生成配置

- PROJECT_NAME：项目名称。
- EXTRACT_ALL：实行强力提取。
- GENERATE_TREEVIEW：生成侧边树样式的文档。
- DISABLE_INDEX：当启用 GENERATE_TREEVIEW 时建议打开。

### 输入配置

- INPUT：需要构建的源文件目录。
- RECURSIVE：是否递归源文件目录。
- FILE_PATTERNS：需要构建的源文件类型。
- EXTENSION_MAPPING：扩展名映射。

### 输出配置

- OUTPUT_LANGUAGE：输出语言。
- GENERATE_LATEX：是否输出 Latex 格式。一般不需要，建议关闭。
- GENERATE_HTML：是否输出 HTML 格式。
- HTML_OUTPUT：HTML 格式输出目录。

## 其他提示

- 关于文件路径参数的填写，若要使用本地路径，必须添加`./`前缀。

## 参考教程

- [Doxygen 10 分钟入门教程](https://cedar-renjun.github.io/2014/03/21/learn-doxygen-in-10-minutes/)
