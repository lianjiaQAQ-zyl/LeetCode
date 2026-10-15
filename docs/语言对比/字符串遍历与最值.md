# Python / TypeScript / C# 字符串遍历与 max 函数对比

> 本文档总结三种语言中「遍历字符串」的常用方式，以及「求最大值」相关函数。

---

## 一、遍历字符串

### Python

```python
s = "hello"

# 1. for 循环直接遍历（最常用，推荐）
for ch in s:
    print(ch)

# 2. 下标遍历
for i in range(len(s)):
    print(s[i])

# 3. 同时取下标和字符
for i, ch in enumerate(s):
    print(i, ch)

# 4. 转成列表遍历
for ch in list(s):
    print(ch)
```

**Unicode 说明**：Python 3 的字符串按 **Unicode 码点** 存储，`for ch in s` 和 `s[i]` 都能正确处理 emoji 等字符，不会出现拆成半截的问题。

```python
s = "a😀b"
print(len(s))        # 3（按码点计数）
for ch in s:
    print(ch)        # a, 😀, b ✅
```

---

### TypeScript

```typescript
const s = "hello";

// 1. for...of（推荐，按码点遍历）
for (const ch of s) {
  console.log(ch);
}

// 2. 传统下标 for 循环
for (let i = 0; i < s.length; i++) {
  console.log(s[i]);      // 或 s.charAt(i)
}

// 3. forEach（先转成字符数组）
Array.from(s).forEach((ch) => console.log(ch));
// 或 [...s].forEach((ch) => console.log(ch));
// 或 s.split("").forEach((ch) => console.log(ch));

// 4. for...in（遍历下标，不推荐用于取值）
for (const i in s) {
  console.log(i, s[i]); // i 是字符串下标
}
```

**Unicode 说明**：`string.length` 和 `s[i]` 按 **UTF-16 码元** 计数，emoji 会被拆成两个。用 `for...of`、`[...s]`、`Array.from(s)` 才能按码点正确遍历。

```typescript
const s = "a😀b";
console.log(s.length); // 4（😀 占两个码元）
for (const ch of s) {
  console.log(ch);     // a, 😀, b ✅
}
```

---

### C#

```csharp
string s = "hello";

// 1. foreach（推荐）
foreach (char c in s)
{
    Console.WriteLine(c);
}

// 2. 传统 for 下标循环
for (int i = 0; i < s.Length; i++)
{
    Console.WriteLine(s[i]); // 返回 char
}

// 3. 用 LINQ（字符串实现 IEnumerable<char>）
using System.Linq;
s.ToList().ForEach(c => Console.WriteLine(c));
```

**Unicode 说明**：`s.Length` 和 `s[i]` 按 **UTF-16 码元** 计数，emoji 会被拆成两个 `char`。要按码点遍历，用 `EnumerateRunes()`（.NET Core 3.0+）或 `StringInfo.GetTextElementEnumerator`。

```csharp
string s = "a😀b";
Console.WriteLine(s.Length); // 4

// 按码点遍历
foreach (var rune in s.EnumerateRunes())
{
    Console.WriteLine(rune); // a, 😀, b ✅
}
```

---

## 二、max 相关函数

### Python

```python
# 1. 内置 max()，支持可变参数
max(3, 1, 4, 1, 5)        # 5

# 2. 对可迭代对象（含字符串、列表）求最大值
max([3, 1, 4, 1, 5])      # 5
max("hello")              # 'o'（按字符的码点比较）

# 3. 带 key 参数
max(["abc", "de", "fgh"], key=len)  # "abc"
```

| 用法 | 说明 |
|------|------|
| `max(a, b, c, ...)` | 可变参数，任意多个 |
| `max(iterable)` | 可迭代对象 |
| `max(iterable, key=func)` | 自定义比较键 |
| `min(...)` | 对应取最小值 |

---

### TypeScript

```typescript
// 1. Math.max()，支持可变参数（但只接受数字）
Math.max(3, 1, 4, 1, 5); // 5

// 2. 数组求最大，用展开运算符
const arr = [3, 1, 4, 1, 5];
Math.max(...arr);        // 5

// 3. 大数组用 reduce（避免参数过多报错）
arr.reduce((a, b) => Math.max(a, b)); // 5

// 4. 取最小值
Math.min(...arr);        // 1
```

| 用法 | 说明 |
|------|------|
| `Math.max(a, b, c, ...)` | 可变参数，只接受数字 |
| `Math.max(...arr)` | 数组需展开 |
| `arr.reduce(...)` | 超大数组更安全 |
| `Math.min(...)` | 对应取最小值 |

**注意**：`Math.max()` 无参数返回 `-Infinity`；含非数字返回 `NaN`；空数组展开返回 `-Infinity`。

---

### C#

```csharp
using System.Linq;

// 1. Math.Max()，只接受两个参数
Math.Max(3, 5);        // 5
// Math.Max(3, 1, 4);  // ❌ 编译错误，无三参数重载

// 2. LINQ 的 Max()，对集合/数组求最大
int[] arr = { 3, 1, 4, 1, 5 };
int m = arr.Max();     // 5

// 3. 字符串（字符集合）求最大
string s = "hello";
char maxChar = s.Max(); // 'o'

// 4. 多个数求最大：数组 + LINQ，或嵌套 Math.Max
int m2 = new[] { 3, 1, 4, 1, 5 }.Max();          // 5
int m3 = Math.Max(Math.Max(3, 1), Math.Max(4, 5)); // 5

// 5. 取最小值
int min = arr.Min();   // 1
```

| 用法 | 说明 |
|------|------|
| `Math.Max(a, b)` | **仅两个参数** |
| `arr.Max()` | LINQ，需 `using System.Linq` |
| `arr.Min()` | LINQ 取最小 |

---

## 三、三语言对照速查表

### 遍历字符串

| 需求 | Python | TypeScript | C# |
|------|--------|------------|-----|
| 逐个取字符 | `for ch in s` | `for (const ch of s)` | `foreach (char c in s)` |
| 下标遍历 | `for i in range(len(s))` | `for (let i=0;i<s.length;i++)` | `for (int i=0;i<s.Length;i++)` |
| 下标 + 字符 | `enumerate(s)` | 手写 `for` 或 `entries` | 手写 `for` |
| 转字符数组 | `list(s)` | `[...s]` / `Array.from(s)` | `s.ToCharArray()` |
| 按码点（emoji 安全） | 默认即安全 ✅ | `for...of` / 展开 | `EnumerateRunes()` |

### 求最大值

| 需求 | Python | TypeScript | C# |
|------|--------|------------|-----|
| 两个数取大 | `max(a, b)` | `Math.max(a, b)` | `Math.Max(a, b)` |
| 多个数取大 | `max(a, b, c)` ✅ | `Math.max(a, b, c)` ✅ | 需数组或嵌套 ❌ |
| 数组/集合取大 | `max(iterable)` | `Math.max(...arr)` | `arr.Max()`（LINQ） |
| 取最小 | `min(...)` | `Math.min(...)` | `Math.Min(a,b)` / `arr.Min()` |

---

## 四、关键结论

1. **Python** 字符串天然按码点处理，遍历最省心；`max()` 既是可变参数函数，又能直接吃可迭代对象。
2. **TypeScript** 遍历用 `for...of` 才 emoji 安全；`Math.max` 支持可变参数，但数组必须 `...` 展开。
3. **C#** 的 `Math.Max` 只有两参数重载，多个数求最大要用 LINQ 的 `Max()` 或嵌套；字符串按码元索引，码点遍历需 `EnumerateRunes()`。
