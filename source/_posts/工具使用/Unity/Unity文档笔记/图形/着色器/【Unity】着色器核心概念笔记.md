---
abbrlink: 2077063050
date: 2024-4-26 11:25
categories:
  - 工具使用
  - Unity
  - Unity文档笔记
  - 图形
  - 着色器
---

# 【Unity】着色器核心概念笔记

https://docs.unity.cn/2022.3/Documentation/Manual/ShadersOverview.html

## 基本概念

着色器就是 GPU 上运行的程序。

### 着色器类型

Unity 支持三种着色器。

- 渲染管线着色器（主要着色器）
- 计算着色器
- 光线追踪着色器

### 着色器组成

- 着色器信息：如着色器名称、回退着色器等着色器自身的信息。
- 子着色器：子着色器用于将着色器对象分成多个部分，以分别兼容不同的硬件、渲染管线和运行时设置。
  - 兼容性说明：指明运行该子着色器所需的硬件、渲染管线和运行时设置，例如 LOD。
  - 子着色器标签：提供子着色器相关信息的键值对。
  - 通道：一个通道对应一次实际的 GPU 渲染。
    - 通道标签：提供通道相关信息的键值对。
    - 渲染状态说明：指明执行该通道依赖的渲染状态设置。
    - 着色器程序：实际的着色器代码，每一个着色器阶段代表一个着色器程序。
      - 着色器变体：一个着色器程序可能有一个或多个变体版本。

### 着色器模型

因为着色器也是慢慢发展过来的，所以也有新旧版本之分，而着色器模型其实就是着色器版本的花俏说法。

一些新模型支持的功能在旧模型中可能不支持，所以开发时要考虑好着色器模型的兼容性，默认情况下 Unity 使用着色器模型 2.5。

https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderCompileTargets.html

### 着色器平台

着色器平台一般指的就是 GPU 驱动。不同操作系统平台通常采用不同的 GPU 驱动，而不同 GPU 驱动间所需的着色器字节码是不一样的，所以每个平台都要单独编译着色器。

此外不同平台支持的功能、功能实现细节都可能有所区别，因此开发时必须要考虑好平台兼容性。

https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderCompilationAPIs.html

## 着色器编译

基本流程：

1. 对着色器源代码进行预处理，如生成代码，处理宏指令等。
2. 使用 Microsoft 的 FXC HLSL 编译器将代码编译为 HLSL 字节码。
3. 按需使用 HLSLcc 将 HLSL 字节码转换成其他平台的字节码。

### 编辑器编译策略

1. 检查当前需要的着色器变体是否存在缓存中。（存储位置：Library/ShaderCache）
2. 找到则直接显示，否则编译该变体并保存到缓存中。

如果启用了异步着色器编译则编译时会临时显示占位符着色器（显示为纯青色）。

### 构建时编译策略

1. 给每个所需的图形 API 编译所有的着色器变体，即使编辑器中并未使用。
2. 尝试检测游戏中尚未使用（需要显式声明）或重复的着色器变体，并将其从构建数据中剥离或合并。

## 着色器加载

### 着色器加载流程

#### 如何加载着色器？

1. 在构建的软件中，着色器变体被压缩成多个块存放在磁盘中。
2. 当加载场景或资源时，Unity 会加载并解压相关的所有变体块到内存中。
3. 当首次需要变体来渲染几何体时，Unity 会选取一个合适的变体并将其传递给图形驱动。
4. 图形驱动会创建该变体的 GPU 版本，并上传到 GPU。

#### 如何处理着色器内存？

1. Unity 提前将着色器加载到内存确保了不会在即将渲染时处理而导致的停顿。
2. 默认情况下仅在要渲染时才创建 GPU 版本，但这可能会导致稍微的停顿（可通过预热解决）。
3. Unity 将缓存 GPU 版本的变体，确保再次需要时不会造成滞后。
4. 当不再有任何对象引用该变体时，Unity 将会从 CPU 和 GPU 中都完全删除该变体。

#### 如何选取子着色器？

