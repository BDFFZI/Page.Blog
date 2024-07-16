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

### 引用包

关于每个包在 CMakeLists 中的使用方式，有两种方式判断：

- 包中可能带有`usage`文件，里面有使用方法。
- 自行分析包中的`vcpkg.json`文件并编写方法。

示例引用方式如下：

```cmake
# 改编自gtest的usage文件
find_package(GTest CONFIG REQUIRED)
target_link_libraries(Executable PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
# 通过vcpkg.json分析得出
find_package(imgui CONFIG REQUIRED)
target_link_libraries(Executable PRIVATE imgui::imgui)
```
