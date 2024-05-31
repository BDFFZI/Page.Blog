---
abbrlink: 305640944
date: 2024-5-21 16:48
categories:
  - 工作
  - Unity
  - Unity文档笔记
  - 脚本
---

# 【Unity】作业系统

https://docs.unity.cn/cn/2022.3/Manual/JobSystem.html

## 技术概述

作业系统是多线程技术的封装，使用户可以充分发挥 CPU 多核优势，大幅提高计算性能。

具体而言 Unity 封装了以下内容：

- 多线程：Unity 将根据 CPU 内核数量智能创建和分配线程，而无需用户管理。
- 偷窃工作：Unity 将会自动平衡各个工作线程之间的任务量，充分利用每个线程。
- 安全系统：Unity 提供各种功能保证多线程中数据读写安全，从而解决多线程竞争问题。
- 其他可选功能包：
  - Burst：高性能代码编译器，能大幅提高代码执行速度，即使不使用多线程技术。
  - Collections：针对作业系统使用场景，封装的一些支持更复杂操作的集合。
  - Entities：一种全新的 ESC 游戏框架。

## 作业

作业是一个继承`IJob`等同类接口的结构体，用于描述一段需要利用作业系统完成的工作，内容包括自定义的工作参数和执行代码。

作业需要在主线程中创建调度，通常一个完整功能可能由多个作业构成，因此 Unity 允许作业间互相依赖。

根据不同的使用场景，Unity 提供了以下作业类型：

- `IJob`：在一个作业线程上运行单个任务。
- `IJobParallelFor`：并行执行任务，通过每个任务的独占的索引来访问公共数据。
- `IJobParallelForTransform`：类似`IJobParallelFor`，但每个任务都有额外的独占转换层`Transform`可操作。
- `IJobFor`：类似`IJobParallelFor`，但允许以非并行的方式调度任务。

所有接口都需要实现`Execute`方法，这是每个作业将会执行的代码。而自定义作业参数则需要自行使用安全类型进行声明。

## 安全类型

### Blittable 类型

https://learn.microsoft.com/zh-cn/dotnet/framework/interop/blittable-and-non-blittable-types

考虑 Burst 编译器的使用，Unity 的作业系统只能使用原生数据类型进行计算，因此 C# 中的很多托管类型无法使用。所以在作业系统中只能使用非托管类型数据，或者更准确的说叫 Blittable 类型。

### 原生容器

由于托管容器无法在作业系统中使用，为此需要改用 Unity 特制容器类型，其是 C# 层对原生内存的包装器。

内置的原生容器有如下几个：

- NativeArray：一个非托管类型数组。
- NativeSlice：可借此从`NativeArray`中获取切片。

如果需要一些更复杂的原生容器，可参考容器包：  
https://docs.unity.cn/Packages/com.unity.collections@latest/

#### 读写权限

因为多线程的存在，原生容器的读写权限被严格控制。

默认情况下数据是同时可读写的，但这会导致 Unity 需要额外的处理来检查并行写入的问题。若显式声明只读，则可优化这部分功能。

所以**正确设置数据的读写权限可以减少一定的性能损耗，并解锁一些读写功能**，这主要由用户使用一些特性标签来进行控制。

- `[ReadOnly]`：表明数据只读。

#### 内存分配器

原生内存需要自己管理，所以创建原生容器时必须声明分配器类型，以告诉 Unity 需要使用多长时间。分配器类型将影响 Unity 创建和回收内存的方式，**确保正确的类型声明将获得最佳性能**。

- Allocator.Temp：最快的分配，从栈中申请的内存，生命周期为一帧或更短。不能传递给作业，但可以在作业代码中做局部变量使用。
- Allocator.TempJob：中等的分配，声明周期为四帧。常用于给作业传递数据，需要手动及时回收。
- Allocator.Persistent：最慢的分配，可一直存在直到被手动回收。这是直接通过调用[`malloc`](https://cplusplus.com/reference/cstdlib/malloc/)声明的堆内存。

#### 高级主题

- 自定义原生容器：

  可创建自己的原生容器类型，具体参考文档：  
  https://docs.unity.cn/2022.3/Documentation/Manual/job-system-custom-nativecontainer.html

- 复制原生容器：

  深入了解原生容器的复制相关事宜，具体参考文档：  
   https://docs.unity.cn/2022.3/Documentation/Manual/job-system-copy-nativecontainer.html

### 特别提示

- 原生容器不实现[`return ref`](https://learn.microsoft.com/zh-cn/dotnet/csharp/language-reference/statements/declarations#reference-variables)，所以`nativeArray[0]++`不会修改数组内容。
- 静态变量将绕过所有安全系统，并可能导致软件崩溃。
- 仅可用原生容器获取返回值，其他变量[只会被封送到原生内存而不会送回](https://stackoverflow.com/questions/65260387/why-is-nativearray-needed-to-obtain-return-values-from-unitys-job-system)。

## 创建并运行作业

创建并运行作业需要以下几步：

1. 创建作业：

   实现`IJob`接口，并创建作业实例，传入自定义的作业数据。

2. 安排作业：

   调用`Schedule`方法，Unity 将复制作业数据（避免和其他作业同时读写），并开始在工作线程执行作业。返回的`JobHandle`可用于建立依赖项。

3. 完成作业：

   调用`Complete`方法，如果作业已完成将立即返回，否则阻塞等待。此时安全系统状态将清理，作业中的数据可再次访问。

### 作业依赖

调用`Schedule`方法将返回一个`JobHandle`对象，可将该`JobHandle`作为其他作业的依赖项，表示其他作业需要该作业的完成结果。

此外可以使用`JobHandle.CombineDependencies`合并依赖项，以实现同时对多个作业结果的依赖。

### 并行作业

Unity 将所有`Execute`执行任务划分为多个批次，然后以批次为单位分配给工作线程处理，通常每个内核都会分配一个工作线程。

当一个工作线程完成所有分配给它的批次时，它还会尝试窃取其他工作线程中尚未完成的批次继续处理，每次最多窃取一半。

**要优化流程，需要指定批次计数**。批次计数是指每个批次的包含的`Execute`执行数量，如果值为 1 可以确保批次分配均匀，但过多的批次分配会带来一定开销，为此应逐步增加批次，直至性能提升忽略不计。

### 最佳实践

- 尽可能延后`Complete`的调用时机：

  调用`Complete`将阻塞主线程等待作业完成，否则作业可在后台异步执行。因此非必要情况应尽可能让作业多在后台执行，例如若能接收一帧延迟，可将作业放在每帧的结束开始之间运行。

- 避免运行长任务，应将其拆分成互相依赖的小任务：

  增加任务数有助于提高并行性，使多个作业链同时进行。否则可能耗光工作线程，导致其他独立作业无法执行，从而引起等待完成时发生的卡顿。

- 任务过长且无法拆分时应考虑增加批次计数：

  作业系统有意尝试在尽可能多的线程上运行作业批次，增加批次计数可增加单个工作线程的压力，从而减少对其他工作线程的占用。
