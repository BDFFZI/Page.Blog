---
categories:
  - 个人研究
  - 技术美术
  - 矩阵变换
abbrlink: 1097932020
---
# 【技术美术】TRS矩阵

TRS 矩阵是最常用的变换矩阵，其本质就是平移、旋转、缩放矩阵的复合，其复合顺序为：

$平移矩阵 * 旋转矩阵 * 缩放矩阵$

本文 TRS 矩阵以行业主流的左手坐标系为准，则：

## 平移矩阵（Translate）

$
\begin{bmatrix}
1&0&0&x\\
0&1&0&y\\
0&0&1&z\\
0&0&0&1
\end{bmatrix}
$

## 旋转矩阵（Rotate）

$
X轴旋转矩阵=
\begin{bmatrix}
1&0&0&\\
0&\cos(x)&-\sin(x)&\\
0&\sin(x)&\cos(x)
\end{bmatrix}
$

$
Y轴旋转矩阵=
\begin{bmatrix}
\cos(y)&0&\sin(y)&\\
0&1&0&\\
-\sin(y)&0&\cos(y)
\end{bmatrix}  
$

$
Z轴旋转矩阵=
\begin{bmatrix}
\cos(z)&-\sin(z)&0\\
\sin(z)&\cos(z)&0\\
0&0&1
\end{bmatrix}
$

若旋转矩阵要同时表示 3 轴旋转，就是通过对 x、y、z 旋转矩阵的复合得出的，如 Unity 中的复合顺序为：

$Y 轴旋转矩阵 * X 轴旋转矩阵 * Z 轴旋转矩阵$

推导复合顺序过程是这样的（四元数的 Euler 函数中也有说明）：

1. 在 Unity 中放一个方块。
2. 对其先后进行 x、y 旋转并逆序尝试。
3. 发现 y 旋转永远都是以世界 y 轴，而不是自身 y 轴（自身 y 轴会随 x 旋转变化）。
4. 说明 x 旋转不会改变 y 的旋转结果，反着说即 x 旋转已经先被应用到了顶点。
5. 故旋转矩阵应用顺序一定是先 X 后 Y，Z 同理推出。

$
复合旋转矩阵\\
=\begin{bmatrix}
\cos(y)&0&\sin(y)&\\
0&1&0&\\
-\sin(y)&0&\cos(y)
\end{bmatrix}  
\begin{bmatrix}
1&0&0&\\
0&\cos(x)&-\sin(x)&\\
0&\sin(x)&\cos(x)
\end{bmatrix}
\begin{bmatrix}
\cos(z)&-\sin(z)&0\\
\sin(z)&\cos(z)&0\\
0&0&1
\end{bmatrix}\\
=\begin{bmatrix}
\cos(y)&0&\sin(y)&\\
0&1&0&\\
-\sin(y)&0&\cos(y)
\end{bmatrix}  
\begin{bmatrix}
\cos(z)&-\sin(z)&0&\\
\cos(x)\sin(z)&\cos(x)\cos(z)&-\sin(x)&\\
\sin(x)\sin(z)&\sin(x)\cos(z)&\cos(x)
\end{bmatrix}\\
=\begin{bmatrix}
\cos(y)\cos(z)+\sin(y)\sin(x)\sin(z)&-\cos(y)\sin(z)+\sin(y)\sin(x)\cos(z)&\sin(y)\cos(x)&\\
\cos(x)\sin(z)&\cos(x)\cos(z)&-\sin(x)&\\
-\sin(y)\cos(z)+\cos(y)\sin(x)\sin(z)&\sin(y)\sin(z)+\cos(y)\sin(x)\cos(z)&\cos(y)\cos(x)
\end{bmatrix}\\
$

## 缩放矩阵（Scale）

$
\begin{bmatrix}
x&0&0\\
0&y&0\\
0&0&z
\end{bmatrix}
$

## 旋转矩阵求逆

观察旋转矩阵中各分量旋转矩阵，可以发现：

以 z 旋转矩阵为例（下方$C_n$表示矩阵中的列向量）：

$\begin{bmatrix}
\cos(z)&-\sin(z)&0\\
\sin(z)&\cos(z)&0\\
0&0&1
\end{bmatrix}$

- $C_1\cdot C_2=\cos(z)*-\sin(z)+\sin(z)*\cos(z)+0*0=0$
- $C_1\cdot C_3=\cos(z)*0+\sin(z)*0+0*1=0$
- $C_2\cdot C_3=-\sin(z)*0+\cos(z)*0+0*1=0$
- $|C_1|=\sqrt{\cos^2(z)+\sin^2(z)+0^2}=1$
- $|C_2|=\sqrt{(-\sin(z))^2+\cos^2(z)+0^2}=1$
- $|C_3|=\sqrt{0^2+0^2+1^2}=1$

以此方式可以推断出各分量旋转矩阵都是正交矩阵，而正交矩阵相乘还是正交矩阵，故最终的复合旋转矩阵也是正交矩阵。

根据正交矩阵的性质，其逆矩阵就非常好求了：

$$
Rotation^{-1}=Rotation^T
$$

## TRS 矩阵的拆解

从 TRS 矩阵中还原 T,R,S 的信息。

### 还原 T

