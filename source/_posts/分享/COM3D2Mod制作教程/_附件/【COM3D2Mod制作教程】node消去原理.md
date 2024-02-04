---
categories:
  - 分享
  - COM3D2Mod制作教程
  - 附件
---
# 【COM3D2Mod制作教程】node消去原理

## menu条目解释

- node消去：将目标骨骼及其子骨骼所影响的面标记为隐藏
- node表示：将目标骨骼及其子骨骼所影响的面标记为显示

## 功能注意点

1. 骨骼名称支持模糊匹配，只要包含目标名称可。  
如Mune可以同时影响Mune_L和Mune_R等。
2. 先标记后结算，结算时只执行隐藏功能。  
故如果一个三角面同时被两根骨骼影响，那只要一根被标记隐藏，那该三角面就会被隐藏。

## 底层代码原理

```c#
/// [Update]表示该函数会在其他时机被调用，具体调用时机我没细看。

// 读取.menu文件，遇到“node消去”和“node表示”时会调用TBody的SetVisibleNodeSlot
class Menu;

class TBody
{
    // 骨骼名称的哈希表
    public static Hashtable hashSlotName;

    // 根据装扮类型有多个槽位，每个装扮类型对应的槽位由hashSlotName获取。
    // 注意：第一个槽位是身体槽位，直接控制着身体模型。
    public List<TBodySkin> goSlot;

    // 调用goSlot中对应部位TBodySkin的SetVisibleFlag。
    public void SetVisibleNodeSlot(string slotname, bool boSetFlag, string name);
    
    // 统计goSlot中所有TBodySkin的m_dicDelNodeBody，根据其值通过goSlot[0].morph.SetVisibleFlag1调整身体可见性。
    [Update]
    public void FixVisibleFlag();
}

class TBodySkin
{
    // 以骨架的所有子物体名称为键
    public Dictionary<string, bool> m_dicDelNodeBody;
    // 用于实际处理网格数据
    public TMorph morph;

    // SetVisibleFlag会遍历身体骨架将名称符合的物体及其子物体的名称作为键，将m_dicDelNodeBody打开或关闭。
    public void SetVisibleFlag(bool boSetFlag, string name, Transform t = null, bool boTgt = false);
}

class TMorph
{
    // 部位名称及其显示状态，排序按Unity中Mesh.bones的顺序
    public List<string> BoneNames;
    public List<bool> BoneVisible;

    // idx是由BoneNames得出，用于在BoneVisible中设置部位是否显示，设置好后会将m_bDut设为true
    public void SetVisibleFlag1(int idx, bool flag);

    // 设置是否为脏
    private bool m_bDut;

    // 当m_bDut不为false时更新网格，通过骨骼权重影响的顶点获取部位对应的三角面，并将三角面面积设0来隐藏面。
    // 只隐藏不显示，所以如果有两根骨骼共用三角面，且先隐藏后显示，那也只会隐藏。
    [Update]
    public void FixVisibleFlag();
}
```
