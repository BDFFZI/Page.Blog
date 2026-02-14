---
categories:
  - 框架使用
  - Unity
  - Unity开发规范
abbrlink: 4118079887
---
# Unity编码规范

## 结构规范

> 让最常用的成员排在前列，让最相关的成员排在一起，便于快速索引有效信息和提示成员功能。

确定类成员的布局方式

1. 按访问修饰符排序：公开->保护->私有[SerializeField]->私有
2. 按是否为静态排序：静态->非静态
3. 按具体类型排序：数据定义->事件->字段->属性->函数
4. 按逻辑关系排序：Unity事件、成对函数等

示例：

```csharp
public class Door
{
    public enum State
    {
        Closing,
        Opening,
        Opened
    }

    public event Action Opened;
    public event Action Closed;

    public State CurrentState => currentState;
    public Func<bool> OpenCondition => openCondition;

    public Task OpenAsync() { throw new NotImplementedException(); }
    public Task CloseAsync() { throw new NotImplementedException(); }

    [SerializeField] AudioSource audioSource;
    [SerializeField] AudioClip openSound;
    [SerializeField] AudioClip closeSound;

    State currentState;
    Func<bool> openCondition;

    void Awake()
    {
        throw new NotImplementedException(); //初始化
    }

    void PlayOpenAnimation() { throw new NotImplementedException(); }
    void PlayCloseAnimation() { throw new NotImplementedException(); }
}

```

## 命名规范

> 增强可读性，避免形式主义，追求美观简洁。

- 统一采用英语命名，不要用汉语拼音。

```csharp
ShiZi //错误（必须人工解读，但没有声调和上下文，难以正确解读。石子、狮子、柿子？）
Persimmon //正确（放入翻译软件中即可得为：柿子）
```

- 私有成员全部采用“驼峰命名法”
- 非私有成员全部采用“帕斯卡命名法”（首字母大写的驼峰命名法）
- 确保利用驼峰分割名称后，可从翻译软件中得到对应中文。

```csharp
public State CurrentStateOfDoor { get;private set}; //正确（公开成员）
State currentStateOfDoor; //正确（私有成员）
//按驼峰分割后“Current State Of Door”可在翻译软件中解读为“门的现状”，正确。
```

- 名称怕短不怕长，除非是 `html` 这类事实标准，否则不要缩写！
- 进行缩写时，对缩写单次采用首字母大写，其余小写的形式。
- 不要使用纯简单的字母、数字等无意义名称，名称必须能自注释。

```csharp
string AssetBundleName; //正确
string AbName; //错误（Ab不是标准缩写，也无法被翻译软件解读）
string DbName; //正确（Db是标准缩写，可以被翻译软件解读为“数据库名称”）
int A,B,C; //错误（完全无法从名称中解读出该成员的含义作用）
for(int i)//正确（在当前上下文属于事实标准写法，i为index的缩写）
```

- 不要故意使用匈牙利命名法（添加多余的前后缀），现代IDE能区分类型信息。（单纯是为了避免重名等需求，不受影响）
- 不要使用下划线，有了驼峰命名法，没有下划线也能分割内容。

```csharp
string userName;
Text userNameText; //错误（不用写明类型，如果这成了规范，会成为一种无意义的负担）
Text userNameUI;//正确（不仅也避免了和userName重名，而且解释了其作用）
Text _user_Name;//错误（同样是无意义的负担，而且看起来很不规整）
```

- 针对具体类型有一些专门的命名要求

| 类型     | 描述                                                                                | 示例                        |
| -------- | ----------------------------------------------------------------------------------- | --------------------------- |
| 接口     | 对应类名添加`I`前缀。                                                                           | `IInterface`                |
| 泛型     | 对应类型添加`T`前缀。                                                                           | `TGeneric`                  |
| 函数     | 表示动作，采用主动语态。                                                                        | `OpenDoor`                  |
| 事件     | 表示动作，采用被动语态。 <br/> 触发在对应函数开头或期间使用进行时，触发在函数结束使用过去时。         | `DoorOpening`、`DoorOpened` |
| 事件函数（用于实现事件的函数） | 表示动作，采用被动语态，在对应事件命名的基础上添加`on`前缀。                    | `OnDoorOpening`、`OnDoorOpened`|
| 字段     | 名词和可选的形容词，采用被动语态。由于字段不允许公开，所以永远为小驼峰命名法。                       | `isDoorOpening`             |
| 属性     | 通常与字段对应，故与对应字段同名。且通常属性是公开类型，故一般会采用大写开头。                       | `IsDoorOpening`             |

- 针对具体功能也有些专门的命名要求（这些要求不分类型，字段名、类名等都要遵守）：