还原 T 最简单，因为 R,S 都是 3x3 矩阵，第四列就是 T 的信息。

$$
\vec{t} = [a14,a24,a34]（其中 A = TRS）
$$

### 还原 S

将 4x4 矩阵强制转为 3x3 矩阵后，余下的就是 RS 矩阵。利用 R 是正交矩阵的特性（在“旋转矩阵求逆”章节中得出），将 RS 乘上 RS 的转置矩阵，可以得到：

$
\begin{aligned}
(RS)^TRS
&= S^TR^TRS\\
&=S^TR^{-1}RS （正交矩阵的逆矩阵等于转置矩阵）\\
&=S^TS\\
&=S^2（对角矩阵的转置等于自身）
\end{aligned}
$

这样就可以非常容易的求出 S 的信息了：

$$\vec{s}=[a11,a22,a33] （其中 A = \sqrt{(RS)^TRS},RS=TRS_{3*3}）$$

### 还原 R

<https://blog.csdn.net/weixin_39675633/article/details/103434557>

有了 S 后可以很容易的获取到 R 矩阵：

$R = RSS^{-1}$

接着观察 R 矩阵的公式，可以发现利用 $a_{23}$ 可以很轻松的求出 $\cos(x)$ 的值，而其他项中有很多用该值组成的多项式：

$
\begin{bmatrix}
\cos(y)\cos(z)+\sin(y)\sin(x)\sin(z)&-\cos(y)\sin(z)+\sin(y)\sin(x)\cos(z)&\sin(y)\cos(x)&\\
\cos(x)\sin(z)&\cos(x)\cos(z)&-\sin(x)&\\
-\sin(y)\cos(x)+\cos(y)\sin(x)\sin(z)&\sin(y)\sin(z)+\cos(y)\sin(x)\cos(z)&\cos(y)\cos(x)
\end{bmatrix}\\
$

但由于 $\cos(x)$ 可能等于 0，且正常的反三角函数无法完全逆推原角度，故此 $\cos(x)$ 不一定准确。所以需要对这些注意事项特殊处理，不能简单粗暴的直接用 $a_{23}$ 推导所有角度：

1. 当 $|\sin(x)| \neq 1$ 时

    - $z=\operatorname{atan2}(a_{21},a_{22})$
    - $y=\operatorname{atan2}(a_{13},a_{33})$

    有了 y 和 z 的角度后我们便可以通过带入求解真正的 $\cos(x)$，但依然要注意值为 0 的问题，好在 cos 和 sin 同时只可能有一个为 0，故只需要简单的判断即可：

    1. 若 $a_{21} \neq 0$，$\cos(x) = a_{21}/\sin(z)$
    2. 若 $a_{21} = 0$，$\cos(x) = a_{22}/\cos(z)$

    有了未损失原角度信息的 $\cos(x)$ 后，x 就很好求出了：

    - $x=\operatorname{atan2}(-a_{23},\cos(x))$

2. 当 $|\sin(x)| = 1$ 时，此时镜头朝正上或正下。

    由于 $\cos(x)$ 的值为 0，因此无法使用第 1 点的计算方法了，但因为 $\sin(x) = \pm 1$（$\sin(x) = -a_{23}$），所以其他的项代数式变的可用。

    1. 当 $\sin(x) > 0$ 时，此时镜头朝下，z 旋转轴与 y 旋转轴反向。

        - $a_{11}=\cos(y)\cos(z)+\sin(y)\sin(z)=\cos(y-z)$
        - $a_{12}=\sin(y)\cos(z)-\cos(y)\sin(z)=\sin(y-z)$

        于是 $y-z=\operatorname{atan2}(a_{12},a_{11})$

    2. 当 $\sin(x) < 1$ 时，此时镜头朝上，z 旋转轴与 y 旋转轴同向。

        - $a_{11}=\cos(y)\cos(z)-\sin(y)\sin(z)=\cos(y+z)$
        - $a_{12}=-(\sin(y)\cos(z)+\cos(y)\sin(z))=-\sin(y+z)$

        于是 $y+z=\operatorname{atan2}(-a_{12},a_{11})$

    可以发现无论怎么求，只能同时求到 y 和 z，因为此时发生了万向锁，y 和 z 的旋转效果变的相关了。由于相关，导致 y 和 z 可以有多种取值组合，因此这种情况下无法求解原角度了。但为了返回结果，我们可以直接随便选一种组合，例如假定 z 始终为 0。于是该情况的 R 信息也可以确定了：

    1. 当 $\sin(x) > 0$ 时

       - $z=0$（z 旋转全部转移到 y 上）
       - $y=\operatorname{atan2}(a_{12},a_{11})$
       - $x=\pi/2$（镜头朝正下方）

    1. 当 $\sin(x) < 0$ 时

       - $z=0$（z 旋转全部转移到 y 上）
       - $y=\operatorname{atan2}(-a_{12},a_{11})$
       - $x=-\pi/2$（镜头朝正下方）

可以看出部分情况下旋转是无法被真正逆推回来的，这也解释了为什么镜头旋转到世界正上方或正下方时无法继续沿镜头上下旋转的原因。因此旋转物体时要保留原始的旋转值，不能完全依靠矩阵。
