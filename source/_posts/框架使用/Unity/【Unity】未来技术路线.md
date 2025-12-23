---
categories:
  - 工具使用
  - Unity
abbrlink: 2200103232
---
# 【Unity】未来技术路线

## 新旧更替

- UnityEngine.Mathf->Mathematics
- UnityEngine.Input->InputSystem
- UnityEngine.UI.Text->TextMeshPro
- Build-in RenderPipeline->URP
- ParticleSystem->VisualEffectGraph
- SurfaceShader->ShaderGraph
- UnityEngine.UI->UI Toolkit
- Build-in Terrain->Terrain Tools
- Resources/AssetBundle->Addressables
- Build-in BuildPipeline->SBP
- GameObject->ECS
- UNet->Netcode

## 实用扩展包

### 技美开发

- Cinemachine：超强相机控制。
- Timeline：制作基于场景范围的分镜动画。
- Sequences：汇总分镜信息，串联Timeline制作完整过场动画。
- Animation Rigging：实现复杂变换约束。
- Polybrush：Unity内修改模型数据，调整美术效果。
- ProBuilder：Unity内快速建模，搭建测试场景。

### 编辑器扩展

- Recorder：编辑器内录制游戏画面。
- FBX Exporter：将Unity内的模型、灯光动画等数据导出为FBX文件。
- Python Scripting：在编辑器运行时编写和执行脚本。
- Editor Coroutines：实现异步编辑器脚本。

### 性能优化

- Adaptive Performance：运行时获取设备功率温度等信息。
- Memory Profiler：内存使用状况分析器。
- Profile Analyzer：汇总对比性能分析器数据。
- Code Coverage：代码覆盖率检测。

### 平台开发

- Device Simulator Devices：移动设备模拟器
- Mobile Notifications：移动设备的通知功能

### 功能实现

- Localization：实现游戏多语言支持。
- Burst,Collections,JobSystem：超高性能的功能实现。
- Splines：创建基于曲线的功能。