1. 在使用 Shader 对象前，Unity 会创建一个子着色器列表，并按顺序添加 Shader 中定义的所有子着色器及其回退着色器。
2. 当 Unity 首次使用 Shader 对象渲染几何体时，或者当着色器 LOD 值或活动渲染管道更改时，Unity 会决定活动子着色器。
3. Unity 遍历子着色器列表，并确定它们是否与满足硬件、渲染管线、LOD 的需求。
4. 如果包含一个或多个满足要求的子着色器，Unity 会选择第一个子着色器作为活动子着色器。
5. 如果不包含任何满足所有要求的子着色器：
   1. 尝试选择首个不满足 LOD 或渲染管线要求，但满足硬件要求的子着色器。
   2. 如果任何子着色器连硬件要求都无法满足，则按顺序尝试使用回退着色器。
   3. 若回退着色器也不兼容，则显示 Unity 内置的错误着色器。

#### 如何挑选合适的变体？

1. 仅加载与平台的图形 API、硬件和图形层兼容的已编译着色器。
2. 加载所需着色器变体。
3. 没有所需变体，则以一套规则加载类似的变体。（详见变体笔记）
4. 类似变体也没有，则显示错误着色器。

从 2022.2 开始播放器面板新增严格变体匹配设置，启用后可以阻止 Unity 选取类似变体，而直接显示错误着色器。

### 处理着色器加载

#### 预热着色器

Unity 默认在需要渲染时才创建变体的 GPU 版本，这在一些性能密集型的软件中可能导致卡顿，所以有时需要对变体的 GPU 版本进行提前加载，而这一过程就叫预热。

可以通过以下方式预热：

