---
abbrlink: 953923210
date: "2024-01-20 09:56"
update: "2024-2-19 16:37"
categories:
  - 工作
  - Blender
---

# 【Blender】插件开发笔记

## 开发环境配置

1. 打开设置“界面-开发选项”：  
   这样可以通过对功能按钮的右键菜单直接复制或查看其源码，以及快速跳转到 API 手册。
2. 打开设置“界面-Python 工具提示”：  
   这样可以通过界面的悬停菜单直接查看该条目在代码中的表示方式。
3. 安装 python 环境，下载 fake-bpy-module 包：  
   这样可以在外部编辑器编写代码，并且有代码代码提示。
4. 下载 VSCode 及其插件 Blender-Development：  
   这样可以快速创建插件项目并能自动导入和调试插件。

## 插件基本原理

### 插件是什么？

插件是将代码集成到 Blender 的一种方式，只要符合以下要求，便可被 Blender 识别为插件。

- 插件是一个 Python 包，即一个带有\_\_init\_\_.py 的文件夹。
- 该包带有 register()和 unregister()两个函数。

### 插件如何被集成？

Blender 识别到插件会主动调用其中的 register 和 unregister 函数，这分别发生在启用插件和禁用插件两个时段，除此之外 Blender 不会调用任何函数。

所以我们必须借助 register 和 unregister 两个时段，主动将我们的“功能”注册给 Blender 编辑器或从中取消注册。

向 Blender 编辑器注册或取消注册是借助 Blender 的内置函数实现的，使用该函数时必须要提供一个参数，而该参数正是用来传递我们的“功能”的。

```python
# 向Blender集成功能的代码示意

import bpy # blender提供的所有功能都在bpy包中

functions = [function1,function2,function3,...] # 我们的功能

def register(): # Blender将在启用插件时调用该函数
    for f in functions: # 利用数组加循环快速批量注册
        bpy.utils.register_class(f) # 注册功能


def unregister(): # Blender将在禁用插件时调用该函数
    for f in functions: # 利用数组加循环快速批量取消注册
        bpy.utils.unregister_class(f) # 取消注册功能
```

### 集成的功能是什么？

注册功能的函数是 Blender 提供的，所以我们的功能也必须符合 Blender 注册函数对参数的要求。

具体来说这些功能实际上是几个 Blender 接口类的实现，目前 Blender 提供以下可用于集成的接口类：

```python
bpy.types.Panel # 常见的用户界面
bpy.types.Menu # 菜单项（可通过“编辑-菜单查找”查看）
bpy.types.Operator # 操作命令（可通过“编辑-操作搜索”查看）
bpy.types.PropertyGroup # 批量存储数据的容器
bpy.types.KeyingSet
bpy.types.RenderEngine
```

具体关于这些接口类的解释，留到后续使用时再进行讲述。

除此之外其实还有些更高级的功能也可以注册，例如网格修改器， 对象类型或着色器节点等，但这些必须要用 C/C++实现，故不做讨论。

## Blender 关键概念

### 内置资源

> 所有资源都由集合存储管理，资源的创建销毁是通过对集合的增删来实现。

在此我们把 Blender 自带的数据类型，如物体，网格，材质等叫做内置资源。

Blender 中所有的内置资源都有专门的类对应，而它们的实例则全部统一存在几个集合中。

Blender 完全用这些集合控制资源的生命周期，所以资源的创建销毁不能用常规的 new，delete 方式，而是要直接操作这些集合，集合的增删就代表着类的创建销毁。

这些集合可以通过 bpy.data 进行访问，也可以通过“大纲视图-Blender 文件”界面进行可视化访问。

```python
# 内置资源处理伪码
import bpy
newMaterial = bpy.data.materials.new('New Material') # 创建资源
oldMaterial = bpy.data.materials['Old Material'] # 获取资源
bpy.data.materials.remove(oldMaterial) # 删除资源
```

