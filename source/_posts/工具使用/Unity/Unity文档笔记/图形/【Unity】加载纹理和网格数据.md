---
abbrlink: 36610906
date: 2024-5-31 17:30
categories:
  - 工具使用
  - Unity
  - Unity文档笔记
  - 图形
---

# 【Unity】加载纹理和网格数据

Unity 使用同步和异步两种方式加载纹理和网格到 GPU，如果资源支持，则 Unity 将**默认使用异步加载**。同步加载会导致游戏卡顿，但异步则不会，所以考虑性能优化，**应尽可能保证资源满足异步加载条件**。

## 工作原理

资源数据由“元数据”（标头数据）和“像素顶点数据”（二进制数据）组成。当资源被要求加载时，其中标头数据必须立即被加载，但二进制数据实际可以在后续慢慢加载。

### 同步上传管线

- 构建时：Unity 将标头数据和二进制数据打包在同一文件中（.res）。
- 运行时：需要资源时，Unity 将 .res 加载到内存中，并在加载完毕后将二进制数据上传 GPU，所有操作在同一帧中完成。

### 异步上传管线

- 构建时：Unity 仅将标头数据存储在 .res 文件中，二进制数据存在单独的 .resS 文件中。
- 运行时：先将标头数据加载进内存，随后利用异步上传方式上传二进制文件，具体流程如下：

  1. 异步等待所需的内存在环形缓冲区中可用。
  2. 将数据从源 .resS 文件读取到分配的内存中。
  3. 执行后处理（纹理解压缩、网格碰撞生成、每个平台修复等）。
  4. 在渲染线程上以时间切片方式分多帧上传数据至 GPU。
  5. 释放环形缓冲器内存。

- 存在多个异步上传命令时：

  可以同时进行多个异步上传命令，所有命令共享环形缓冲区，如果环形缓冲区已满，后续命令将等待，这不会造成主线程阻塞，只是减慢异步加载过程。

- 当资源超出缓冲区最大大小时：

  加载单个数据到 CPU 内存是一次性完成的，如果缓冲区完全无法容纳，Unity 会待缓冲区使用完毕后临时重新分配缓冲区大小，上传完后再还原，但这个过程非常缓慢。

## 异步加载要求

资源必须满足以下要求才可支持异步上传管线，否则将一律被 Unity 用同步的方式加载。

- 纹理与网格的通用要求

  - 未启用读/写权限。
  - 不在 Resources 文件夹中。
  - 如果构建目标是 Android，在项目的构建设置 (Build Settings) 中启用了 LZ4 压缩。

- 针对网格的额外要求

  - 网格没有混合形状数据。
  - 网格没有骨骼权重数据。
  - 网格拓扑不是四边形。
  - 网格没有启用网格压缩。
  - 网格不会被动态批处理。
  - 网格数据没有被粒子系统、地形、碰撞器使用。

## 判断是否被异步加载

可通过 Profiler 分析工具观察线程活动来识别，具有以下任意特征时表示被异步加载：

- 具有以下标记的活动：
  - AsyncUploadManager.ScheduleAsyncRead
  - AsyncReadManager.ReadFile
  - Async.DirectTextureLoadBegin
- AsyncRead 线程上活动。

除此之外都表明未使用异步加载。即使依然能看到一些带有 async 的活动，但那只有 Unity 用于检测是否需要异步加载而已。

## 配置异步加载管线

质量设置面板可以调节部分异步加载管线的参数：

- Async Upload Time Slice：上传时间片，每帧花多少毫秒用于上传数据。
- Async Upload Buffer Size：环形缓冲区的大小，二进制数据加载进内存的临时空间。
- Async Upload Persistent Buffer：是否持续保留环形缓冲区内存而不释放。

最佳的设置方法是：

1. 保证不丢帧的情况下尽可能把上传时间片调大，在一些不影响视觉效果的时候（如加载界面）还可以临时进一步调大。
2. 通过分析器检查时间片时间是否被完全利用，如果未完全利用一般说明缓冲区大小不够用，需增加环形缓冲区大小。
3. 超出缓冲区上限的大型资源会导致 Unity 临时重建缓冲区，如果这种情况较多，应增加默认缓冲区大小以避免发生。
4. 释放内存会导致内存碎片，所以除非有令人信服的理由，否则不要关闭保留缓冲区内存选项。
