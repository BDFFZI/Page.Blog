---
abbrlink: 3495031609
date: 2024-10-10 20:00
categories:
  - 工具使用
  - Blender
---

# 【Blender】杂项笔记

## 空间坐标系

Blender 中的轴向：

- Y 轴向前（前视图看向的方向就是前方，其默认向 Y 轴看）
- Z 轴向上

保持轴向导出到 Unity 时（包括直接保存、导出 FBX 但保持轴向）：

- Unity 会继续保持 Y 轴和 Z 轴坐标，但反转 X 轴。

不保持轴向导出到 Unity 时：

- 顶点数据与保持的情况一致，但模型旋转被修改。
- 可以推出 **Blender 是仅通过旋转模型来模拟的轴变换**。

举例：

1. 导出 FBX 时选择 Y 向上，-Z 向前，此时若要将 blender 环境改为该坐标系，需要将原坐标系 x 旋转 -90 度。
2. 而为保持模型在环境中不变，相对就需要对模型 x 轴旋转 90 度，这就是轴向改变对模型的影响。
3. 接着导出到 Unity，由于 Unity 默认对右手坐标系模型 x 轴转-90 度，两者一抵消，于是在 Unity 显示旋转为 0。

## 实用插件

- [空物体转骨架](https://github.com/artellblender/empties_to_bones)

## 操作技巧

- [导出 FBX 至 Unity](https://polynook.com/learn/how-to-export-models-from-blender-to-unity)