### 自定义属性

> 若要存储或显示自己的数据，必须借助“自定义属性”功能来实现。

自定义属性是 Blender 提供的一种在 Blender 文件中存储数据的方法，除此之外没有其他方式。

用户可以利用 Blender 提供的属性类创建自己的数据容器，并将其附加到任何一个 Blender 的内置资源上。

不单是存储数据，如果要在面板上显示数据，也必是自定义属性的数据才行。

在代码中利用 bpy.props 空间提供的类就能创建自定义属性对象，或在资源面板的“自定义属性”子面板中也能可视化编辑。

```python
# 自定义数据存储伪码
import bpy

def register():
    # 向Scene类型注册一个名为my_data的自定义属性
    bpy.types.Scene.my_data = bpy.props.StringProperty()

def unregister():
    # 取消注册自定义属性
    del bpy.types.Scene.my_data

def my_function():# 在自己的函数中使用自定义属性
    bpy.data.scenes['Scene'].my_data = 'Hello Blender' # 给名为"Scene"的场景资源中的自定义属性赋值
    print(bpy.data.scenes['Scene'].my_data) # 读取自定义属性

```

### 运算符

> 若要实现显示或执行自定义功能，必须借助“运算符”功能来实现。

Blender 中所有的可执行功能也即界面上的按钮都一定对应一个运算符，可以借此在代码中像在用户界面上一样调用这些功能，blender 内置的运算符都存放在 bpy.ops 空间。

同理若要实现自己的功能并显示在界面上也一定要实现自己的运算符。

```python
# 自定义运算符伪码

class MyOperator(bpy.types.Operator): # 某个自定义运算符
    bl_idname = "my.operator" # 自定义属性的ID

class MyPanel(bpy.types.Panel): # 某个自定义面板
    layout.operator(MyOperator.bl_idname, text="MyOperator") # 显示运算符

# 两者都需要注册
bpy.utils.register_class(MyOperator)
bpy.utils.register_class(MyPanel)
```

## Blender 框架结构

根据框架结构可以了解到 Blender 中有哪些可操作对象，以及它们的关系，配合英文名搜索，可以快速定位目标对象在代码中的修改位置。

### 库结构

```python
bpy.types #blender中所有的类型，可以此为特定类注册自定义属性，以及获取用于继承的接口类。
bpy.props #blender中所有的属性类型，可用于创建自定义属性。
bpy.ops #blender中所有的自带运算符，当成函数调用则可直接运行。
bpy.data #当前文件里所有的资源，对应“大纲视图-Blender文件”界面。
bpy.context #当前编辑环境，如选中的物体，正在使用的编辑模式等。
bpy.utils #可选的帮助函数，注意用于注册或取消注册自定义类。
bpy.msgbus
bpy.path
bpy.app
```

### 文件结构

参考“大纲视图-Blender 文件”界面

```python
bpy.data.
    #用户界面相关
    window_managers. #窗口管理器
    workspaces. #工作区：编辑器顶部切换工作区域的工具栏
    screens. #屏幕：编辑器中间的工作区域
    #工具相关
    brushes. #笔刷
    palettes. #调色板
    #场景资源
    worlds. #世界环境
    collections. #集合
    objects. #物体
    scenes. #场景
    #物体资源
    cameras. #摄像机
    lights. #灯光
    meshes. #网格
    #渲染资源
    images. #图像
    materials. #材质
    linestyles. #线条样式
```

### 资源结构

参考“大纲视图-Blender 文件”界面

```python
bpy.context.
    scene. #当前场景
        view_layers. #视图层
        world. #世界环境
        collection. #集合
        objects. #物体
    active_object. #当前选中的物体
        data. #物体绑定的数据，存在多种类型
        data(camera). #相机
        data(light). #灯光
        data(mesh). #网格
            materials. #材质
```

### 界面结构

参考 <https://docs.blender.org/manual/en/latest/interface/index.html#window-system>

