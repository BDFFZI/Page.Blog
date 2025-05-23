---
abbrlink: 330516235
date: 2024-3-14 17:09
categories:
  - 个人研究
  - 技术美术
---

# 【技术美术】GPU 渲染管线笔记

## 基本术语

- 基元、图面：

  网格中所使用的顶点数据布局，常见的如点、线、三角面等，特殊的甚至包括一些带邻近基元的基元类型。

## 参数语义

语义是附加到着色器输入或输出参数的字符串，用于传达给系统有关参数的预期用途的信息。

### 用户语义

由用户在 IA 阶段定义的语义。用于描述传入的数据类型，以便系统在 VS 阶段分配输入参数；同时也用于在其他着色器中标记要在各阶段传递的数据，虽然此时数据类型已无作用，但语法上还是需要的。

### 系统语义

系统语义则是系统自带的语义，这些语义为了满足一些功能而存在，如向系统表明着色器输出参数，请求系统自动设置一些特殊值等，其都使用`SV_`作为前缀。

https://learn.microsoft.com/zh-cn/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics#system-value-semantics

## 数据类型

在各着色器间都存在输入输出参数，其中一些参数由系统语义明确定义，此外还可以由用户语义自定义一些额外参数。这些数据间通常还存在着共用的传递关系，根据使用情况可以分为以下几类。

### 装配数据

顶点着色器的输入数据，大多由用户语义定义，是来自 IA 阶段的原始网格数据。

- SV_VertexID：当前顶点在原始网格数据中的排序序号。
- SV_InstanceID：当使用实例化渲染功能时额外传递的实例序号。

### 顶点数据

经过顶点着色器处理后的，系统所用的顶点数据，多个着色器阶段都会用到该数据。

- SV_Position：基元光栅化时所用的最终顶点位置。

### 外壳数据

在分割阶段中使用的数据，由外壳着色器创建。

- SV_TessFactor：基元的边缘分割次数。
- SV_InsideTessFactor：基元的内部分割次数。

#### 备注

- 如果外壳着色器阶段将任何边缘细化因子设置为 = 0 或 NaN，则将剔除补丁，从而不生成可见输出。

## 渲染流程

https://learn.microsoft.com/zh-cn/windows/win32/direct3d11/overviews-direct3d-11-graphics-pipeline

### 1. 输入装配器阶段（IA）

从用户填充的缓冲区中读取基元数据 (点、线和/或三角形) ，并将数据组装成将由其他管道阶段使用的基元。

1.  创建输入缓冲区。
2.  创建输入布局对象。
3.  将对象绑定到输入。
4.  指定基元类型。
5.  调用绘制方法。

### 2. 顶点着色器阶段（VS）

处理输入装配器提供的顶点，执行每个顶点运算，例如空间变换、外观变形和每顶点照明。

- 输入：装配数据
- 输出：顶点数据

### 3. 分割阶段

通过在硬件中实施分割，使图形管道可解算更低画质（多边形数量更少）模型但并以更高画质进行渲染。

- 域：

  用于分隔的多边形。

- 补丁：

  域被分割后产生的新多边形。

- 控制点：

  本质是一个顶点，其决定了域的形状，如三角形域就是有三个控制点。在分割补丁时会将其作为补丁顶点的参考点，通过权重混合多个控制点来得出。

- 修补程序：

  负责将原始域分割为补丁的程序。

#### 3.1. 外壳着色器阶段（HS）

https://learn.microsoft.com/zh-cn/windows/win32/direct3d11/direct3d-11-advanced-stages-hull-shader-design

该阶段由两个函数构成。

##### 外壳着色器

计算构成补丁的控制点的顶点数据。

- 输入：原始基元控制点的顶点数据、SV_PrimitiveID、SV_OutputControlPointID
- 输出：补丁的其中一个控制点的顶点数据

##### 修补程序常量函数

为细分器阶段和域着色器阶段提供计算参数。

- 输入：原始基元控制点的顶点数据、SV_PrimitiveID
- 输出：外壳数据

#### 3.2. 细分器阶段（Tessellator）

这是一个固定函数阶段，利用从外壳着色器阶段传入的细化因素和分区类型，将域（四边形、三角形或线）分割为很多较小对象（三角形、点或线）。

#### 3.3. 域着色器阶段（DS）

https://learn.microsoft.com/zh-cn/windows/win32/direct3d11/direct3d-11-advanced-stages-domain-shader-design

计算补丁程序生成的新基元的顶点数据。

- 输入：外壳数据、补丁控制点的顶点数据、SV_DomainLocation
- 输出：补丁的其中一个顶点的顶点数据

### 4. 几何着色器阶段（GS）

https://learn.microsoft.com/zh-cn/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-stream-stage-getting-started

将顶点作为输入，并能够在输出中生成新顶点，从而实时创造新的网格。

- 输入：原始的顶点数据序列
- 输出：新增的顶点数据序列

### 5. 光栅化阶段（RS）

将网格基元映射为屏幕上的“像素”（实际是由 2x2 像素区域构成的片元），并计算对应的顶点数据插值，以便为像素着色器提供输入数据。

### 6. 像素着色器阶段（PS）

对像素实现丰富的着色技术，例如每像素照明和后期处理。

- 输入：顶点数据
- 输出：SV_Target、SV_Depth

### 7. 输出合并阶段（OM）

将像素着色器生成的像素数据按原始三角形的顺序进行进一步处理。

1. 裁剪测试：

   剔除指定区域（通常是视口）之外的像素。

2. alpha 测试：

   剔除透明度低于指定值的像素。（Direct3D 10 及以上版本不支持，但可通过`clip()`实现等效操作）

3. 模板测试
4. 深度测试

   深度测试分为早期和晚期两种：在晚期执行是传统方案，也即目前的流程；早期则是提前到像素着色器之前，从而免去晚期中的很多无效运算。现代 GPU 大多支持早期深度测试，前提是没有在像素着色器中写入深度或使用 AlphaTest、Blend 等功能，否则会退化为晚期测试。

   Unity 中深度测试默认是早期，根据官方的渲染流程图中可以看出。

5. 混合

## 参考资料

- [Direct3D11 图形管道](https://learn.microsoft.com/zh-cn/windows/win32/direct3d11/overviews-direct3d-11-graphics-pipeline)
- [深度测试/模版测试/透明度测试先后顺序是什么样的？](https://www.zhihu.com/question/384124671)
- [Unity Shader-渲染队列，ZTest，ZWrite，Early-Z](https://gwb.tencent.com/community/detail/114229)
