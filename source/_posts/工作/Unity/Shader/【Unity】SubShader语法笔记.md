---
abbrlink: 2092124193
date: "2024-2-24 13:42"
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

## LOD

LOD 用于表示子着色器在计算方面的需求大小，配合 Shader.maximumLOD 可以排除高于指定 LOD 的子着色器，从而实现根据不同硬件情况运行不同的着色器效果。

```subShader
// LOD的语法示例
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

#### ForceNoShadowCasting

使用该标签且值为“True”，可使该 SubShader 绝不会投射阴影。对透明对象使用 Shader 替换但不希望继承阴影通道时非常有用。

#### DisableBatching

使用批处理渲染时，相关物体的顶点数据将全部转换到世界空间，导致依赖物体空间的 Shader 失效。

如果遇到这种情况可利用 DisableBatching 禁止对该 Shader 使用批处理。

支持以下值：

- True：始终禁用批处理
- False：不禁用批处理
- LODFading：当 LOD 淡化处于激活状态时禁用批处理；主要用于树

#### IgnoreProjector

使用该标签且值为“True”，可使投影器功能忽略该对象，这对半透明物体很有用。

#### PreviewType

指定材质面板的预览效果，默认为球体，也可以改为“Plane”（显示为 2D），“Skybox”（显示为天空盒）。

#### CanUseSpriteAtlas

使用该标签且值为“True”，可警告用户该 Shader 依赖原始纹理坐标，所以不能打包到图集中。

### 通道 Tags

可以在 SubShader 中编写 Pass 中专用的 Tags，这将自动把相关 Tags 应用到所有 Pass。

### 自定义 Tags

利用 Material.GetTag 可以读取 Unity 内置标签之外的标签，这对自定义渲染管线时非常有用，实际上 Unity 内置渲染管线的运作方式应该也是一样的原理。

## CommonState

CommonState 实际上是对应 Pass 中的 RenderSetup 部分，在 SubShader 中提供的 RenderSetup 将自动应用到所有 Pass 中。

## Pass

用于编写实际渲染用的着色器代码。

- 一次 Pass 的运行代表一次渲染，可定义多个。
- 如果有多个 Pass 则按编写的顺序执行。

### 自定义 Pass

https://docs.unity.cn/cn/2019.4/Manual/SL-Pass.html

见 Pass 语法笔记。

### UsePass

借助 UsePass，可不自行实现而可以直接使用现有的 Pass。

- 系统只会考虑目标第一个受支持的子着色器。
- 目标 Pass 需要带有 Name 命令。

```shaderlab
UsePass "<shaderName/passName>"
```

### GrabPass

利用此 Pass 可将屏幕内容抓取到纹理中用于在后续通道中使用。

GrabPass 只是一种特殊的 Pass，所以也可以使用 Pass 的 Name 和 Tags 命令。

```shaderlab
GrabPass { } // 直接抓取到名为_GrabTexture的纹理中，注意：这种抓取方式将为每个使用它的对象单独抓取一次。
GrabPass { "<TextureName>" } // 抓取到指定纹理中，一帧只执行一次，后续通道共用纹理结果。
```
