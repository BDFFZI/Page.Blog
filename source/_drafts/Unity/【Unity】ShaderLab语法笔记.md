# 【Unity】ShaderLab语法笔记

## Shader是什么

正常我们写在CPU上的代码叫程序，而这些代码跑到GPU上摇身一变，名字就改叫Shader了。所以说Shader只是运行在GPU上的一段代码，只是最初这些代码都比较特殊，只能干计算像素颜色这一特定的任务，所以叫Shader（着色器）。

但Unity中的Shader和传统Shader还是有些不同的：它的功能更宽泛些，有点像对Direct3D整体功能的封装，而不仅是包含传统的Shader功能；它是一种结构化文件，使用声明性语言ShaderLab描述结构。

## [ShaderLab结构](https://docs.unity.cn/cn/2019.4/Manual/SL-Shader.html)

```shaderlab
Shader "<shaderName>"{
    // 持久化保存并显示在面板上的参数。
    [Properties]
    // 所用的着色器列表。
    <SubShader>
    //当所有SubShader都不支持时的回退方式
    [Fallback]
    //使用的自定义GUI
    [CustomEditor]
}
```

### Properties写法

```shaderlab
Properties {
    // 任意数量的参数声明
    [attributes] <property>
    ...
}
```

#### 支持的property

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
- defaulttexture - 默认为空字符串，但对于2D纹理可选如下默认纹理

  - white：（RGBA：1,1,1,1）
  - black：（RGBA：0,0,0,0）
  - gray：（RGBA：0.5,0.5,0.5,0.5）
  - bump：（RGBA：0.5,0.5,1,0.5）
  - red：（RGBA：1,0,0,0）

#### 支持的attributes

- [MainTexture] - 表示一个属性 (property) 是材质的主纹理，否则默认纹理为_MainTex，多次使用选首个。
- [MainColor] - 表示一个属性 (property) 是材质的主色，否则默认主色为_Color，多次使用选首个。
- [HideInInspector] - 不在材质检视面板中显示属性值。
- [NoScaleOffset] - 对于具有此特性的纹理属性，材质检视面板不会显示纹理平铺/偏移字段。
- [Normal] - 表示纹理属性需要法线贴图。
- [HDR] - 表示纹理属性需要高动态范围 (HDR) 纹理。
- [Gamma] - 表示在 UI 中将浮点/矢量属性指定为 sRGB 值（就像颜色一样），并且可能需要根据使用的颜色空间进行转换。
- [PerRendererData] - 表示纹理属性将以 MaterialPropertyBlock 的形式来自每渲染器数据。材质检视面板会更改这些属性的纹理字段 UI。

#### 其他类型属性

其他类型属性（如矩阵、数组）默认不支持序列化，所以property并不支持，但这不代表无法读写。其他属性也可以写在Pass中，但只能运行时访问且只能通过代码访问（如Material.SetFloat），具体见Pass部分笔记。

### Fallback写法

```shaderlab
// 回退到其他着色器
Fallback "shaderName"
// 不回退且不发出警告
Fallback Off
```

### CustomEditor写法

[自定义GUI需要编写配套的C#类。](https://docs.unity.cn/cn/2019.4/Manual/SL-CustomShaderGUI.html)

```shaderlab
// 使用指定的ShaderGUI类作为GUI
CustomEditor "className"
```

### Category

<https://docs.unity.cn/cn/2019.4/Manual/SL-Other.html>

Category是一个可选功能，它能将SubShader分组，为其批量设置参数。这个功能仅影响着色器解析，效果完全等同于将参数设置粘贴到每个SubShader下。

### SubShader写法

见后续的SubShader章节

## SubShader

- 一个ShaderLab结构中必须要有一个或多个SubShader，这里存放着实际渲染时使用的代码。
- 若编写了多个SubShader只会使用首个用户支持的SubShader，这主要用于根据不同设备状况实现渲染效果的回退。

### SubShader的结构

```shaderlab
// 可定义多个，然后根据用户设备的支持情况选择一个实际运行。
SubShader
{
    
    [Tags]
    //当前区域所有Pass的共用渲染设置
    [CommonState]

    // 实际渲染用的着色器代码，一次Pass的运行代表一次渲染，可定义多个。
    Pass
    {
        [Name]
        [Tags]
        [RenderSetup]
        <passcode>
    }
    ...

    UsePass "<passName>" //附加现有的Pass
}
...
```

### Pass的三种写法

- 固定函数着色器（过时）

    <https://docs.unity.cn/cn/2019.4/Manual/ShaderTut1.html>

    一个早期旧版着色器，使用内置的函数快速编写一些简单的着色器，不需要会HLSL，但扩展能力差。

- 表面着色器（过时）

    <https://docs.unity.cn/cn/2019.4/Manual/SL-SurfaceShaders.html>

    使用HLSL编写，但对光照等功能进行了封装，不需要自己实现，也因此编写比较简单同时具备一定扩展能力。但新出的SRP不支持该写法，看样子官方要用Shader Graph代替它。

- 顶点和片元着色器

    <https://docs.unity.cn/cn/2019.4/Manual/SL-ShaderPrograms.html>

    接近原生的HLSL体验，光照等功能都需要自行实现，难度较高，但通用性最强，一般考虑利用改源码的方式实现，从而简化制作流程。

## 着色器的组成

```
```

# 顶点着色器中和输入输出

## Input

名称|描述
---|---
POSITION|物体空间下的顶点位置。对于屏幕矩形（如使用Graphics.Blit功能），其生成的顶点位置是0-1的UV坐标

## Output

名称|描述
---|---
SV_POSITION|其POSITION对应在剪辑空间下的位置
