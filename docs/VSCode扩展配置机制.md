# VS Code 扩展配置机制（以 LeetCode 插件为例）

> 目的：搞清楚扩展的"设置项"是怎么来的、去哪里找、怎么写、怎么生效，方便以后自定义任意插件的配置。
> 文中所有 LeetCode 的声明都取自本机实际安装的 `leetcode.vscode-leetcode-0.18.4`。

---

## 一、总览：设置从哪来

每个 VS Code 扩展的 `package.json` 里有一个 `contributes` 字段，声明它"贡献"给 VS Code 的所有东西（命令、菜单、视图、快捷键、**设置**……）。

设置对应的贡献点是 **`contributes.configuration`**。写在这里的每个属性，VS Code 会自动：

- 显示在**设置界面（UI）**里；
- 允许用户写进 **`settings.json`**；
- 让扩展代码通过 API **读出来**。

**一句话：`contributes.configuration` = 扩展作者递给用户的"可配置项清单"。**

---

## 二、找到某个扩展的配置声明

扩展安装后位于（Windows）：

```
%USERPROFILE%\.vscode\extensions\<publisher>.<name>-<version>\
```

例如 LeetCode 插件：

```
C:\Users\26259\.vscode\extensions\leetcode.vscode-leetcode-0.18.4\
```

打开它的 `package.json` → `contributes.configuration`，就是该扩展的全部设置声明。

> - 扩展随包**只发布编译后的 JS**（`out/` 目录），原始源码在 GitHub 仓库里；
> - 但 `package.json` 和 `README.md` 是原样打包的，想快速了解设置，读这两个最快。
> - extensions 目录可能被 `--extensions-dir` 重定向或做成符号链接（本机实际指向 `E:\MyVsCodeExtention`），以实际解析到的路径为准。

---

## 三、`configuration` 的结构

`contributes.configuration` 可以是**一个对象**，也可以是**对象数组**（按分组，每组一个 `title`）。LeetCode 用的是数组：

```jsonc
"contributes": {
  "configuration": [
    {
      "title": "LeetCode",          // 设置 UI 里的分组名
      "properties": {
        // 每一个设置项：key 就是设置名，value 是它的声明
        "leetcode.hideSolved": { ... },
        "leetcode.endpoint":   { ... },
        "leetcode.filePath":   { ... }
      }
    }
  ]
}
```

---

## 四、一个设置项有哪些字段

以 LeetCode 的真实声明为例：

```jsonc
"leetcode.endpoint": {
  "type": "string",                     // 值类型
  "default": "leetcode",                // 默认值
  "scope": "application",               // 作用域（见第五节）
  "enum": [ "leetcode", "leetcode-cn" ],// 限定取值 → UI 变下拉框
  "description": "Endpoint of the user account."
},
"leetcode.workspaceFolder": {
  "type": "string",
  "scope": "application",
  "description": "The path of the workspace folder to store the problem files.",
  "default": ""
}
```

常用字段：

| 字段 | 作用 |
|------|------|
| `type` | `string` / `number` / `boolean` / `object` / `array` / `null`，决定 UI 控件与校验 |
| `default` | 默认值，用户没配时用它 |
| `description` / `markdownDescription` | UI 里的说明文字（后者支持 Markdown） |
| `enum` / `enumDescriptions` | 限定取值；UI 变成下拉框，并可为每个取值配一句说明 |
| `scope` | 作用域，决定能否被工作区/文件夹覆盖（见第五节） |
| `properties` | 仅当 `type: "object"`，描述嵌套结构（如 `leetcode.filePath`） |
| `items` | 仅当 `type: "array"`，描述数组元素 |
| `order` / `tags` / `deprecationMessage` / `ignoreSync` | 排序；标记（如 `"experimental"`、`"usesOnlineServices"`）；废弃提示；是否参与 Settings Sync |

---

## 五、`scope`：决定"能不能被工作区覆盖"

| scope | 含义 |
|-------|------|
| `application` | 全局；**只能在用户设置里配**，工作区/文件夹不可覆盖 |
| `machine` | 机器相关；只在用户设置或远程机器设置里配，不参与同步 |
| `machine-overridable` | 机器相关，但可被工作区/文件夹覆盖 |
| `window`（**省略时的默认值**） | 窗口级；用户 / 工作区 / 远程设置都能配 |
| `resource` | 资源级；可针对具体文件/文件夹配置 |
| `language-overridable` | 资源级，且可**按语言**覆盖（`"[python]": { ... }`） |

