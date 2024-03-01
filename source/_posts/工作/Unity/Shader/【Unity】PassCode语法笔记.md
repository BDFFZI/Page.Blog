---
abbrlink: 1849850651
categories:
  - 工作
  - Unity
  - Shader
---

# 【Unity】PassCode 语法笔记

https://docs.unity.cn/cn/2022.3/Manual/SL-ShaderPrograms.html

PassCode 区域存放实际的渲染代码，目前支持 3 种写法。

- 固定函数着色器（过时）

  <https://docs.unity.cn/cn/2019.4/Manual/ShaderTut1.html>

  一个早期旧版着色器，使用内置的函数快速编写一些简单的着色器，不需要会 HLSL，但扩展能力差。

- 表面着色器（过时）

  <https://docs.unity.cn/cn/2019.4/Manual/SL-SurfaceShaders.html>

  使用 HLSL 编写，但对光照等功能进行了封装，不需要自己实现，也因此编写比较简单同时具备一定扩展能力。但新出的 SRP 不支持该写法，看样子官方要用 Shader Graph 代替它。

- 顶点和片元着色器

  <https://docs.unity.cn/cn/2019.4/Manual/SL-ShaderPrograms.html>

  接近原生的 HLSL 体验，光照等功能都需要自行实现，难度较高，但通用性最强，一般考虑利用改源码的方式实现，从而简化制作流程。

根据兼容性下面只记录“顶点和片元着色器”写法。

## 代码块

所有的 PassCode 代码必须要写在代码块中，代码块里存放着 Shader 程序实际运行用的 HLSL 源代码。

共支持四种代码块形式：

- HLSLINCLUDE：可写在任意区域，区域下的所有 Pass 将自动包含块中的代码。
- HLSLPROGRAM：写在 Pass 区域中，是当前 Pass 使用的 Shader 代码。
- CGINCLUDE（仅内置管线）
- CGPROGRAM（仅内置管线）

```shaderlab
// 语法示例
Shader "Examples/ExampleShader"
{
  SubShader{

    HLSLINCLUDE
      // 当前区域内所有Pass的共享 HLSL 代码
    ENDHLSL

    Pass{

      HLSLPROGRAM
        // 当前Pass的 HLSL 代码
      ENDHLSL

    }
  }
}
```

## HLSL 语法

常规语法见微软的 [HLSL 文档](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl)

Unity 支持两种版本的 HLSL 语法，DirectX 9 样式和 DirectX 10+ 样式，两者主要在纹理采样有所区别，同时平台兼容性也不一样。

- DX 9：使用 `sampler2D、tex2D()` 采样纹理。全平台兼容。
- DX 10+：使用 `Texture2D、SamplerState、.Sample()` 采样纹理。部分语法在 OpenGL 平台无效。

部分操作也可选择使用 Unity 提供宏函数编写，从而解决兼容性差异。

## 预处理指令

### include

- `#include "<file>"`

插入其他文件（如 `.hlsl` 文件）内容。注意其不会插入内容中的`#pragma`指令。

### include_with_pragmas

- `#include_with_pragmas "<file>"`

插入其他文件内容，并且包含其中的`#pragma`指令。

使用该指令必须确保启用了编辑器中的缓存预处理器功能（2020.2.0a13 版开始默认启用）

### pragma

为着色器编译器提供其他类型的预处理器指令未涵盖的其他信息。

部分参数可多填，用空格分割。

- 声明表面着色器选项。

  - `#pragma surface <surface function> <lighting model> <optional parameters>`

- 声明着色器阶段函数。

  - `#pragma vertex <name>`
  - `#pragma fragment <name>`
  - `#pragma geometry <name>`
  - `#pragma hull <name>`
  - `#pragma domain <name>`

- 声明着色器变体和关键字。

  - `#pragma multi_compile <keywords>`：声明着色器变体。相关变体将全部编译。
  - `#pragma multi_compile_local <keywords>`：同上但改为使用本地关键字控制。
  - `#pragma shader_feature <keywords>`：声明着色器变体。仅使用变体将编译。
  - `#pragma shader_feature_local <keywords>`：同上但改为使用本地关键字控制。
  - `#pragma hardware_tier_variants <values>`：在编译给定图形 API 时为图形层（内置管线功能）添加关键字。
  - `#pragma skip_variants <keywords>`：删除指定的关键字。

- 声明着色器模型和所需 GPU 功能。

  https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderCompileTargets.html

  部分功能不能满足跨平台的需求所以默认被隐藏，如有需要必须用下列指令显式声明。

  - `#pragma target <value>`：声明最小着色器模型。
  - `#pragma require <value>`：声明兼容的最低 GPU 功能。

- 包含或排除给定图形 API 的代码。

  https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderCompilationAPIs.html

  Unity 默认为所有平台编译代码，但实际有些平台可能不支持或不需要，可借此排除。

  - `#pragma only_renderers <api>`：仅针对给定图形 API 编译程序。
  - `#pragma exclude_renderers <api>`：不要为给定图形 API 编译程序。

- 启用并设置 GPU 实例化功能。

  - `#pragma instancing_options <options>`

- 限定当前文件仅包含一次。

  - `#pragma once`

- 为目标 API 生成调试符号并禁用所有 API 优化。

  - `#pragma enable_<api>_debug_symbols`

- 关闭目标图形 API 的优化。

  - `#pragma skip_optimizations <api>`：可多写，用空格分割。

- 将反汇编的 HLSLcc 字节码嵌入到转换的着色器中。

  - `#pragma hlslcc_bytecode_disassembly`

- 启用涉及 NaN 处理的精确 IEEE 754 规则。 当前这仅影响 Metal 平台。

  - `#pragma disable_fastmath`

- 强制进行同步着色器编译。

  - `#pragma editor_sync_compilation`

- 使用 CBUFFER 宏时，即使目标平台不支持，也要写入相关代码。

  - `#pragma enable_cbuffer`

### 着色器模型和 GPU 功能

### 目标图形 API 和平台

### 着色器变体和关键字

### 着色器阶段函数
