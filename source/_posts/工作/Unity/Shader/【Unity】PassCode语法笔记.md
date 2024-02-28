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

- `#include "<file>"`

  引入其他文件代码。

- `#include_with_pragmas "<file>"`

  引入其他文件代码，并且包含其中的`#pragma`指令。

  使用该指令必须确保启用了编辑器中的缓存预处理器功能（2020.2.0a13 版开始默认启用）

-
