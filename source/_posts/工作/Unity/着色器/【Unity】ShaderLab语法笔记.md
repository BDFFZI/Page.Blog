---
abbrlink: 1843614380
date: 2024-2-26 16:45
categories:
  - 工作
  - Unity
  - Shader
---

# 【Unity】ShaderLab 语法笔记

https://docs.unity.cn/cn/2022.3/Manual/SL-Reference.html

```shaderlab
Shader "<shaderName>"
{
    [Properties]

    [Category] // 可以实现批量设置SubShader参数
    {
        SubShader
        {
            [PackageRequirements]
            [LOD]
            [Tags]
            [Commands]

            Pass{
                [PackageRequirements]
                [Name] "<passName>" // 配合UsePass等功能使用
                [Tags]
                [Commands]
                <PassCode>
            }
            ...// 一次Pass渲染一次，可填写多个，按顺序多次渲染。

            [UsePass] "<shaderName/passName>" // 直接使用现成的Pass，等于将另一个Pass的代码复制进来。
            [GrabPass] { {} | "textureName" } // 纹理抓取Pass，仅内置管线可用，影响性能不建议使用。
        }
        ...// 可填多个SubShader，但只会使用首个有效的，用于根据设备环境调整显示效果。
    }

    [Fallback] {"<shaderName>" | Off} // 所有SubShader无效时的回调。

    [CustomEditor] "<className>" // 自定义面板界面，需继承ShaderGUI的类。
}
```

- 如果 shaderName 以`Hidden/`为前缀则将在 Shader 菜单中隐藏。

## PackageRequirements

https://docs.unity.cn/cn/2022.3/Manual/SL-PackageRequirements.html

描述该着色器依赖的 Unity 包环境，这对那些需要做多管线支持的着色器非常有用，可以避免因管线兼容性导致的编译错误。

## LOD

指明当前 SubShader 对计算方面的需求，可配合 Shader.maximumLOD 排除高于指定 LOD 的 SubShader，从而实现根据不同硬件性能使用不同的显示效果。

```shaderlab
LOD <number>
```

### 内置 Shader 的 LOD

- Unlit 系列：100
- Standard：300

## Propertie

用于定义显示在面板的参数，同时这些参数会被序列化保存。

```shaderlab
Properties
{
    [attribute]... <name> ("<displayname>",<type>) = <defaultValue>
    ...
}

```

### 支持的 type 及其 defaultValue

| type                 | defualtValue                              | 备注                     |
| -------------------- | ----------------------------------------- | ------------------------ |
| Integer              | \<number>                                 |
| Int                  | \<number>                                 | 旧版整数，实际是浮点实现 |
| Float                | \<number>                                 |
| Range(\<min>,\<max>) | \<number>                                 | 显示为滑动条的浮点数     |
| Color                | (\<number>,\<number>,\<number>,\<number>) |
| Vector               | (\<number>,\<number>,\<number>,\<number>) |
| 2D                   | "\<defaulttexture>"                       |
| 2DArray              | ""                                        |
| 3D                   | ""                                        |
| Cube                 | ""                                        |
| CubeArray            | ""                                        |

- **number、min、max**

  任意数值

- **defaulttexture**

  内置纹理

  - 留空：默认为 gray
  - white：（RGBA：1,1,1,1）
  - black：（RGBA：0,0,0,0）
  - gray：（RGBA：0.5,0.5,0.5,0.5）
  - bump：（RGBA：0.5,0.5,1,0.5）
  - red：（RGBA：1,0,0,0）

### 支持的 attribute

- [Gamma] - 指示浮点数或矢量属性使用 sRGB 值，这意味着如果项目中的颜色空间需要，则它必须与其他 sRGB 值一起转换。
- [HDR] - 指示纹理或颜色属性使用高动态范围 (HDR) 值。
- [HideInInspector] - 指示检视面板中隐藏该属性值。
- [MainTexture] - 表示该属性为材质主纹理，否则默认纹理为\_MainTex，多次使用选首个。
- [MainColor] - 表示该属性为材质主色，否则默认主色为\_Color，多次使用选首个。
- [NoScaleOffset] - 指示检视面板隐藏此纹理属性的平铺和偏移字段。
- [Normal] - 指示该纹理属性需要法线贴图。
- [PerRendererData] - 指示纹理属性将来自每个渲染器数据，形式为 MaterialPropertyBlock。

