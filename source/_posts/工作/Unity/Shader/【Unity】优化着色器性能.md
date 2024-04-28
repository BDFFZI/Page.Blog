---
abbrlink: 4183025058
date: "2024-4-28 16:58"
categories:
  - 工作
  - Unity
  - Shader
---
# 【Unity】优化着色器性能

https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderPerformance.html

## 仅执行所需的运算

- 去除无效计算：

  例如为材质增加了基础色功能，但所有材质都只用默认的白色，那这种功能就是无效功能，可去除或借助变体功能拆分成多个着色器。

- 减少计算频率：

  尽可能将计算放在低频率的过程中，例如将像素着色器中的计算移到顶点着色器，甚至完全移出着色器在外部计算再设置回着色器。

## 降低计算精度

由于改进的 GPU 寄存器分配或针对低精度数学的“快速路径”执行单元，较低的数字精度通常有更好的运算性能。哪怕没有原始性能优势，低精度运算通常也能起到降低功耗延长电池续航的效果。

因此只要精度够用，应尽可能降低数字精度。

- `float`：世界位置、纹理坐标
- `half`：矢量、HDR 颜色
- `fixed`：普通纹理颜色（这只对非常老的设备有效，目前更推荐 half）

对于精度的测试必须要在目标设备上进行，因为高端 GPU 通常会将默认精度以下的精度做和默认精度一样的处理（如统一看作`float`），所以只有在低端设备上才能感受到精度降低带来的优化或显示异常。

## 减少复杂运算

- 减少超越函数

超越函数（如`pow`,`exp`,`log`,`cos`,`sin`,`tan`等），会占用大量的 GPU 资源，避免使用。如果可以建议改为在外部预计算然后通过缓冲区或纹理传入。

- 减少自建函数

如果所需操作可用内置函数实现，就不要自行实现，内置函数有特别的编译优化，会比自己写的好得多。

## Alpha 测试

Alpha 测试或等效函数`clip()`在不同平台有不同的性能特征。

- 一般情况下使用该功能移除大部分平台上的透明像素时都可获得少量性能优势。
- 但在 ios 或某些 Android 设备的 PowerVR GPU 上，该功能是资源密集型任务，反而会对性能产生负面影响。

## 颜色遮罩

在某些平台（主要是 iOS 和 Android 设备的移动端 GPU）上，`Color Mask`是资源密集型的操作，除非绝对需要，否则不要使用。

## 优化表面着色器

针对表面着色器也支持一些特定的功能优化，详细见官方文档。

## 调试着色器

在使用 DirectX 11 或 12 的 Windows 平台上，可以使用 Visual Studio 调试 Unity 应用程序中的着色器，以便分析着色器问题或性能优化点。

https://docs.unity.cn/cn/current/Manual/SL-DebuggingD3D11ShadersWithVS.html

如果使用的是 DirectX 12，Microsoft 建议使用 PIX 而不是 Visual Studio 来调试着色器。

https://docs.unity.cn/cn/current/Manual/DebuggingShadersWithPIX.html
