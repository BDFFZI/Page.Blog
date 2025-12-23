---
abbrlink: 3793607597
date: 2024-4-26 11:25
categories:
  - 工具使用
  - Unity
  - Unity文档笔记
  - 图形
  - 着色器
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

## 基本结构

### 代码块

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

### HLSL 语法

代码块中使用正常的 HLSL 编写着色器代码，对于常规的 HLSL 语法可见微软的 [HLSL 文档](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl)

Unity 支持两种版本的 HLSL 语法，DirectX 9 样式和 DirectX 10+ 样式，两者主要在纹理采样有所区别，同时平台兼容性也不一样。

- DX 9：使用 `sampler2D`、`tex2D()` 采样纹理。全平台兼容。
- DX 10+：使用 `Texture2D`、`SamplerState`、`.Sample()` 采样纹理。部分语法在 OpenGL 平台无效。

部分操作也可选择使用 Unity 提供宏函数编写，从而解决兼容性差异。

### GLSL 语法

https://docs.unity.cn/cn/2022.3/Manual/SL-GLSLShaderPrograms.html

除了 HLSL，Unity 还支持使用 GLSL 编写着色器，相应的 GLSL 代码必须使用相应的`GLSLPROGRAM`代码块，但这种方式并不提倡。

## 预处理指令

### include

- `#include "<file>"`

插入其他文件（如 `.hlsl` 文件）内容。注意其不会插入内容中的`#pragma`指令。

### include_with_pragmas

- `#include_with_pragmas "<file>"`

插入其他文件内容，并且包含其中的`#pragma`指令。

使用该指令必须确保启用了编辑器中的缓存预处理器功能（2020.2.0a13 版开始默认启用）

### define

- `#define <key> <value>`
- `#undef <key>`

定义或取消定义关键字，用于实现宏函数或编译时常量。

### if

配合关键字定义实现编译时分支功能。注意其无法控制内容中的`#pragma`指令。

- 根据关键字是否被定义来分支。

  - `#ifdef <key>`
  - `#ifndef <key>`

- 根据关键字的值来分支

  - `#if <key>`
  - `#elif <key>`

- `#else`
- `#endif`

### pragma

为着色器编译器提供其他类型的预处理器指令未涵盖的其他信息。

部分参数可多填，用空格分割。

#### 常用编码指令

- 声明表面着色器选项。

  - `#pragma surface <surface function> <lighting model> <optional parameters>`

- 声明着色器阶段函数。

  - `#pragma vertex <name>`
  - `#pragma fragment <name>`
  - `#pragma geometry <name>`
  - `#pragma hull <name>`
  - `#pragma domain <name>`

- 声明着色器变体和关键字。

  https://docs.unity.cn/2022.3/Documentation/Manual/SL-MultipleProgramVariants.html

  - `#pragma multi_compile <keywords>`：声明着色器变体。相关变体将全部编译。
  - `#pragma multi_compile_local <keywords>`：同上但改为使用本地关键字控制。
  - `#pragma shader_feature <keywords>`：声明着色器变体。仅使用变体将编译。
  - `#pragma shader_feature_local <keywords>`：同上但改为使用本地关键字控制。
  - `#pragma hardware_tier_variants <values>`：在编译给定图形 API 时为图形层（内置管线功能）添加关键字。
  - `#pragma skip_variants <keywords>`：删除指定的关键字。

- 限定当前文件仅包含一次。

  - `#pragma once`

#### 特殊功能开关

- 声明着色器模型和所需 GPU 功能。

  https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderCompileTargets.html

  部分功能不能满足跨平台的需求所以默认被隐藏，如有需要必须用下列指令显式声明。

  - `#pragma target <model>`：声明最小着色器模型。
  - `#pragma require <feature>`：声明兼容的最低 GPU 功能。

- 启用并设置 GPU 实例化功能。

  - `#pragma instancing_options <options>`

- 启用涉及 NaN 处理的精确 IEEE 754 规则。 当前这仅影响 Metal 平台。

  - `#pragma disable_fastmath`

#### 编译阶段控制

- 包含或排除给定图形 API 的代码。

  https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderCompilationAPIs.html

  Unity 默认会为当前构建目标列表中所有的平台编译着色器，但实际有些平台可能不支持或不需要某些着色器程序，可借此排除。

  - `#pragma only_renderers <api>`：仅针对给定图形 API 编译程序。
  - `#pragma exclude_renderers <api>`：不要为给定图形 API 编译程序。

- 为目标 API 生成调试符号并禁用所有 API 优化。

  - `#pragma enable_<api>_debug_symbols`

- 关闭目标图形 API 的优化。

  - `#pragma skip_optimizations <api>`：可多写，用空格分割。

- 将反汇编的 HLSLcc 字节码嵌入到转换的着色器中。

  - `#pragma hlslcc_bytecode_disassembly`

- 强制进行同步着色器编译。

  - `#pragma editor_sync_compilation`

- 使用 CBUFFER 宏时，即使目标平台不支持，也要写入相关代码。

  - `#pragma enable_cbuffer`

