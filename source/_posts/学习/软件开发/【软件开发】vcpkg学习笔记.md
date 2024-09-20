---
abbrlink: 4146197106
date: "2024-7-16 9:08"
categories:
  - 学习
  - 软件开发
---

# 【软件开发】vcpkg 学习笔记

"vcpkg"是一个免费开源的 C++包管理器，可以以此很方便的处理第三方库的接入，且可嵌入进 CMake 中。

## 入门

入门教程请参照：  
https://learn.microsoft.com/zh-cn/vcpkg/get_started/get-started?pivots=shell-cmd

大概的接入流程是：

1. 安装 vcpkg。
2. 编写 CMakePresets，接入 vcpkg 的 CMake 工具链工具。（CMake 笔记中的 CMakePresets 示例）
3. 使用`vcpkg new --application`创建清单文件，填写所需库。
4. 在 CMakeLists 中使用`find_package()`获取 vcpkg 库。
5. 使用带有 vcpkg 的 CMakePresets 生成项目，完成接入。

## 配置

### 查找包

若要查看 vcpkg 支持的所有包，有两种方式：

- 可以在安装目录的“vcpkg/ports”文件夹内查看。
- 或者从在线的 github 仓库也可以：  
   https://github.com/microsoft/vcpkg/tree/master/ports

### 安装包

vcpkg 支持“经典模式”和“清单模式”两种方式安装包：

- 经典模式，通过终端指令程序化处理。
- 清单模式，通过编辑根目录的“vcpkg.json”文件处理。

推荐使用清单模式，因为这样使用更透明更清晰:  
https://learn.microsoft.com/zh-cn/vcpkg/reference/vcpkg-json

文件内容示例如下：

```json
{
  "dependencies": [
    "gtest",
    {
      "name": "imgui",
      "features": ["dx12-binding", "win32-binding"]
    }
  ]
}
```

#### 锁定版本

要想锁定包版本必须先提供基线参数，基线参数是vcpkg仓库一次提交的SHA（必须是本地vcpkg仓库中存在的版本），这样所有包版本将都被限制于必须大于等于该基线时的版本。

当提供基线后才可以使用`overrides`参数进行版本覆盖，以强制使用特定版本，示例如下：

```json
{
  "builtin-baseline": "333ba63a16024f05f9172fe403f9eced000389e5",
  "overrides": [
    {
      "name": "imgui",
      "version": "1.91.0"
    }
  ]
}
```


### 引用包

关于每个包在 CMakeLists 中的使用，基本由以下两个命令构成：

```cmake
find_package(<pkgCMakeName> CONFIG REQUIRED)
target_link_libraries(<projectName> PRIVATE <pkgCMakeProjectName>)
```

其中 `projectName` 是你自己需要用到该包的项目，而 `pkgCMakeName` 和 `pkgCMakeProjectName` 的内容则需要进行推断，具体有两种方法：

1. 通过官方的`usage`文件得出，这是官方编写的默认引入方法。
2. 通过查看包安装目录的一个文件得出：  
   例如要引入 gtest，通过查看 vcpkg_installed\x64-windows\share\GTestTargets.cmake 文件，其中文件名前缀`GTest`即`pkgCMakeName`，文件内的 add_library(GTest::gtest) 则表明`GTest::gtest`是`pkgCMakeProjectName`。

示例引用方式如下：

```cmake
# 改编自 gtest 的 usage 文件
find_package(GTest CONFIG REQUIRED)
target_link_libraries(Executable PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
# 通过分析 imguiTargets.cmake 得出
find_package(imgui CONFIG REQUIRED)
target_link_libraries(Executable PRIVATE imgui::imgui)
```
