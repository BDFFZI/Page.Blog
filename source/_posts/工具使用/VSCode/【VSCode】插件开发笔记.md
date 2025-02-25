# VSCode 插件开发笔记

## VSCode 的插件实质

VSCode 在设计之初就考虑了扩展性，甚至很多 VSCode 的核心功能也是基于扩展的方式编写的，并且使用的扩展 API 也是一样的。

VSCode 的源代码基本都是用 TS 编写的，所以他的扩展编写也基本一样，使用 TS 或 JS 开发。

VSCode 插件其实就是一个 node.js 包，插件的各种配置信息也都存在包中的 package.json 文件中。

VSCode 安装的插件都存放在了用户目录的“.vscode\extensions”文件夹中，可以直接把自己的插件放进去来实现手动安装。

## 开发环境搭建

### 配置项目

1. 确保安装了 Node.js 和 Git。
2. 使用 npm 安装 Yeoman。
3. 利用 Yeoman 创建项目。

```bash
npm install --global yo generator-code #安装yeoman
yo code #创建项目
```

### 运行调试

项目默认配置了 HelloWorld 程序，直接 F5 运行即可。这样会自动打开一个新的 VSCode 编辑器并将当前开发的插件安装进去。

#### 注意：

- 插件对 VScode 的版本是有需求的，可以通过 package.json 中依赖的 vscode 包版本看出，如果版本不兼容就会导致插件安装失败，但这个报错调试时是不会显示的，要人为检查一下版本信息。

## 插件集成原理

### 代码集成

当插件被装入 VSCode 并激活时，VSCode 会主动调用插件中的一些事件函数。接着我们要在这些函数中通过 VSCode 提供的方法，向 VSCode 注册我们的功能。

```ts
import * as vscode from "vscode"; // 导入vscode的扩展api

// 插件被激活时，vscode将调用该函数。
export function activate(context: vscode.ExtensionContext) {
  //此时向vscode注册功能，比如此处注册了一条命令
  let disposable = vscode.commands.registerCommand(<id>, <event>);
  context.subscriptions.push(disposable);
}

// 插件被停用时，vscode将调用该函数。
export function deactivate() {}
```

### 扩展清单

插件向 VSCode 注册的所有功能都必须先在扩展清单中声明，扩展清单其实就是 package.json 文件，但我们需要在里面填充一些 VSCode 需要的东西。

- name：扩展 ID，用于唯一表示插件。
- activationEvents：激活事件，说明在哪些时候自动激活插件。
- main：插件的入口脚本。
- contributes：贡献点，要为 VSCode 增加哪些功能。