## 着色器语义

编写 HLSL 着色器程序时，输入和输出变量必须要通过语义来表明其“意图”，从而正确接收或传递渲染管线中的数据。

以下介绍一些 Unity 支持的常用语义，其他更特殊的语义需自行查看 HLSL 文档并检查是否可用。

### 顶点着色器输入阶段

顶点阶段中的大多语义与用户定义的网格数据布局有关，此处以 Unity 默认的布局结构为准。

- `POSITION`：顶点位置，通常为`float3`。
- `NORMAL`：顶点法线，通常为`float3`。
- `TEXCOORD0`、`TEXCOORD1`、`TEXCOORD2`、`TEXCOORD3`：顶点 uv，通常为`float2`。
- `TANGENT`：顶点切线，通常为`float4`。
- `COLOR`：顶点颜色，通常为`float4`。

当声明的变量的分量多余实际的输入变量时，多余部分按默认值填充，默认值为`(0,0,0,1)`。

### 顶点着色器输出及片元着色器输入阶段

- `SV_POSITION`：顶点的裁剪空间位置，必须为`float4`。
- `其他非特殊语义`：用于标识从顶点传递到片元的自定义数据。

#### 语义限制

在渲染管线中传递的数据必须用语义标记，但大多数现代 GPU 其实并不关心语义是什么，所以当要传递自定义数据时随意搭配一些不冲突的语义即可。

但在一些老旧的系统在，比如着色器模型 2，会有一些特殊语义规则：

- `TEXCOORD0`、`TEXCOORD1`：代表任意高精度数据，如纹理坐标、位置。
- `COLOR0`、`COLOR1`：任意 0-1 的低精度数据，如简单的颜色。

因此为了最佳的跨平台支持，应尽可能使用类似`TEXCOORD<num>`这样的语义传递自定义数据。

#### 插值器限制

顶点着色器输出同时也可以作为片元着色器的输入，因为光栅化会自动将顶点数据通过插值器插值成像素数据。

但插值器也存在一些数量限制：

- 最多 8 个：OpenGL ES 2.0(Android)、Direct3D 11 9.x level(Windows Phone)。
- 最多 10 个：着色器模型 3.0。
- 最多 16 个：OpenGL ES 3.0(Android)、Metal(iOS)。
- 最多 32 个：着色器模型 4.0。

如果插值器数量不够用，可以考虑共用插值器，比如因为插值器是支持 4 位浮点数的，所以可以将两个 2 位浮点数放在一起传递。

此外从性能的角度考虑，通常也都应该尽可能减少插值器的使用数量。

### 片元着色器输出阶段

- `SV_Target`：片元着色器输出的颜色，必须为任意精度的 4 分量浮点向量。
- `SV_Target<number>`：当使用“多渲染目标”渲染技术时的输出颜色，如 SV_TARGET4。
- `SV_Depth`：自定义深度输出，必须为 float 类型。非必要不要使用，这会产生和 alpha 测试（使用`clip()`）类似的性能成本。

### 其他特殊语义

这类语义可以提供一些额外的特殊信息，一般用来实现一些特殊效果。

#### 顶点着色器输入

- `SV_VertexID`：顶点在网格中的编号，为无符号整数。需要着色器模型 3.5 支持。

#### 片元着色器输入

- `VPOS`：屏幕空间像素位置，通常为`float4`但考虑兼容性应使用`UNITY_VPOS_TYPE`作为变量类型。需要着色器模型 3.0 支持。
- `VFACE`：表面是否正向摄像机，返回 0 或 1 的值。需要着色器模型 3.0 支持。

## 着色器属性

可以直接在代码块中定义变量，接着只需提供这些变量的名称和类型信息，就可以通过多种外部途径读写这些值，从而使着色器程序参数化。

### 提供着色器属性值

有以下方式可以向着色器程序提供着色器属性值，使用时需确保与代码块中的变量名一致且类型匹配。

1. MaterialPropertyBlock 中设置的每渲染器值。
2. 渲染对象所用材质球上的值，包括直接用代码传入的运行时值或`Properties`块中定义的序列化值。
3. 全局着色器属性值。
4. 该变量类型的默认值。

当同时使用多种途径提供属性值时，将按上述顺序作为优先级进行调用。

### 特殊的着色器属性值

一些着色器属性在传入时还会附带一些额外数据，可通过定义符合这些额外属性命名和类型规范的变量来接收。

- `float4 <textureVarName>_ST`：纹理属性额外的平铺和偏移信息。内容如下`(x平铺值,y平铺值,x偏移值,y偏移值)`。
- `float4 <textureVarName>_TexelSize`：纹理属性额外的大小信息，内容如下`(1.0/宽度,1.0/高度,宽度,高度)`。
- `float4 <textureVarName>_HDR`：纹理属性额外的 HDR 参数信息，指明如何在当前颜色空间下解码可能的 HDR 纹理，详见 UnityCG.cginc 中 DecodeHDR 函数。

### 颜色空间下的数据转换