```python
bpy.context.screen. #当前工作区
    areas. #工作区细分为多个功能区域
        regions. #将区域中的界面按部位进一步细分
        spaces. #区域空间信息，存在多种空间类型
        space(SpaceView3D). #3D视图空间
```

#### SpaceView3D

```python
SpaceView3D.
    region_3d. #3D视图区域数据
        view_perspective #视图投影模式
        view_camera_offset #相机视图偏移
        view_camera_zoom  #相机视图缩放
```

### 工具结构

#### 图像绘制工具

```python
bpy.context.tool_settings #当前使用工具的相关设置
    image_paint. #图像绘制工具
        brush. #当前使用的笔刷
            texture_slot #纹理功能设置
            stencil_dimension #楼板尺寸
            stencil_pos #楼板位置

bpy.context.active_object. #当前选择的物体
    active_material. #当前选择的材质球
        texture_paint_images #当前可绘制的贴图
        paint_active_slot #当前绘制中的贴图索引

```

## 常用接口类

### Operator

添加自定义操作时必须要实现 Operator。

```python
class bpy.types.Operator:
    # 必填，需要按“类别.名称”的ID格式填写，具体内容可自定义，但点号必须要有且全小写。
    bl_idname : str
    # 必填，表示操作名称。
    bl_label : str

    # 可选，用于告知blender该运算符是否可用。
    @classmethod
    def poll(cls, context) -> bool

    # 必填，用于编写执行操作的具体代码。
    def execute(self, context) -> Set[str]
```

### Panel

添加自定义面板界面时必须要实现 Panel。

```python
class bpy.types.Panel：
    # 必填，表明该面板应显示在哪种界面空间上。
    bl_space_type : str
    # 必填，表示该面板的区块类型。
    bl_region_type : str
    # 必填，表示面板标题。
    bl_label : str

    # 可选，表明显示在所处界面的哪个二级分类下。
    bl_category : str

    # 必填，用于编写绘制界面的代码。
    def draw(self,context) -> None
```

### PropertyGroup

自定义参数过多时可以借助 PropertyGroup 实现合并注册。

```python
class bpy.types.PropertyGroup:
    # 任意个用类型注解表示的自定义属性，不要赋值
    ......
```

## 常用操作

### 绘制界面

bpy.types.Panel 等类提供名为 layout 的成员变量，该变量提供绘制函数使用。

```python
# 使用案例
class MyPanel(bpy.types.Panel):
    def draw(self, context):
        layout = self.layout

        # 显示文字
        layout.label(text="渲染参数")

        # 显示属性
        column = layout.column(align=True) # 创建布局组
        column.prop(context.scene.render, "resolution_x",text="宽度")
        column.prop(context.scene.render, "resolution_y",text="高度")

        # 显示操作
        layout.operator("view3d.view_center_camera", text="重置摄像机视图")
```

### 弹出信息

bpy.types.Operator 等类提供名为 report 的成员函数，可以在底部信息栏弹出消息。

```python
# 使用案例
class MyOperator(bpy.types.Operator):
    def execute(self, context):
        if(context.active_object == None):
            self.report({"ERROR"}, "当前没有激活的物体")
            return {"CANCELLED"}
        return {"FINISHED"}
```

## 细节提示

- 在注册期间无法访问 bpy.data 和 bpy.context，所以要注册自定义函数必须借助 bpy.types 以类为目标。
- 可以借助“编辑”菜单中的“菜单查找”和“操作搜索”找到注册进 blender 的自定义菜单或运算符。
- 如果要利用插件传递资源，可以在插件中携带 blender 文件，并利用“关联”和“追加”功能实现。
- 如果在自定义运算符的 bl_idname 中使用了非官方的类别，那右键菜单中将缺少指定快捷键功能。

## 参考资料

- [Blender 官方 API 文档](https://docs.blender.org/api/current/index.html)
