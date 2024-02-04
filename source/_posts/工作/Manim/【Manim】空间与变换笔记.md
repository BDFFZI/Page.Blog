---
abbrlink: 3454987331
date: '2024-01-14 13:29'
categories:
  - 工作
  - Manim
---
# 【Manim】空间与变换笔记

所有常量都可以在constants.py中找到

## 屏幕空间

屏幕中心为原点(0,0,0)，遵循右手坐标系，向右为x轴正方向，向上为y轴正方向，向前为z轴负方向，旋转时正方向为顺时针方向。

### 相关常量

名称|描述
--|--
FRAME_HEIGHT|屏幕高度，默认为8
FRAME_WIDTH|屏幕宽度，默认为ASPECT_RATIO * FRAME_HEIGHT
ASPECT_RATIO|屏幕宽高比，默认为16 / 9
FRAME_Y_RADIUS|屏幕高度的一半
FRAME_X_RADIUS|屏幕宽度的一半

## 向量（np.ndarray）

### 创建方式

```python
np.array([x,y,z])
```

### 相关常量

名称|对应坐标|描述
--|--|--
RIGHT|(1,0,0)
UP|(0,1,0)
LEFT|(-1,0,0)
DOWN|(0,-1,0)
UR|(1,1,0)
UL|(-1,1,0)
DR|(1,-1,0)
DL|(-1,-1,0)
TOP|(0,FRAME_Y_RADIUS,0)|屏幕最上方
BOTTOM|(0,-FRAME_Y_RADIUS,0)|屏幕最下方
RIGHT_SIDE|(FRAME_X_RADIUS,0,0)|屏幕最右方
LEFT_SIDE|(-FRAME_X_RADIUS,0,0)|屏幕最左方
OUT|(0,0,1)
IN|(0,0,-1)

## 变换函数

以下都是Mobject对象的成员函数。
函数名|作用
--|--
shift|移动物体
move_to|移动物体到指定位置
scale|缩放物体
rotate|旋转物体
flip|翻转物体
stretch|拉伸物体
to_corner|移动物体到屏幕角落
to_edge|移动物体到屏幕边缘
align_to|对齐物体
next_to|排列物体
set_height|设置物体高度
set_width|设置物体宽度

### 可选参数

以下为部分举例，因为内容过多，不同函数参数也有不同，所有不方便记。但相关内容我也没在文档里找到，不过我发现在源码里可以找到。每个类可用的参数，都作为字典写在了类结构里，翻翻几个基类的源文件就知道了。
参数名|作用
--|--
aligned_edge|设置物体相对于目标点的对齐方式
coor_mask|限制物体在部分轴上不运动
about_edge|设置相对于边沿的运动
about_point|设置相对于点位的运动
buff|设置相对于默认变换结果的空间空隙
submobject_to_align|使用子物体进行对齐
index_of_submobject_to_align|对齐到目标的子物体

### 动画效果

需要配合scene.play使用，下方为举例。

```python
self.play(
    image.shift,
    np.array([1,0,0])
)
```
