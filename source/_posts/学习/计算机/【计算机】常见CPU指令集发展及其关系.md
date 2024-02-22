---
abbrlink: 1481637271
date: "2024-2-22 21:22"
categories:
  - 学习
  - 计算机
---

# 【计算机】常见 CPU 指令集发展及其关系

## CPU 与指令集

任何计算机都有一块 CPU，CPU 有其支持的指令集，根据指令集间的兼容性，一种 CPU 可能同时支持多种指令集。

指令集中记录了 CPU 的机器代码格式，由于很多编译性语言最终都是要编译成机器代码才能运行，所以编译时必须指明目标指令集，在 IDE 中也能看到对应选项。

使用不同指令集的平台间软件通常无法兼容，所以跨平台时需要重新打包程序，或者也有同时打包多份不同指令集的程序塞在一个软件中的方法，以便平台按需选取。

这也是为什么 Github 中很多支持跨平台的软件，其发布时都会提供好多种不同指令集后缀的版本，或者不写清指令集但体积巨大的版本。

## x86 系列

### x86

x86 架构由 Intel 公司发明，首次使用在其 1978 年推出的 Intel 8086 处理器中，且后来逐渐成为了个人电脑的标准架构。

最早使用该架构的系列处理器（如 Intel 8086、80186、80286 等）都是以数字表示，且都采用了 86 作为后缀，所以该架构也就称为 x86 架构，其中 x 是为了表示同系列有多款 CPU 使用该架构。

### x86、i386、IA-32

最早的 x86 是 16 位的，但从 80386 开始被经改进成为了 32 位架构。

对于这升级后的一新架构，Intel 将其称为 IA-32（Intel Architecture 32bit）；而在 Linux 平台则习惯将其称为 i386，因为这是从第三代 86 系列处理器（80386）开始的。

如今 16 位的 x86 已经淘汰，所以通常我们也会直接称呼 x86 来泛指其 32 位版本。

### IA-64

在 1989 年，惠普认为现有的 RISC 架构存在性能瓶颈，于是在 1994 年与 Intel 开始合作设计一款全新的架构 IA-64。Intel 很看好这一新架构，投入了大量开发资源，并在 2001 年推出第一代基于 IA-64 的安腾处理器。

但该架构有一个致命缺陷，那就是和之前大火的 x86 架构不能兼容，且旧架构通过增强超标量的方法足以赶上 IA-64 的优势，所以该架构很快就退出市场了，2017 年之后 Intel 再也没发布过基于 IA-64 的新处理器。

### x86_64、x64、AMD64、Intel 64

在 1976 年时，Intel 公司将 8080 架构（x86 的前身）的微指令授权给了 AMD 公司，导致 AMD 公司拥有在 x86 基础上继续开发的权力。

之后除开 Intel，AMD 公司也在进行 64 位架构的开发。其吸取了 IA-64 的失败，选择在 x86 的基础上继续开发，并于 1999 设计出了新的 AMD64 架构。

AMD64 架构可以说就是 x86 架构的 64 位版本，所以与之兼容，也为了给 Intel 留点面子，所以该架构又叫 x86_64 架构，简称 x64。

后来 Intel 公司也开始采用该架构，并将其直接将称为 Intel 64。

## ARM 系列

ARM 架构是 ARM 公司推出的指令集，过去也被称为进阶精简指令集机器（Advanced RISC Machine），其设计简单且功耗低，所以广泛使用在嵌入式和移动通讯领域。

ARM 架构随着时代发展发布了多个版本，最早于 1985 年开发出了第一代 ARM 架构（ARMv1），次年又开发了第二代 ARM 架构（ARMv2） 并开始量产。

在早期 ARM 的指令集架构一直以“ARMv{版本号}”的形式命名。但为了顺应市场发展，从 ARMv6 开始，又会以“ARMv{版本号}-{型号}”的命名方式推出多个分支指令集架构，以应对不同的使用环境。而从 ARMv7 开始则完全采用后一种命名方式，所以说此时实际已不存在名为 ARMv7 的某一特定架构，ARMv7 是对第七代 ARM 架构的统称。

相应的，之后的处理器也改为了“Cortex-{型号}{版本号}”的命名方式，比如曾经手机主流的 Cortex-A7 处理器就是基于 ARMv7-A 指令集架构制作的。

虽然 ARM 架构的版本众多，但 ARM 架构具有版本兼容性，采用旧版本指令集构建的软件在搭载新版本指令集的处理器中依然可用，从而保障了它的发展生命力。

今天最多听到 ARM 架构的地方都是在 Android 开发上，Android 开发中通常用 ABI 来描述程序集所用框架，虽然 ABI 并不是专指指令集架构但也有包含的关系，所以考虑常用性这里就借用 ABI 介绍指令集好了。

### armeabi、ARMv5、ARMv6

armeabi 是最早手机所用的 ABI，其对应 ARMv5 架构 ，因为兼容性的原因后续的 ARMv6 等架构处理器也可使用，不过现今该架构已被 Android NDK 弃用。

### armeabi-v7a、ARMv7、ARMv7-A

armeabi-v7a 是 armeabi 架构的升级版，其中 v7a 就是指其对应架构为 ARMv7-A 。其一个显著的特点是增加了硬件浮点运算的功能，所以大幅提高了性能，是过去非常主流的手机架构。

不过 ARMv7-A 是 32 位指令集，在如今的 64 位时代已然过时，目前市面上只有少部分手机还在使用。

### arm64-v8a、ARM64、ARMv8-A

arm64-v8a 对应 ARMv8-A 架构，其名称中的 64 宣布了又一个里程碑式的进步，从此刻开始 ARM 架构也步入了 64 位时代，并且从 2019 年 8 月 1 日起 Google Play 就开始强制要求所以软件都必须支持 64 位设备，即支持 arm64-v8a。

所以该架构是目前手机最主流的架构。

## mips 系列

在查资料时发现的另一种指令集系列，很多跨平台软件的发布类型中除开 x86 和 ARM 就剩它了，但生活中并没见过，而且其和 armeabi 一样被 Android NDK 弃用了，显然应该是某种过时的处理器架构，所以此处不进行讨论。

## 参考资料

- [计算机中 x32、x64、x86 是什么意思？](https://www.zhihu.com/question/63040129)
- [为什么 64 位是 x64 而 32 位是 x86？](https://www.zhihu.com/question/438625652)
- [i386 和 x86-64 有什么区别？](https://www.zhihu.com/question/19573226)
- [百科-安腾处理器](https://baike.baidu.com/item/%E5%AE%89%E8%85%BE%E5%A4%84%E7%90%86%E5%99%A8)
- [Intel 为什么不取消 AMD 的 X86 架构授权？](https://www.zhihu.com/question/397027038)
- [百科-X86-64](https://baike.baidu.com/item/X86-64/1352293)
- [百科-ARM 架构](https://baike.baidu.com/item/ARM%E6%9E%B6%E6%9E%84)
- [armv7-A 系列 0 - ARM 处理器架构发展史](https://zhuanlan.zhihu.com/p/362648496)
- [List of ARM processors](https://en.wikipedia.org/wiki/List_of_ARM_processors)
- [Android ABI](https://developer.android.google.cn/ndk/guides/abis?hl=zh-cn)
- [支持 64 位架构](https://developer.android.com/google/play/requirements/64-bit)
- [UnityAPI AndroidArchitecture](https://docs.unity3d.com/2020.1/Documentation/ScriptReference/AndroidArchitecture.html)