|功能|要求|
|---|---|
|负责UI相关的元素（对应MVC中的V）|+UI|
|负责驱动模块，推进游戏逻辑的元素（对应MVC中的C）|+Controller|
|用于实例化的预制体|+Prefab|

## 实现规范

> 使用优秀的代码设计策略，增加程序的可维护性。

### 功能性要求

> 这些要求会影响你的功能实现，但有助于不同功能间的开发协作。

- 非常量字段永远不允许公开，应使用属性代替。

```csharp
public int Value;//错误（字段操作无法监控，外部程序可以随意操控）
public int Value {get; private set;};//正确（属性可以验证操作过程，避免错误行为）
```

- 基于`[SerializeField]`（或`[RequireComponent]`）实现依赖注入，而非不受控的依赖获取。

```csharp
class {
    public void Play(){
        AudioSource audioSource = GetComponent<AudioSource>()
        audioSource.Play();
    }
} //错误（无法确保外部环境正确，也无法提醒用户配置）

class {
    public void Play(){
        audioSource.Play();
    }
    [SerializeField] AudioSource audioSource;
} //正确（外部用户将会在面板上看到依赖的对象的槽位）

[RequireComponent(typeof(AudioSource))]
class {
    public void Play(){
        AudioSource audioSource = GetComponent<AudioSource>()
        audioSource.Play();
    }
} //正确（附着的物体将会被强制添加上依赖组件）
```

- 在`Awake`（或`OnEnable`）阶段完成功能模块初始化，存在依赖时配合`[DefaultExecutionOrder]`控制。
- 不要基于构造函数、析构函数等C#事件处理数据，应全部在Unity事件中处理，否则部分Unity功能会无法使用。
- 尽可能禁用编辑器选项 `Reload Domain` 和 `Reload Scene`，从而加快编辑器运行速度，提高开发迭代效率。

```csharp
[DefaultExecutionOrder(-10)]//正确（SeedGenerator的初始化时机被显式指定，便于其他模块使用）
class SeedGenerator {
    public int GetSeed() => currentSeed;
    const int defaultSeed = 1;
    const int lastSeed = 9;
    int currentSeed = defaultSeed;//错误（不允许依赖构造函数等初始化，部分情况下不能正常触发）
    void Awake(){
        //兜底初始化
        currentSeed = defaultSeed;//正确（在Awake阶段时确保模块被初始化（可用））
    }
    void OnEnable(){
        //完全初始化
        currentSeed = lastSeed;//正确（基于Unity事件处理数据）
    }
    void OnDisable(){
        //资源回收
        currentSeed = -1;//正确（基于Unity事件处理数据）
    }
}
class Random {
    [SerializeField] A a;
    int seed;
    void Awake(){
        //依赖A的初始化
        seed = a.GetSeed();//正确（由顺序被确定，此时a已经执行OnEnable（或Awake），可以使用）
    }
}
```

### 可读性要求

> 这些要求不会影响功能和程序设计，但可以使代码美观，便于读写。

- 省略默认代码（如私有访问修饰符），让系统自动提供，从而简化代码减少阅读量。

```csharp
//错误（显式写明修饰符较冗长且需要专门阅读修饰符的单词内容）
private int value; 
public int Value {public get;public set;}
//正确（非常简约，大幅减少代码量，且不需要专门阅读修饰符）
int value;
public int Value {get;set;}
```

- 区分“取反”和“判否”，在判否时显式写明为与否定的比较。

```csharp
if(isDoorOpened == false)//正确
if(isDoorOpened)//正确（省略默认代码，减少阅读量）
if(!isDoorOpened)//错误（用取反符号判断容易看漏，而且无法显式表达出判否的意图）
bool isDoorClosing = !isDoorOpened;//正确（按取反的意图使用取反符号）
```

- 避免使用 var，应显式写明类型信息。

```csharp
var a = GetValue(); //错误（必须通过二次查询才能获取类型信息，非常影响阅读效率）
int a = GetValue(); //正确（一眼就可以看出类型信息，非常直观，清晰的类型名易于理解代码）
List<int> b = new();//正确（最新的语法糖，保留类型信息同时简化代码，比var好）
```

- 当有多个if判断时，使用基于级联的布局方式，而非嵌套。

```csharp
void Func(){
    if(){
        if(){
            if(){
            }
        }
    }
}//错误（嵌套布局，代码块重合，不满足开闭原则，可维护性差）
void Func(){
    if(){}
    return;
    if(){}
    return;
    if(){}
    return;
}//正确（各代码块相互独立，不容易误操作，且功能独立易于理解）
```

- 代码中的任何元素（变量函数等）应保持最小作用域，仅放在确实要用的环境中。