#### 自定义 attribute

https://docs.unity.cn/cn/2022.3/ScriptReference/MaterialPropertyDrawer.html

允许通过 MaterialPropertyDrawer 创建自定义 attribute，可实现修改属性的显示效果或其他特殊功能。

内置的 MaterialPropertyDrawer 实现有以下几种：

- [Toggle]：将浮点数属性显示为开关，打开时会自动启用材质中对应的着色器关键字，需配合 shader_feature 使用。默认关键字为“{propertieName}\_ON”全大写，也可用括号参数自定义关键字。
- [ToggleOff]：类似 Toggle 的取反版本，默认关键字为“{propertieName}\_OFF”全大写。
- [KeywordEnum(enum1,enum2...)]：类似 Toggle 的多选项版本，将浮点属性显示为弹出菜单，浮点的值将决定启动的关键字，关键字为“{propertieName}\_{enumName}”全大写，最多 9 个。
- [Enum(enum1,number1,enum2,number2...)]：将浮点属性显示为弹出菜单，按枚举值给浮点设置数值。
- [PowerSlider(number)]：将 Range 属性的滑动条改为非线性滑动条。
- [IntRange]：将 Range 属性的滑动条改为整数滑动条。

## Tags

```shaderlab
Tags
{
    "<key>" = "<value>"
    ...
}
```

### SubShader 专用

- **RenderPipeline**

  告知 Unity 该 Shader 是否支持 Unity 预定义的几个渲染管线。

  - UniversalRenderPipeline：支持 URP
  - HighDefinitionRenderPipeline：支持 HDRP
  - 其他值或未声明：不兼容相关管线

- **Queue**

  定义渲染顺序，数字越大越靠后，可用加减运算进一步调整。

  - Background：1000
  - Geometry：2000，不透明物体
  - AlphaTest：2450
  - Transparent：3000，透明物体
  - Overlay：3000

- **RenderType**

  https://docs.unity.cn/cn/2019.4/Manual/SL-ShaderReplacement.html

  给 SubShader 分类，用于实现着色器替换功能，仅内置渲染管线支持。

- **ForceNoShadowCasting**

  禁用 Shader 投射或接收阴影，具体不同管线效果不一样，一般用于配合着色器替换功能使用。

  - True
  - False

- **DisableBatching**

  禁用动态批处理功能。因为动态批处理会将所有几何体转为世界空间，这对需要物体空间的 Shader 将产生负面影响，所以要关闭。

  - True
  - False
  - LODFading：当几何体使用 LODGroup 功能且 Fade Mode 不为 None 时生效。

- **IgnoreProjector**

  使几何体不受内置渲染管线的投影器功能的影响，在其他渲染管线中该标签无效。

  - True
  - False

- **PreviewType**

  指明材质在 Inspector 面板中的预览效果。

  - Sphere：默认值
  - Plane
  - Skybox

- **CanUseSpriteAtlas**

  在 SpritePacker（已弃用） 功能中，实现警告用户着色器依赖于原始纹理坐标，因此不应将其纹理打包到图集中。

  - True：默认值，兼容图集
  - Flase：不兼容图集

### Pass 专用

也可在 SubShader 中填写 Pass 的 Tags，这会自动应用到所有 Pass。