> ⚠️ 注意 `application`：`leetcode.endpoint`、`leetcode.workspaceFolder`、`leetcode.filePath` **全都是 `application`**——意味着它们**只能写进用户设置**。放进项目 `.vscode/settings.json` 会被 VS Code 直接忽略。这就是为什么自定义 `filePath` 要写在用户全局设置里。

---

## 六、设置的分层与优先级

VS Code 设置是分层的，越"具体"的越优先：

```
默认值  <  用户(User)  <  远程(Remote)  <  工作区(Workspace)  <  文件夹(Folder)
```

| 层 | 位置 |
|----|------|
| 用户（全局） | `%APPDATA%\Code\User\settings.json` |
| 工作区 | `<项目>\.vscode\settings.json` |
| 远程（SSH/WSL/容器） | 远端 `<项目>/.vscode/settings.json`，或远端机器级 `~/.vscode-server/data/Machine/settings.json` |
| 文件夹（多根工作区） | 每个文件夹下的 `.vscode\settings.json` |

（`application` scope 的设置不参与这套覆盖——它被钉死在用户层。）

---

## 七、扩展代码怎么读这些设置

扩展通过 `workspace.getConfiguration` 读取：

```ts
import * as vscode from "vscode";

const cfg = vscode.workspace.getConfiguration("leetcode"); // 传入设置名前缀
const endpoint = cfg.get<string>("endpoint");              // 读 leetcode.endpoint
const wpFolder = cfg.get<string>("workspaceFolder");       // 读 leetcode.workspaceFolder

// object 类型的嵌套项，支持点号路径
const folder = cfg.get<string>("filePath.cpp.folder");
```

- `getConfiguration()` 不传前缀会返回整棵配置树；
- 设置变化可用 `workspace.onDidChangeConfiguration` 监听；
- `get` 的第二个参数可以给一个兜底默认值：`cfg.get("endpoint", "leetcode")`。

---

## 八、实战：`leetcode.filePath`（object 类型 + 占位符）

这是本仓库实际用到的配置，也是"自定义文件落盘路径"的正规入口。

### 8.1 声明（节选自 package.json 第 401 行起）

```jsonc
"leetcode.filePath": {
  "type": "object",
  "scope": "application",
  "description": "The output folder and filename to save the problem files.",
  "properties": {
    "default": { "type": "object", "properties": { "folder": {...}, "filename": {...} },
                 "required": ["folder", "filename"] },
    "cpp":     { "type": "object", "properties": { "folder": {...}, "filename": {...} },
                 "minProperties": 1 },
    "csharp":  { "type": "object", "properties": { "folder": {...}, "filename": {...} },
                 "minProperties": 1 }
    // ... 其它语言 key
  },
  "additionalProperties": { "type": "object", "properties": { "folder": {...}, "filename": {...} } },
  "default": { "default": { "folder": "", "filename": "${id}.${kebab-case-name}.${ext}" } }
}
```

### 8.2 语义

- 按 **语言 key** 分组，每组给 `folder`（子目录）+ `filename`（文件名模板）；
- **最终路径** = `path.join(workspaceFolder, folder, filename)`（源码 `out/src/commands/show.js`）；
- 某语言没配 → 回退到 `filePath.default` → 再回退到内置默认（根目录 + `${id}.${kebab-case-name}.${ext}`）。

### 8.3 支持的语言 key

显式列出：`bash` `c` `cpp` `csharp` `golang` `java` `javascript` `kotlin` `mysql` `php` `python` `python3` `ruby` `rust` `scala` `swift`（另有 `default`）。
因为有 `additionalProperties`，**任意 key 都合法**——所以 `typescript`（不在列表里）也能用。

### 8.4 文件名占位符