- [Experimental.Rendering.ShaderWarmup](https://docs.unity.cn/2022.3/Documentation/ScriptReference/Experimental.Rendering.ShaderWarmup.html)：预热 Shader 对象或着色器变体集合
- [ShaderVariantCollection.WarmUp](https://docs.unity.cn/2022.3/Documentation/ScriptReference/ShaderVariantCollection.WarmUp.html)：预热着色器变体集合
- [Shader.WarmupAllShaders](https://docs.unity.cn/2022.3/Documentation/ScriptReference/Shader.WarmupAllShaders.html)：预热内存中的所有着色器变体
- 将着色器变体集合添加到图形设置中的“预加载着色器”，Unity 将自动在启动时使用`ShaderVariantCollection.WarmUp`预热。

##### 部分特殊图形驱动的预热

DirectX 12、Metal、Vulkan 驱动程序需要在确切知道顶点数据布局和呈现状态的情况下才能创建变体的准确 GPU 表示，因此如果直接通过部分 API 预热可能会不准确而导致失败。

如果要在这些平台进行预热有如下方式：

- 故意在屏幕外渲染材质来主动触发变体的 GPU 版本创建。
- 调用`Experimental.Rendering.ShaderWarmup`并提供顶点数据布局且准确设置渲染状态。
- 其他预热方式均有可能失败。

#### 着色器加载探测

可以通过探测器窗口的一些事件标记查看着色器加载情况。

- `Shader.ParseThreaded`、`Shader.ParseMainThread`：表示用于从序列化数据加载着色器对象的事件。
- `Shader.CreateGPUProgram`：表示创建着色器变体 GPU 版本的事件。

#### 控制着色器内存

Unity 允许通过设置来控制加载块的大小以及加载进内存的数量，以控制在部分平台的内存占用：
https://docs.unity.cn/2022.3/Documentation/Manual/shader-loading.html#dynamicloading

## 着色器渲染

Unity 将使用相同着色器变体的几何体成批渲染，每帧一次，对于每批几何体：

1. 确定它们需要渲染活动子着色器中的哪些通道，以及帧中的哪些点。
2. 渲染需要的每一个通道：
   1. 如果当前渲染状态与通道定义不同，Unity 会根据通道定义重新设置渲染状态。
   2. 调用 GPU 使用相关的着色器变体渲染几何体。

## 着色器分支

分支用于实现同一个着色器但不同情况下执行一些差异化代码，从而显示不同的效果。

### 分支的种类

目前有以下几种方式实现分支需求。

#### 静态分支

着色器编译器在编译时评估条件代码。

- 优点：易于编写和维护，不会对生成时间、文件大小或运行时性能产生负面影响。
- 缺点：但不能在运行时切换执行的分支。

实现方式：

- 使用`#define`、`#if`等宏指令。
- 使用基于编译时常量做条件的`if()`语句。

#### 动态分支

GPU 在运行时评估条件代码。

- 优点：支持运行时切换分支且不会增加着色器变体数量（即节省文件内存大小）。
- 缺点：会产生一定的 GPU 性能损耗。

实现方式：

- 使用任何运行时状态的`if()`语句。
- 使用动态分支的着色器关键字。
- 使用计算着色器关键字。

底层原理与注意点：

动态分支会破坏程序的并行性，因此 GPU 必须选择执行不同操作，又或者选择同时执行两个分支然后丢弃一个的方式保持并行。这两种方式都会导致 GPU 性能损耗。

同时，无论什么时候，GPU 都必须为最坏的情况分配寄存器，所以 GPU 处理分支的开销基本是恒定的。如果一个分支比另一个分支复杂的多，那运行成本也将完全按最坏的情况来。

所以**确保分支的工作负载均衡**是非常重要的。此外 GPU 在处理**基于统一值的分支**（分支条件都使用同一个变量）上效率是比较高的，所以确保分支条件的简单也很重要。

#### 使用运算代替分支

对动态分支的一种改进，利用如 Step 之类的返回 0 或 1 的数学运算来变相执行条件代码。

- 优点：不产生分支所以不会破坏程序并行性。
- 缺点：可能导致产生难以维护的复杂代码。

根据具体情况，与动态分支相比，它可能只带来非常小的性能改进或根本没有优势，具体还是要在真机上进行测试。

#### 着色器变体

根据静态分支将着色器编译成多个变体版本，这不会对 GPU 性能产生影响。但过多的变体可能导致构建时间、文件大小、运行时内容占用和加载时间的增加。同时在手动预加载着色器功能中着色器变体也会引入额外的代码复杂性。

着色器变体是 Unity 的一个**核心功能**，是一种高性能的分支实现方案，具体见着色器变体笔记。

### 分支的选用

使用时应分析应用程序性能，根据实际情况进行决定。

例如，如果能接受一定的 GPU 性能损耗，那最好选择动态分支功能以降低多变体功能带来的风险；

反之，如果对 GPU 性能要求非常高且考虑了使用变体的成本，那可以选择使用变体功能。

## 其他着色器功能

### 替换着色器

利用该功能可以一次性匹配替换场景上的某些着色器，从而实现类似夜视之类的特效。但该功能仅内置渲染管线可用，其他管线需采用其他办法。  
https://docs.unity.cn/2022.3/Documentation/Manual/SL-ShaderReplacement.html

### 计算着色器

借助这一特殊的着色器可以使 GPU 不仅用于渲染，还能将其卓越的运算性能发挥在其他地方。  
https://docs.unity.cn/2022.3/Documentation/Manual/class-ComputeShader.html

### 退守着色器

Unity 的着色器采用退守机制（fallback），即确保永远都是可渲染的，而不是引起程序异常。为此内置了一些着色器用于实现该机制。

#### 错误着色器

当用户提供的着色器无法使用时，改为显示错误着色器。表现效果为洋红色。

注意当使用`BatchRendererGroup`API 时默认不会显示错误着色器，需要调用`BatchRendererGroup.SetErrorMaterial`手动设置。

#### 加载着色器

若启用了异步编译，当对象需要渲染但着色器尚未编译完成时，将使用加载着色器代替渲染。表现效果为青色。

#### 虚拟纹理错误材料

当项目使用流式虚拟纹理（SVT），则 Unity 会使用特殊材质来指示 SVT 设置中的问题。  
https://docs.unity.cn/2022.3/Documentation/Manual/svt-error-material.html

## 故障排除

### 重复编译着色器、绘制调用批处理被中断？

因为材质球实际没有被共用。通常以下情况会出现该问题：

- 资源包与非资源包场景引用了相同的着色器
- 多个资源包引用了未加入资源包的材质球

### 材质中使用了 shader_feature 关键字，但变体依旧被剥离？

判断 shader_feature 关键字是否被使用是通过检查当前一起构建的材质中是否使用来得知的。

若材质和着色器分别打包，则着色器无法得知当前使用的材质球，因而无法判断 shader_feature 使用情况。

所以解决方法有两种：

1. 改用`multi_compile`声明关键字，保证 Unity 永远编译相关变体。
2. 利用“变体集合”代替材质球与着色器一起打包，相当于显式收集相关变体。

### 着色器打包后丢失？

若该着色器是仅在代码中通过`Shader.Find()`等形式获取，且没有任何材质或其他物体引用，则可能被 Unity 误判为未被使用的资源而在打包时剥离。

所以解决方法有两种：

1. 改用物体引用的方式获取，让 Unity 能自动识别到该着色器。
2. 将该着色器放入到“Always Included Shaders”中，显式永远打包。
3. 将该着色器放入到 Resources 文件夹，改用 Resources.Load 加载着色器。
