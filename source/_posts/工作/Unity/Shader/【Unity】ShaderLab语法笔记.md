---
abbrlink: 1843614380
date: 2024-2-18 17:04
categories:
  - 工作
  - Unity
  - Shader
---

# 【Unity】ShaderLab 语法笔记

在 Unity 中使用 ShaderLab 来编写着色器文件。

ShaderLab 是一种声明性语言，和单纯的 HLSL、GLSL 等着色器语言不同，它的功能更宽泛，像是对整个图形接口功能的封装。

它可以实现指定渲染管线状态、支持一些 Unity 独有的渲染功能、提供利用传统着色器语言实现渲染逻辑等。

```shaderlab
// Shaderlab的基本语法结构
Shader "<shaderName>"{
    [Properties]
    <SubShader>
    [Fallback]
    [CustomEditor]
}
```

## Properties

Properties 用于描述需要显示在面板上的参数，并且这些参数还将被持久化保存。

```shaderlab
Properties {
    // 任意数量的参数声明
    [attributes] <property>
    ...
}
```

### 支持的 property

```shaderlab
// 滑动条和数字
<name> ("<displayname>", Range (<min>, <max>)) = <number>
<name> ("<displayname>", Float) = <number>
<name> ("<displayname>", Int) = <number>
// 颜色和矢量
<name> ("<displayname>", Color) = (<number>,<number>,<number>,<number>)
<name> ("<displayname>", Vector) = (<number>,<number>,<number>,<number>)
// 纹理
<name> ("<displayname>", 2D) = "<defaulttexture>" {}
<name> ("<displayname>", Cube) = "<defaulttexture>" {}
<name> ("<displayname>", 3D) = "<defaulttexture>" {}
```

- name - 自定义的属性名
- displayname - 自定义的显示在面板上的名称
- min、max、number - 表示任意数字
- defaulttexture - 默认为空字符串，但对于 2D 纹理可选如下默认纹理

  - white：（RGBA：1,1,1,1）
  - black：（RGBA：0,0,0,0）
  - gray：（RGBA：0.5,0.5,0.5,0.5）
  - bump：（RGBA：0.5,0.5,1,0.5）
  - red：（RGBA：1,0,0,0）

### 支持的 attributes

- [MainTexture] - 表示一个属性 (property) 是材质的主纹理，否则默认纹理为\_MainTex，多次使用选首个。
- [MainColor] - 表示一个属性 (property) 是材质的主色，否则默认主色为\_Color，多次使用选首个。
- [HideInInspector] - 不在材质检视面板中显示属性值。
- [NoScaleOffset] - 对于具有此特性的纹理属性，材质检视面板不会显示纹理平铺/偏移字段。
- [Normal] - 表示纹理属性需要法线贴图。
- [HDR] - 表示纹理属性需要高动态范围 (HDR) 纹理。
- [Gamma] - 表示在 UI 中将浮点/矢量属性指定为 sRGB 值（就像颜色一样），并且可能需要根据使用的颜色空间进行转换。
- [PerRendererData] - 表示纹理属性将以 MaterialPropertyBlock 的形式来自每渲染器数据。材质检视面板会更改这些属性的纹理字段 UI。

### 其他类型属性

其他类型属性（如矩阵、数组）默认不支持序列化，所以 property 并不支持，但这不代表无法读写。其他属性也可以写在 Pass 中，但只能运行时访问且只能通过代码访问（如 Material.SetFloat），具体见 Pass 部分笔记。

## SubShader

https://docs.unity.cn/cn/2019.4/Manual/SL-SubShader.html

由于 SubShader 的写法过于复杂，具体参见单独的 SubShader 语法笔记。

## Fallback

用于指明当所有子着色器都不支持时尝试使用的备用着色器，相当于在末尾插入目标着色器的所有子着色器。

```shaderlab
// 回退到其他着色器
Fallback "shaderName"
// 不回退且不发出警告
Fallback Off
```

## CustomEditor

<https://docs.unity.cn/cn/2019.4/Manual/SL-CustomShaderGUI.html>

用于实现自定义的属性面板，需要编写配套的 C#类。

```shaderlab
// 使用指定的ShaderGUI类作为GUI
CustomEditor "className"
```

## 其他命令

<https://docs.unity.cn/cn/2019.4/Manual/SL-Other.html>

### Category

Category 是一个可选功能，它能将 SubShader 分组，为其批量设置参数。这个功能仅影响着色器解析，效果完全等同于将参数设置粘贴到每个 SubShader 下。