| 占位符 | 含义 |
|--------|------|
| `${id}` | 题号 |
| `${name}` | 题目原名（力扣上即中文标题） |
| `${camelCaseName}` | 驼峰 |
| `${PascalCaseName}` | 大驼峰 |
| `${kebab-case-name}` | 短横线（也写作 `${kebabCaseName}`） |
| `${snake_case_name}` | 下划线（也写作 `${snakeCaseName}`） |
| `${ext}` | 扩展名 |
| `${language}` | 语言 key |
| `${difficulty}` | 难度（小写：easy / medium / hard） |
| `${tag}` / `${company}` | 标签 / 公司（有多个时会弹窗让你选） |

> 占位符**大小写不敏感**（源码先 `toLowerCase()` 再匹配）；写错会报 `The config 'xxx' is not supported.`

### 8.5 本仓库的实际配置

```jsonc
"leetcode.filePath": {
  "cpp":        { "folder": "cpp",        "filename": "${id}.${name}.${ext}" },
  "csharp":     { "folder": "csharp",     "filename": "${id}.${name}.${ext}" },
  "typescript": { "folder": "typescript", "filename": "${id}.${name}.${ext}" }
}
```

配合 `"leetcode.workspaceFolder": "d:\\leetcode"`，新题会自动落到 `d:\leetcode\cpp\`、`d:\leetcode\csharp\`、`d:\leetcode\typescript\`。

---

## 九、拿到任意扩展配置项的"四步法"

1. **设置 UI 搜**：`Ctrl+Shift+P` → `Preferences: Open Settings (UI)`，搜扩展名或关键字，能改就直接改（UI 背后写的还是 `settings.json`）。
2. **看 README**：扩展的 README 常有设置表格（LeetCode 的就有，里面还链到专门的 wiki）。
3. **看 `package.json`**：`contributes.configuration` → `properties`，**最权威**，能看到类型、默认值、scope、枚举。
4. **看源码怎么用**：在 GitHub 仓库（或本地 `out/` 的编译结果）里搜设置名，看它读了哪个键、怎么兜底、占位符/路径怎么拼。

---

## 十、进阶机制（顺带认识）

- **`contributes.configurationDefaults`**：扩展可以**预置默认值**，甚至为别的扩展的设置设默认，还支持按语言：

  ```jsonc
  "configurationDefaults": {
    "[python]": { "editor.insertSpaces": true }
  }
  ```

- **按语言覆盖**：任何 `language-overridable` 的设置都能写成 `"[languageId]": { ... }` 的形式。
- **`restricted`**：标记"需要工作区信任（Workspace Trust）才生效"，用于安全敏感设置。
- **`ignoreSync`**：声明该设置**不参与** Settings Sync。

---

## 十一、写给未来的自己：自己写扩展时怎么暴露配置项

最小示例（`package.json`）：

```jsonc
{
  "contributes": {
    "configuration": {
      "title": "My Extension",
      "properties": {
        "myext.enabled": {
          "type": "boolean",
          "default": true,
          "scope": "window",
          "description": "Turn the feature on/off."
        },
        "myext.pattern": {
          "type": "string",
          "default": "${file}",
          "markdownDescription": "输出路径模板，支持 `${file}` 等占位符。"
        }
      }
    }
  }
}
```

代码里读：

```ts
const enabled = vscode.workspace.getConfiguration("myext").get<boolean>("enabled", true);
```

要点：

- 设置名以**扩展标识前缀**开头（`myext.`），避免和其它扩展冲突；
- 一定给 `default`，代码里 `get` 时再给一次兜底；
- 需要时用 `enum` 限定取值、用 `markdownDescription` 写富文本说明；
- object 类型适合"按维度分组"的配置（如按语言分目录），再在源码里遍历消费。

---

## 参考

- VS Code 官方贡献点文档：[contributes.configuration](https://code.visualstudio.com/api/references/contribution-points#contributes.configuration)
- 官方 API：[WorkspaceConfiguration](https://code.visualstudio.com/api/references/vscode-api#WorkspaceConfiguration)
- LeetCode 插件仓库：[LeetCode-OpenSource/vscode-leetcode](https://github.com/LeetCode-OpenSource/vscode-leetcode)
- 该插件自定义文件路径的 wiki：[Customize the Relative Folder and the File Name of the Problem File](https://github.com/LeetCode-OpenSource/vscode-leetcode/wiki/Customize-the-Relative-Folder-and-the-File-Name-of-the-Problem-File)