通常情况下纹理颜色等信息都是基于 sRGB 空间存储的，其会更多的存储暗色部分的颜色，这也更符合人眼的认知特征。但现实的物理计算都是基于线性空间的，因此为了更真实的渲染效果，通常都需要进行转换。

幸运的是，当 Unity 的渲染设置为线性空间时，Unity 会自动将 sRGB 空间的输入转换为线性空间，确保在着色器程序中直接拿到手的就是线性值。这主要是针对以下类型起作用：

- 纹理属性
- 颜色属性

其他属性如浮点数、向量则不会转换，除非是显式的在属性块中添加了`[Gamma]`特性标签。

## 内置着色器成员

Unity 提供一些必要和常用的内置成员以便获取着色器环境、渲染数据等。

但要注意的是，不同渲染管线中提供的内置成员并不完全兼容，实际使用时还要具体看文档。

### 内置头文件

https://docs.unity.cn/cn/2022.3/Manual/SL-BuiltinIncludes.html

所有内置成员都是通过头文件引入的，不同的功能会用到不同的头文件，如果是使用`GCPROGRAM`代码块，Unity 还会自动引入一些常用头文件。

不过可惜这仅是内置渲染管线所用的头文件，如果是其他 SRP，则需要选择引入不同的头文件。

### 内置宏

https://docs.unity.cn/cn/2022.3/Manual/SL-BuiltinMacros.html

Unity 内置了一些平台宏或宏函数，以在编译阶段帮助处理平台差异。

- 当前编译的平台
- 当前编译的着色器模型
- 当前编译的 Unity 版本
- 当前编译的着色器阶段
- 当前平台的支持功能
- 当前平台的阴影贴图处理
- 当前平台的常量缓冲区处理
- 当前平台的纹理采样器处理
- 当前平台的深度纹理处理
- 禁用自动升级着色器代码
- 当前编译的表面着色器的通道

### 内置函数

https://docs.unity.cn/cn/2022.3/Manual/SL-BuiltinMacros.html

一些常用的函数，实现空间变换、光照阴影等功能，可惜内置渲染管线和 SRP 中所用的函数并不相通。

### 内置变量

https://docs.unity.cn/cn/2022.3/Manual/SL-UnityShaderVariables.html

内置变量是外部传入的渲染计算所需的参数，通常应通过内置函数访问而不是直接使用。

- 空间变换矩阵
- 相机与屏幕信息
- 时间
- 光照（光源信息、光照贴图、雾效和环境光）
- 其他（LODGroup 等级、UI 纹理格式）

## 数据类型

### 浮点数

- `float`：32 位高精度浮点数。
- `half`：16 位半精度浮点数。
- `fixed`：11 位低精度浮点数。过时类型，仅 OpenGL ES 2.0 支持。

在高端平台（如 PC）的 GPU 中始终使用高精度，仅当面向移动 GPU 时精度选择才会起效。

更低精度的浮点数通常性能更高，虽然随着硬件发展，性能优势可能逐步减弱，但依然会更加节省功耗。所以只要精度够用应优先使用低精度，然后按需提高。

#### 浮点数后缀

HLSL 原生语法中支持浮点数后缀，但**Unity 不支持**，所有带后缀的浮点数都将被识别为 float 类型，这可能导致计算精度提高而**造成负面的性能影响**。

#### 特殊浮点数

对于如无穷大、非数值等特殊浮点数，不同 GPU 间处理方式可能不同。

所有支持 Direct3D10 的电脑 GPU 都支持 IEEE754 浮点标准，所以处理浮点数的行为和常规 CPU 编程语言一致。

移动 GPU 的支持级别则可能不同，所以**必须确保在目标设备上测试着色器以检查支持情况**。

### 整数

- `int`

在一些老式平台上（如 OpenGL ES 2.0）不支持，甚至需要用复杂的浮点运算去模拟一些位、逻辑运算。

### 矢量

```hlsl
<type><number>
```

- type：任意整数或浮点数类型。
- number：任意整数。

如 `float2`、`int3`。

### 矩阵

```hlsl
<type><number>x<number>
```

- type：任意整数或浮点数类型。
- number：任意整数。

如 `int2x3`、`float4x4`。

在一些老式平台（如 OpenGL ES 2.0）仅支持方形矩阵。

### 纹理

```hlsl
sampler<type>[_<precision>]
```

- type：任意纹理类型

  - 2D：常用的平面纹理。
  - 3D
  - CUBE

- precision：任意精度类型

  - half
  - float

如 `sampler2D`、`samplerCUBE_float`。

#### 精度后缀

纹理支持可选的后缀来描述精度，但实际上如果不写明后缀，最终也会由 Unity 根据“着色器精度”设置自动添加后缀。

在移动平台，纹理精度后缀默认为 `half`。

#### 采样器状态

https://docs.unity.cn/cn/2022.3/Manual/SL-SamplerStates.html

在许多较新的 API 中，采样器和纹理可以分开定义，并支持在着色器中直接声明采样模式。