- **LightMode**

  不同的渲染任务（如渲染深度图，向前或延迟渲染）一般需要使用一些特定 Pass，所以要用该标签声明这些特殊 Pass ，这样 Unity 渲染时就知道此时要调用 Shader 中的哪些 Pass。

  该标签与多 Pass 功能存在关联，使用标签通常会导致无法使用多 Pass 功能，反之如果不提供该标签则默认为使用多 Pass 功能，按序多次渲染，具体还是要看渲染管线的实现方式。

  具体标签值要根据渲染管线填写。

  [渲染管线 (URP) 中的 LightMode 通道标签](https://docs.unity.cn/Packages/com.unity.render-pipelines.universal@11.0/manual/urp-shaders/urp-shaderlab-pass-tags.html#urp-pass-tags-lightmode)

### 自定义 Tags

可自由填写键值对，然后用 Material.GetTag 读取。

## Commands

- 在 SubShader 中填写 Commands 会自动应用到所有 Pass。
- 命令的参数实际是数字，故也可用 Propertie 表示值，如 `ZTest [<propertieName>]`，从而实现程序化控制。

### 颜色输出相关

#### Blend

确定 GPU 如何将片元着色器的输出与渲染目标进行合并。

默认混合公式：$finalValue = sourceFactor * sourceValue + destinationFactor * destinationValue$

```shaderlab
//默认值，禁用混合。
Blend [<renderTarget>] Off
// 设置当前输出值和缓冲区值的各自系数
Blend [<renderTarget>] <sourceFactor> <destinationFactor>
//将RGB与Alpha的系数分开设置
Blend [<renderTarget>] <sourceRGBFactor> <destinationRGBFactor>, <sourceAlphaFactor> <destinationAlphaFactor>
```

- **factor**（`UnityEngine.Rendering.BlendMode`）

  混合系数。

  - One（1）
  - Zero（0）
  - SrcColor（3）
  - SrcAlpha（5）
  - DstColor（2）
  - DstAlpha（7）
  - OneMinusSrcColor（6）
  - OneMinusSrcAlpha（10）
  - OneMinusDstColor（4）
  - OneMinusDstAlpha（8）

- **renderTarget**

  渲染目标索引，0-7 的整数。

#### BlendOp

https://docs.unity.cn/cn/current/Manual/SL-BlendOp.html

指定 Blend 命令使用的混合操作。使用该命令时必须同时使用 Blend 命令。并非所有设备都支持所有混合操作。

```shaderlab
BlendOp <operation>
```

- **operation**（`UnityEngine.Rendering.BlendOp`）

  - Add（0）：默认值，将源和目标相加。
  - ...

#### ColorMask

颜色通道写入遮罩，可用于禁用部分通道的写入。

```shaderlab
ColorMask <channels> [<renderTarget>]
```

- **channels**（`UnityEngine.Rendering.ColorWriteMask`）

  可写入的通道，其中 RGBA 4 项可任意组合使用，如默认值为 RGBA。

  - 0（0）：全通道不可写入
  - R（8）：仅 R 通道
  - G（4）
  - B（2）
  - A（1）

- **renderTarget**

  渲染目标索引，0-7 的整数。

### 模板深度相关

#### Offset

设置深度偏移，负数将减小深度，使离摄像机更近。

偏移公式：$offset = (m * factor) + (r * units)$

- m：多边形相对摄像机 z 轴的斜率，如果正视摄像机即与近平面平行则为 0。
- r：其是使深度差异可分辩的最小值，这是由渲染设备定义的一个常量。

```shaderlab
Offset <factor>, <units>
```

- factor：-1 到 1 的小数。
- units：-1 到 1 的小数。

#### Stencil

如何进行模板测试，测试通过后将进入深度测试，否则丢弃片元。

- 所有遮罩均是指哪些位可操作，并不是直接与值并运算，所以如写入遮罩为 0，表示所有位都无法写入，而不是写入 0。
- 部分参数支持 Back 和 Front 的后缀，从而实现对正反片元单独处理，但如果提供了无后缀版本，则后缀版本被覆盖。

测试公式：$\text{(ref \& readMask) comparisonFunction (stencilBufferValue \& readMask)}$

```shaderlab
Stencil
{
    [Ref <ref>] //表明引用值，0-255的整数，默认为0
    [ReadMask <readMask>] //缓冲区读取遮罩，0-255的整数，默认为255
    [WriteMask <writeMask>] //缓冲区写入遮罩，0-255的整数，默认为255
    [Comp[{Back | Fornt}] <comparisonOperationFront>] // 定义如何测试
    [<event> <operation>] // 定义测试结束后的处理事件
    ...
}
```

- **comparisonFunction**

  可用 C#中的 Rendering.CompareFunction 表示。

  | 值       | 对应数值 | 描述                           |
  | -------- | -------- | ------------------------------ |
  | Never    | 1        | 比较永远失败，即从不渲染像素。 |
  | Less     | 2        | 参考值小于缓冲区值时通过。     |
  | Equal    | 3        |                                |
  | LEqual   | 4        |                                |
  | Greater  | 5        |                                |
  | NotEqual | 6        |                                |
  | GEqual   | 7        |                                |
  | Always   | 8        | 默认值，比较始终成功。         |

- **event**

  可添加 Back 或 Front 后缀。

  - Pass：当通过模板测试和深度测试时
  - Fail：当未通过模板测试时
  - ZFail：当通过模板测试，但未通过深度测试时

- **operation**

  可用 C#中的 Rendering.Rendering.StencilOp 表示。

  | 值       | 对应数值 | 描述                                          |
  | -------- | -------- | --------------------------------------------- |
  | Keep     | 0        | 默认值，保持模板缓冲区中的内容。              |
  | Zero     | 1        | 将 0 写入模板缓冲区。                         |
  | Replace  | 2        | 将参考值写入模板缓冲区。                      |
  | Invert   | 3        | 将缓冲区中的值的所有位取反。                  |
  | IncrSat  | 4        | 递增缓冲区中的值，上限 255。                  |
  | DecrSat  | 5        | 递减缓冲区中的值，下限 0。                    |
  | IncrWrap | 6        | 递增缓冲区中的值，如果当前值为 255 则变为 0。 |
  | IncrWrap | 7        | 递减缓冲区中的值，如果当前值为 0 则变为 255。 |

#### ZClip

如何处理近平面和远平面的片元。

```shaderlab
ZClip <state>
```

- **state**
  - True：默认值，超出近平面或远平面的片元直接丢弃。
  - False：夹紧超出近平面或远平面的片元。

#### ZTest

如何进行深度测试，片元越近深度值越小，越远深度值越大。

```shaderlab
ZTest <operation>
```

- **operation**
  - Less
  - LEqual：默认值，当前深度值小于等于缓冲区的值时渲染，当前几何体位于现有几何体的前面或等距。
  - Equal
  - GEqual
  - NotEqual
  - Always：不进行深度测试，永远绘制。

#### ZWrite

是否写入深度值。

```shaderlab
ZTest <state>
```

- **state**
  - On：默认值，写入深度值。
  - Off

### 其他

#### AlphaToMask

https://docs.unity.cn/cn/current/Manual/SL-AlphaToMask.html

启用或禁用 GPU 上的 alpha-to-coverage 模式，可以减少将多样本抗锯齿 (MSAA) 与使用 Alpha 测试的着色器（如植被着色器）一起使用时出现的过度锯齿。

```shaderlab
AlphaToMask <state>
```

- **state**
  - On：启用 alpha-to-coverage 模式。
  - Off：禁用 alpha-to-coverage 模式。

#### Conservative

是否启用保守光栅化。正常光栅化在确定像素被三角面覆盖的同时还会判断覆盖范围是否足够，保守光栅化则只要被覆盖就光栅化。

```shaderlab
Conservative <state>
```

- **state**
  - True：启用保守光栅化。
  - False：默认值，禁用保守光栅化。

#### Cull

设置多边形的正反面剔除。

```shaderlab
Cull <state>
```

- **state**
  - Back：默认值，剔除背面。
  - Front：剔除前面。
  - Off：不剔除。

## PassCode

见 PassCode 语法笔记。
