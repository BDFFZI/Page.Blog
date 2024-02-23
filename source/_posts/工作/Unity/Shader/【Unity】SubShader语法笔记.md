---
abbrlink: 2092124193
published: false
categories:
  - 工作
  - Unity
  - Shader
---

# 【Unity】SubShader 语法笔记

SubShader 是 ShaderLab 中最复杂的区域，这里存放着渲染时实际使用的逻辑代码，所以也是必须要有的区域。

- 一个 ShaderLab 结构中必须要有一个或多个 SubShader。
- 若编写了多个 SubShader 只会使用首个用户支持的 SubShader。
- 如果所有 SubShader 都不支持，将会使用 Fallback 着色器。

```shaderlab
// 可定义多个，然后根据用户设备的支持情况选择一个实际运行。
SubShader
{
    [LOD]
    [Tags]
    [CommonState]
    <Pass>
}
...
```

### LOD

LOD 用于表示子着色器在计算方面的需求大小，配合 Shader.maximumLOD 可以排除高于指定 LOD 的子着色器，从而实现根据不同硬件情况运行不同的着色器效果。

```subShader
// LOD的基本语法
LOD <number>
```

## Tags

Tags 本质就是一些键值对，用于给 Unity 提供一些额外的渲染参数。

```shaderlab
// Tags语法示例
Tags {
  "<TagName1>" = "<Value1>"
  "<TagName2>" = "<Value2>"
  ...
}
```

### 内置 Tags

#### Queue

渲染存在先后关系，这是通过一个整数值描述的，数值越大越靠后。而利用 Queue 可在 Shader 中显式指明该值。

Queue 支持以下键，这些键是一些常用顺序值的别名：

- Background：1000
- Geometry：2000，不透明物体
- AlphaTest：2450
- Transparent：3000，透明物体
- Overlay：3000

可以利用加减进一步控制顺序，如：

```shaderlab
Tags { "Queue" = "Geometry+1" }
```

#### RenderType

https://docs.unity.cn/cn/2019.4/Manual/SL-ShaderReplacement.html

RenderType 用于给 SubShader 分类，从而实现着色器批量替换功能，不过该功能不支持 SRP，所以已过时。

#### DisableBatching

使用批处理渲染时，相关物体的顶点数据将全部转换到世界空间，导致依赖物体空间的 Shader 失效。

如果遇到这种情况可利用 DisableBatching 禁止对该 Shader 使用批处理。



### 自定义 Tags

利用 Material.GetTag 可以读取 Unity 内置标签之外的标签，这对自定义渲染管线时非常有用，实际上 Unity 内置渲染管线的运作方式应该也是一样的原理。

### Pass

用于编写实际渲染用的着色器代码。

- 一次 Pass 的运行代表一次渲染，可定义多个。
- Pass 定义中允许的部分语句也可直接出现在 Subshader 中，这将同时设置所有通道的状态。

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

#### passcode

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

#### UsePass

借助 UsePass，也可不自行实现而可以直接使用现有的 Pass。

- 系统只会考虑目标第一个受支持的子着色器。
- 需要配合 Name 命令使用。

```shaderlab
UsePass "<shaderName/passName>"
...
```
