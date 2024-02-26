---
abbrlink: 1849850651
categories:
  - 工作
  - Unity
  - Shader
---

# 【Unity】Pass 语法笔记

```shaderlab
Pass
{
    [Name]
    [Tags] // 提供给Unity一些额外信息
    [RenderSetup] // 渲染管线状态设置
    <passcode>
}
...
```

## Name

给 Pass 起名，以配合多种功能使用。

```shaderlab
Name "<passName>"
```

## Tags

类似 SubShader 中的 Tags，但此处是一些专门作用于 Pass 的 Tags。

### LightMode

在自定义灯光时使用，内置管线中不同的灯光使用不同的 Pass，该标签用于表面该 Pass 对应的灯光类型。不过该做法在 SRP 有所调整，所以具体的 LightMode 值还要看对应的渲染管线。

## RenderSetup

```shader
AlphaToMask {On | Off} //启用或禁用 GPU 上的 alpha-to-coverage 模式。
```

### Blend
指明如何将片元着色器输出值与渲染目标值混合

## passcode

passcode 是实际的渲染代码，目前支持 3 种写法。

- 固定函数着色器（过时）

  <https://docs.unity.cn/cn/2019.4/Manual/ShaderTut1.html>

  一个早期旧版着色器，使用内置的函数快速编写一些简单的着色器，不需要会 HLSL，但扩展能力差。

- 表面着色器（过时）

  <https://docs.unity.cn/cn/2019.4/Manual/SL-SurfaceShaders.html>

  使用 HLSL 编写，但对光照等功能进行了封装，不需要自己实现，也因此编写比较简单同时具备一定扩展能力。但新出的 SRP 不支持该写法，看样子官方要用 Shader Graph 代替它。

- 顶点和片元着色器

  <https://docs.unity.cn/cn/2019.4/Manual/SL-ShaderPrograms.html>

  接近原生的 HLSL 体验，光照等功能都需要自行实现，难度较高，但通用性最强，一般考虑利用改源码的方式实现，从而简化制作流程。
