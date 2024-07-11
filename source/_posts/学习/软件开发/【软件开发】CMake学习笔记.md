# 【软件开发】CMake 学习笔记

## CMake指令

```bash
# 将目标位置的CMakeLists构建到当前文件夹
cmake <path>
```

## CMakeLists指令

### 核心指令

通常必不可少的指令。

```cmake
# 描述CMake的最低支持版本，顶层CMake文件必带
cmake_minimum_required(VERSION <version>)
# 创建一个项目
project(<projectName>)
# 设置开发中的环境变量
set(<variableName> <variableValue>)
```

### 工具指令

一些功能性的实用指令。

```cmake
# 复制文件到指定位置，并可利用环境变量修改内容（新文件会自动移入构建目录）
configure_file(<inputFile> <outputFile>)
```

### 环境指令

用于描述开发环境

```cmake
# 设置所需的C++标准库版本（需在指定项目输出前使用）
set(CMAKE_CXX_STANDARD <version>)
set(CMAKE_CXX_STANDARD_REQUIRED True)

```

### 项目指令

用于描述项目信息

```cmake
# 表明该项目的输出是一个可执行文件
add_executable(<projectName> <sourceFiles>)
# 表明项目所用头文件的目录（指定完项目输出后才可用）
target_include_directories(<projectName> PUBLIC <headerDirs>)
```

## 参考资料

- [CMake 官方教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
