# 【Unity】视图投影矩阵推导

## 视图矩阵

视图矩阵本质就是不受缩放影响的相机物体空间矩阵的逆矩阵。此外 Unity 使用的是 opengl 风格的视图矩阵，即最终会对 z 轴进行反转，使相机正前方为-z（即会对矩阵中的 m33 取反）。

~~这样设置是为了后续便于将深度计算为 DX 风格的 1-0（越远深度值越小），而不是传统风格的 0-1（越远深度值越大）。~~（根据投影矩阵的映射关系，显然不是，但深度图确实是以这种方法存储的）

## 投影矩阵

投影矩阵用于将视图矩阵的结果转换到剪辑空间（采用齐次坐标的 NDC），但具体根据当前所使用的图形 API 不同，其投影矩阵和 NDC 都会有所差异：

https://docs.unity.cn/cn/2022.3/Manual/SL-PlatformDifferences.html

对于 NDC 的 x,y 轴，全平台都是一致的：

- 屏幕从左到右为 x 轴的-1 到 1
- 屏幕从下到上为 y 轴的-1 到 1

对于 NDC 的 z 轴，即视图空间下的近平面到远平面的 z 轴：

- 在 OpenGL 平台：屏幕从前到后为 z 轴的-1 到 1
- 在 Direct3D 平台：屏幕从前到后为 z 轴的 1 到 0

从相机中直接获取投影矩阵（`Camera.projectionMatrix`），Unity 始终返回 OpenGL 风格。但若想获取着色器中实际使用的矩阵，则需要调用`GL.GetGPUProjectionMatrix`，而该矩阵会随图形 API 不同而不同。

考虑 Unity 的深度图是采用 Direct3D 风格存储的，此次仅推导 Direct3D 风格的透视矩阵，另外由于投影矩阵可以拆成正交和透视两个矩阵过程，因此此处推导也分成两份便于计算。

### 正交矩阵

正交矩阵由以下参数构成：

- size：视锥体半高度。
- aspect：宽高比（宽度/高度），用于得出半宽度。
- near：近平面位置。
- far：远平面位置。

由这些参数可以简单得出以下变量：

- h：半高度（size）
- w：半宽度（size\*aspect）
- n：近平面（near）
- f：远平面（far）

正交矩阵是线性变换，所以可以通过直线公式来实现（$y=Ax+B$），具体而言是要实现以下映射：

- $(-h,h)=>(-1,1)$
- $(-w,w)=>(-1,1)$
- $(-n,-f)=>(1,0)$（受视图矩阵的 z 反转影响，故远近平面取反）

对于第一第二点，只要改变斜率，除以对应的高度或宽度即可。对于第三点则可以构成以下公式：

- $-An+B=1$
- $-Af+B=0$

推导可得

$
\begin{aligned}
(-An+B)-(-Af+B) &= 1-0 \\
-An+B+Af-B &= 1 \\
Af-An &= 1 \\
A(f-n) &= 1 \\
A &= \frac{1}{f-n} \\
\end{aligned}
$

$
\begin{aligned}
-(\frac{1}{f-n})f+B&=0\\
B &= \frac{f}{f-n}\\
\end{aligned}
$

最终根据上述结论可构成矩阵

$$
\begin{bmatrix}
\frac{1}{w} & 0 & 0 & 0 \\
0 & \frac{1}{h} & 0 & 0 \\
0 & 0 & \frac{1}{f-n} & \frac{f}{f-n} \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
$$

### 透视矩阵

透视矩阵要实现近大远小，即根据 z 位置缩放 xy 轴，使任何位置的 x,y 都等于近平面的 x',y'。因此根据相似三角形定理可得如下公式：

