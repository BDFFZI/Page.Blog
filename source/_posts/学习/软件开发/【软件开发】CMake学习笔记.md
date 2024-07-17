---
abbrlink: 2795034225
date: "2024-7-16 9:07"
categories:
  - 学习
  - 软件开发
---

# 【软件开发】CMake 学习笔记

## CMake 指令

https://cmake.org/cmake/help/latest/manual/cmake.1.html

```bash
# 将目标位置的CMakeLists生成为原生构建系统的项目
cmake <path>
# 编译已生成的原生构建系统的项目
cmake --build <path>
```

## CMakeLists 指令

### 核心指令

通常必不可少的指令。

```cmake
# 描述CMake的最低支持版本（顶层CMake文件必带）
cmake_minimum_required(VERSION <version>)
# 创建一个项目
project(<projectName>)
# 添加一个子目录（子目录需要带有CMakeLists）
add_subdirectory(<source_dir>)
# 寻找一个模块（带有特定配置文件的目录将会被识别为模块，可当成项目使用）
find_package(<packageName> CONFIG REQUIRED)
```

### 工具指令

一些功能性的实用指令。

```cmake
# 设置开发中的环境变量
set(<variableName> <variableValue>)

# 获取一组文件并将其路径打包在一个环境变量中，支持glob语法
file({GLOB | GLOB_RECURSE} <outputVarName> <inputFile>...)
# 在vs中将指定文件分类到与文件系统一致的筛选器结构，而不是默认筛选器。
source_group(TREE <fileRootDir> FILES <inputFile>...)

# 复制并修改文件到指定位置。自动替换文件中的环境变量值（@<var>@）并自动移入当前CMakeLists的构建目录。
configure_file(<inputFile> <outputFile>)
```

### 环境指令

用于描述开发环境

```cmake
# 设置所需的C++标准库版本（需在指定项目输出前使用）
set(CMAKE_CXX_STANDARD <version>)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 添加全局附加包含目录
include_directories(<headerDir>...)
# 添加全局附加库
link_libraries({<projectName>|<libFile>...})
```

### 项目指令

针对单个项目的信息描述指令。

```cmake
# 描述项目输出（必须要设置）

# 输出为可执行文件
add_executable(<projectName> <sourceFile>...)
# 输出为静态/动态库文件
add_library(<projectName> [STATIC|SHARED] <sourceFile>...)
# 特殊的没有输出的项目
add_custom_target(<projectName> SOURCES <sourceFile>...)

# 描述项目环境（指定完项目输出后才可用）

# 添加附加包含目录
target_include_directories(<projectName> {PUBLIC|PRIVATE} <headerDir>...)
# 添加附加库
target_link_libraries(<projectName> {PUBLIC|PRIVATE} {<projectName>|<libFile>...})
# 分类到vs中的解决方案文件夹（3.26之前需先打开 USE_FOLDERS 功能）
set_target_properties(<projectName> PROPERTIES FOLDER <folderName>)
```

#### 选项说明

- `PUBLIC`：使用该选项添加引用，引用将传染给使用该项目的其他项目。
- `PRIVATE`：引用不具备传染性，其他使用该项目的项目可能要再次添加引用。

## 环境变量

仅写出部分通常只读的变量，需用户配置的变量见“环境指令”章节。

### 项目相关

- `<projectName>_VERSION_MAJOR`：声明项目时额外提供的主版本号。
- `<projectName>_VERSION_MINOR`：声明项目时额外提供的次版本号。

### 目录相关

- `PROJECT_BINARY_DIR`：上次调用`project()`的 CMakeLists 对应的构建目录。
- `PROJECT_SOURCE_DIR`：上次调用`project()`的 CMakeLists 对应的源目录。

注意：`add_subdirectory()`不会影响以上两条变量的值。

- `CMAKE_SOURCE_DIR`：顶级 CMakeLists 的源目录。
- `CMAKE_BINARY_DIR`：顶级 CMakeLists 的构建目录。
- `CMAKE_CURRENT_SOURCE_DIR`：当前 CMakeLists 的源目录。
- `CMAKE_CURRENT_BINARY_DIR`：当前 CMakeLists 的构建目录。

## CMakePresets

https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html

“CMakePresets.json”是 CMake 的配置文件，存放在项目的根目录中，用于配置一些 CMake 构建选项。

示例内容如下：

```json
{
  //CMakePresets所用的版本
  "version": 6,
  //定义生成配置。可定义多个，然后在构建指令中指明。
  "configurePresets": [
    {
      //配置名称
      "name": "default",
      //所用的生成器，用于将CMake项目转为原生项目
      "generator": "Visual Studio 17 2022",
      //生成输出目录（构建目录）
      "binaryDir": "${sourceDir}/build",
      //配置CMake缓存变量（一种长期存储的环境变量，存在CMakeCache.txt文件中）
      "cacheVariables": {
        //构建中使用的一些构建工具，由cmake语言写成
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```

配置完毕后生成时通过添加`--preset`选项，如`cmake --preset=<presetName>`指令使用。

## 使用技巧

- 仅添加些文件到 CMake 生成的解决方案文件夹中？

  无法直接支持，但可使用`add_custom_target`指令间接实现：  
  https://stackoverflow.com/questions/20251829/is-it-possible-to-add-files-to-a-cmake-generated-solution-folder-in-visual-studi

## 参考资料

- [CMake 官方教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [CMake 官方文档](https://cmake.org/cmake/help/latest/index.html)
- [理解 cmake 中 PRIVATE、PUBLIC、INTERFACE 的含义和用法](https://blog.csdn.net/qq_41314786/article/details/129970547)