```csharp
//错误（PreProcess仅在Process使用，但放在外部作用域反而会成为读者的干扰项（误以为是有用的独立函数））
void PreProcess(){};
//错误（同 PreProcess，而且其命名“i”在定义环境下无法向读者解释含义，存在命名错误）
int i;
void Process(){
    PreProcess();
    for(i){}
}

void Process(){
    //正确（只有阅读Process的读者，才需要知道PreProcess和processStep的实际功能）
    void PreProcess(){};
    int processStep;

    PreProcess();
    for(processStep){}
}
```

- 优先使用新式的C#语法糖和功能，减少代码量，统一实现方式。

```text
//示意例子：
Action -> await / async
if(a is null) -> ?.
a == null ? a:b => ??
for/foreach -> Linq
class{} -> var=new {}
int i,j,k;k=j;j=i;i=k -> (i,j) = (j,i)
.....等等
```

注意：部分新式写法存在性能和兼容性问题，例如：Linq 可能产生 GC；空条件运算符对Unity对象无效。故要根据实际情况选择性使用，甚至在性能密集场合，手写的排序比 List.Sort 更高效（因为Sort也有GC）。

如果偷懒不想阅读 C# 文档，建议使用一下 Rider IDE，远比 VS 等在代码提示和检查上智能的多（毕竟以前都没有免费版），用一遍可以学到很多知识。

### 设计性要求

> 这些要求影响你如何设计你的代码实现，能大幅增加代码的健壮性，减少问题概率且易于调整扩展。

#### 检错方法

- **避免无法预估的if判断**

如果你的代码实现里用到很多if判断，且你无法确定他们的最大数量，可以预见性的，他们未来还会继续增加。那你不应该使用if实现，而是改用委托，由外部调用者实现原本的if代码。

- **杜绝复制粘贴的编码方式**

如果你写代码时出现了成段的复制粘贴行为，那说明你的代码需要优化。你应该将你复制的代码进行封装，考虑改用可以共用的函数实现，这样后期修改才不会出现“一个调整，到处要改”的情况。

- **保持树型代码引用关系**

如果你有两个不相关的功能 A、B，将A的代码删除后，B却报错了，那你的代码就存在依赖问题。不同功能间的依赖应该是树状的，有明显的层级隔离，一个枝杈不可能影响到另一个枝杈。如果难以理解，那就想这么一个需求，你的每一个功能模块都应该可以做到随时移植到其他项目，且仅依赖必要的其他功能代码。

#### 开发方法

- **基本原则：通过“高内聚低耦合”实现“模块化”**

很多人把自己的功能称为模块，但事实上他们并没有实现模块化。“模块化”在《软件工程》中有专门的定义，逻辑上的功能独立并不能说明你的代码是模块化的。无论多复杂的编码规范技巧，本质都是为了实现“模块化”，实现“高内聚低耦合”，包括我们上面已经提到的几点方案。

- **开发技巧：设计模式**

市面上有很多类似的技巧和规范教我们如何高效管理项目，例如《设计模式》就是其中一种，建议看看：[【软件开发】设计模式个人解读](https://bdffzi.github.io/blog/posts/2553791405)

- **开发框架：MVC框架**

如果你的代码实现了模块化，那必然就会自动使用上MVC框架，因为MVC就代表了三个不同的模块：

- M（模型）：核心功能，类似于API的存在。
- V（视图）：用户界面，与M分离，用于想用户呈现特定的数据交互形式。
- C（控制器）：驱动游戏逻辑，将各个模块桥接，激活。

如果偷懒，MV可以合成一个模块（但那样不利于自动化测试，因为V只能人工测试），但C一定是分离出来的。一个应用说到底就是由一堆功能模块和驱动模块构成的。

- **避免形式主义，不要写没有提到的功能。**

很多首次接触代码设计规范的人，会陷入一种形式主义，一开始就想把代码设计的尽善尽美，很小的一个功能，却花大量的事件用于构建框架。但通常的结果是，这些复杂的框架不仅没起到效果，还严重影响维护和新功能的开发。代码设计应该是动态调整的，不会为未提出的需求做过多准备，但一旦有需求也能迅速改进适应。

> 没人是先知，人总是活在当下，程序也是一样。项目刚开始时，你可以用一些if，可以少量复制粘贴，可以不用父类抽象，这没问题，因为这样写最简单省时间，只要功能没有改动，这就是当前状态最易于理解的代码。直到功能开始复杂，你就该意识到需要换个策略应对变化了。用上那些你学到的设计技巧，在一次一次迭代中将程序不断调整完善，因地制宜，一砖一瓦，稳稳的做出每个当下的最合适的项目形状。