$
\begin{aligned}
\frac{y'}{-n} &=\frac{y}{-z} \\
y'&=\frac{yn}{z}\\
x'&=\frac{xn}{z}（如上同理）
\end{aligned}
$

乘 n 通过缩放矩阵就可实现，而实现除 z 则需利用齐次坐标实现（w 分量设为 z），具体方法就是将矩阵 m43 设为-1（注意因为视图矩阵中 z 被反转，此处为保证 xy 不受影响，需再次反转以获取 +z）。

但齐次坐标不应影响 z 轴，故需要对 z 特殊处理，至少要实现如下效果：

- $-An+B=-n^2$（齐次坐标是实现除 z 而不是-z，所以为保持 -z 不变，等式右侧为负号）
- $-Af+B=-f^2$

推导可得

$
\begin{aligned}
(-An+B)-(-Af+B) &= (-n^2)-(-f^2) \\
-An+B+Af-B &= f^2-n^2 \\
Af-An &= (f-n)(f+n) \\
A(f-n) &= (f-n)(f+n) \\
A &= f+n \\
\end{aligned}
$

$
\begin{aligned}
-(f+n)f+B&=-f^2\\
B &= -f^2+(f+n)f\\   
B &= -f^2+f^2+nf\\   
B &= nf\\   
\end{aligned}
$

最终根据上述结论可构成矩阵

$$
\begin{bmatrix}
n & 0 & 0 & 0 \\
0 & n & 0 & 0 \\
0 & 0 & f+n & nf \\
0 & 0 & -1 & 0 \\
\end{bmatrix}
$$

## 最终投影矩阵

将正交投影和透视投影的矩阵相结合可得如下矩阵：

$
\begin{aligned}
&=\begin{bmatrix}
\frac{1}{w} & 0 & 0 & 0 \\
0 & \frac{1}{h} & 0 & 0 \\
0 & 0 & \frac{1}{f-n} & \frac{f}{f-n} \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
*\begin{bmatrix}
n & 0 & 0 & 0 \\
0 & n & 0 & 0 \\
0 & 0 & f+n & nf \\
0 & 0 & -1 & 0 \\
\end{bmatrix}\\
&=\begin{bmatrix}
\frac{n}{w} & 0 & 0 & 0 \\
0 & \frac{n}{h} & 0 & 0 \\
0 & 0 & \frac{f+n}{f-n}-\frac{f}{f-n} & \frac{nf}{f-n} \\
0 & 0 & -1 & 0 \\
\end{bmatrix}\\
&=\begin{bmatrix}
\frac{n}{w} & 0 & 0 & 0 \\
0 & \frac{n}{h} & 0 & 0 \\
0 & 0 & \frac{n}{f-n} & \frac{nf}{f-n} \\
0 & 0 & -1 & 0 \\
\end{bmatrix}
\end{aligned}
$

在透视投影中，Unity 不直接提供 h（半高），需要利用视野角度（fov）计算，利用三角函数可以轻松得出:

$
h = \tan(fov/2)*n\\
w = h * aspect
$

重新整理后可得最终透视投影矩阵：

$$
\begin{bmatrix}
\frac{1}{\tan(fov/2)*aspect} & 0 & 0 & 0 \\
0 & \frac{1}{\tan(fov/2)} & 0 & 0 \\
0 & 0 & \frac{near}{far-near} & \frac{near*far}{far-near} \\
0 & 0 & -1 & 0 \\
\end{bmatrix}
$$

## 深度分析

很多特效有时会利用 NDC 深度重建世界信息，因此还需要研究一下如何逆推深度信息。

### LinearEyeDepth

反推视图空间的非反转深度。可将此分成两个步骤，先执行逆投影函数得出视图空间的反转深度，然后再对该深度二次反转，以得到非反转深度。

即 $LinearEyeDepth(z) = -逆最终透视投影(z)$

根据之前的矩阵计算可得：

$
\begin{aligned}
最终透视投影 
&= \frac{(\frac{nz}{f-n}+\frac{nf}{f-n})}{-z}\\
&=\frac{n(z+f)}{z(n-f)}\\
\end{aligned}
$

再对最终透视投影求逆运算：

$
\begin{aligned}
z'&=\frac{n(z+f)}{z(n-f)} \\
z(n-f)z'&=nz+nf\\
z(n-f)z'-nz&=nf\\
z((n-f)z'-n)&=nf\\
z&=\frac{nf}{(n-f)z'-n}\\
\end{aligned}
$

$
逆最终透视投影=\frac{nf}{(n-f)z-n}
$

再反转结果并简化：

$
\begin{aligned}
&= -\frac{nf}{(n-f)z-n}\\
&= \frac{nf}{(f-n)z+n}\\
&= \frac{1}{\frac{f-n}{nf}z+\frac{1}{f}}\\
\end{aligned}
$

故最终结论为：

$$
LinearEyeDepth(z)=\frac{1}{\frac{f-n}{nf}z+\frac{1}{f}}
$$

### Linear01Depth

求解线性 0-1 深度（相机位置为 0，远平面为 1）。很容易想到，只需要对 $LinearEyeDepth$ 的结果除以远平面大小即可，即：

$
\begin{aligned}
&= \frac{1}{\frac{f-n}{nf}z+\frac{1}{f}} * \frac{1}{f}\\
&= \frac{1}{\frac{f-n}{n}z+1}\\
\end{aligned}
$

故最终结论为：

$$
Linear01Depth(z)=\frac{1}{\frac{f-n}{n}z+1}
$$

### Linear01DepthFromNear

求解线性 0-1 深度（近平面为 0，远平面为 1）。
