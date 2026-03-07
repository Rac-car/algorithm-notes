这题的核心不是算法很复杂，而是**系统题的“状态设计 + 细节实现”**比较容易乱。

**SPMS（Student Performance Management System）** 的难点不在某个高深算法，而在于：

- 你能不能先把**数据结构想清楚**
- 你能不能把每个功能拆干净
- 你能不能处理好一堆“规则细节”

我按这三个层次给你讲：

1. **整体思路怎么走**
2. **思维难点在哪里**
3. **代码难点在哪里**
4. **我认为最难的部分代码怎么写**

# 一、整体思路怎么走

这题最适合的做法是：

## 1. 用一个数组存所有学生

每个学生保存这些信息：

- `SID`
- `CID`
- `name`
- 4 门课分数
- 总分

再加一个：

- `removed[i]`：表示这个学生有没有被删除

## 2. 不真的删，做“逻辑删除”

删除学生时，不要把数组里的元素搬来搬去，也不要真的 `erase`。

直接：

```cpp
removed[i] = true;
```

就行。

这样好处很大：

- 删除操作简单
- 查询时只要跳过被删的学生
- 排名和统计时也只统计没被删的学生

## 3. 排名不要预处理，查询时现算

这题里最容易误以为要排序。

其实**不需要排序**。

因为题目查询时要输出某个学生当前的 rank，你可以直接定义：

> 一个学生的 rank = 1 + 当前所有未删除学生中，总分比他高的人数

也就是“前面有多少人比他强，我就是第几名”。

这个定义可以直接写成一个函数：

```cpp
int rank(int i)
```

每次现算。

这样就避免了：

- 删除后重新排序
- 多个同分如何排名
- 名次跳跃怎么处理

这些麻烦。

## 4. 每个功能写成独立函数

建议拆成：

- `add()`
- `remove_or_query(int isQuery)`
- `rank(int i)`
- `show_ranking()`
- `show_statistics()`

其中删除和查询非常像，所以可以合并成一个函数：

- `isQuery = 1` 表示查询
- `isQuery = 0` 表示删除

这也是书上那段 `DQ(int isq)` 的意思。

# 二、思维难点

这题真正难的不是语法，是下面这几个“脑子里要先想通”的点。

## 难点 1：到底要不要排序

很多人一看到“rank”就会想排序。  
但这题排序其实会让你更难写。

因为：

- 学生会被删除
- 查询是按 SID 或 name 查某个人
- 同分排名不是简单下标 + 1
- 统计时又和排序没关系

所以更好的思路是：

> **不维护一个有序表**  
> 只维护原始数据数组，排名在需要时计算

这题最重要的观念就是：

**“rank 是一个函数，不是一张表。”**

## 难点 2：删除不要真的删

如果你真的把数组里的某个学生删掉：

- 后面元素要前移
- 下标会变化
- 查询和统计容易乱

所以最自然的方式是：

> 学生记录一直留着，只是打一个“已删除”的标记

这样所有功能都可以统一写成：

```cpp
if (!removed[i]) {
    ...
}
```

## 难点 3：查询和删除为什么能合并

因为这两个操作流程几乎一样：

1. 输入一个字符串 `s`
2. `s` 可能是 SID，也可能是 name
3. 遍历所有未删除学生
4. 找到满足：
   - `sid == s`
   - 或 `name == s`
5. 然后：
   - 如果是查询，就打印这个学生
   - 如果是删除，就把他标记删除

所以两者只是“找到后做什么”不一样。

这类题很适合抽出公共逻辑。

## 难点 4：rank 的本质是什么

rank 不是“把学生排一下得到下标”。

题目需要的是**竞赛排名**，同分同名次。

比如总分：

- 100
- 95
- 95
- 80

那么名次应是：

- 1
- 2
- 2
- 4

这个定义最自然的写法就是：

> `rank(i) = 1 + 比 i 总分高的人数`

这句非常重要，因为它自动解决了：

- 同分同名次
- 后面的名次跳过

## 难点 5：统计时到底统计谁

统计功能里有一个班级号 `CID`：

- 输入 `0` 表示统计所有班
- 输入某个班号表示只统计这个班

所以你写统计代码时，脑子里一定要先有一个“是否纳入统计”的判断条件：

> 某学生参与统计，当且仅当：
> - 没被删除
> - 并且 `cid == 0 || student.cid == cid`

很多人会把这句条件分散在各处，最后很乱。

## 难点 6：整体统计和单科统计不是一回事

统计里有两类信息：

### 单科统计

比如语文：

- 平均分
- 及格人数
- 不及格人数

这要一科一科统计。

### 整体统计

比如：

- 4 门都及格的人数
- 至少 3 门及格的人数
- 至少 2 门及格的人数
- 至少 1 门及格的人数
- 全部不及格的人数

这不是按单科算，而是要对**每个学生**先数他过了几门。

所以统计函数里一般要分两层：

- 对每门课累加
- 对每个学生统计 `passCount`

# 三、代码难点

## 难点 1：输入字符串后判断它是 SID 还是 name

其实你不需要真的判断。

最简单的方法是：

```cpp
if (sid[i] == s || name[i] == s)
```

只要任一匹配就行。

这就是书上那句：

```cpp
if(strcmp(sid[i], s) == 0 || strcmp(name[i], s) == 0)
```

这招很巧，因为题目保证：

- SID 唯一
- name 可以重复

所以：

- 输入 SID 时最多命中一个
- 输入 name 时可能命中多个

刚好符合题意。

## 难点 2：rank 函数怎么写得又短又对

关键点：

- 只统计未删除学生
- 只比较总分
- 不需要排序

最容易错的是：

- 把自己也算进去
- 把同分也算“比他高”
- 忘记跳过删除学生

## 难点 3：统计函数很容易越写越乱

因为它同时要做：

- 按班级过滤
- 4 门课平均
- 4 门课及格数
- 4 门课不及格数
- 每个学生过了几门
- 整体通过情况

所以统计函数必须分清两类量：

### 按学科累加

例如：

- `sum[sub]`
- `pass[sub]`

### 按学生统计

例如：

- `passCnt`：这个学生过了几门

然后根据 `passCnt` 去更新：

- `passedAll`
- `passed3OrMore`
- `passed2OrMore`
- `passed1OrMore`
- `failedAll`

## 难点 4：平均分输出的小数问题

这题书上专门提到 `EPS`，原因是浮点输出有时会有误差。

比如你明明想输出 `58.5`，内部可能是 `58.499999999`。

所以常见做法是：

```cpp
avg + 1e-5
```

或者直接保证分子分母是 `double`。

# 四、我认为最难的部分

我觉得这题最难的是两个地方：

1. `rank()`
2. `show_statistics()`

因为这两个地方最容易“概念没想透就写歪”。

下面我把这两块写给你，并带上解释。

## 1. 排名函数

### 思路

对第 `i` 个学生：

- 从头扫一遍所有学生
- 跳过已删除的
- 只要有人总分比他高，排名就加 1

### 代码

```cpp
int get_rank(int i) {
    int r = 1;
    for (int j = 0; j < n; j++) {
        if (!removed[j] && score[j][4] > score[i][4]) {
            r++;
        }
    }
    return r;
}
```

### 为什么这样写是对的

如果总分序列是：

- 100
- 95
- 95
- 80

那么对于 80：

- 比他高的有 3 个
- 所以 rank = 4

对于第二个 95：

- 比他高的只有 1 个（100）
- 所以 rank = 2

同分不会额外加，所以自然是并列名次。

这比“先排序再找位置”更稳。

## 2. 删除 / 查询合并函数

### 思路

这个函数的本质就是：

- 循环读一个字符串
- 读到 `"0"` 结束
- 每次遍历所有未删除学生
- 只要 SID 或 name 匹配：
  - 查询模式就输出
  - 删除模式就标记删除并计数

### 代码

```cpp
void DQ(int isQuery) {
    char s[maxl];
    for (;;) {
        printf("Please enter SID or name. Enter 0 to finish.\n");
        scanf("%s", s);

        if (strcmp(s, "0") == 0) break;

        int removedCount = 0;

        for (int i = 0; i < n; i++) {
            if (removed[i]) continue;

            if (strcmp(sid[i], s) == 0 || strcmp(name[i], s) == 0) {
                if (isQuery) {
                    printf("%d %s %d %s %d %d %d %d %.2f\n",
                           get_rank(i), sid[i], cid[i], name[i],
                           score[i][0], score[i][1], score[i][2], score[i][3],
                           score[i][4] / 4.0 + EPS);
                } else {
                    removed[i] = 1;
                    removedCount++;
                }
            }
        }

        if (!isQuery) {
            printf("%d student(s) removed.\n", removedCount);
        }
    }
}
```

### 这段代码最妙的地方

删除和查询的“查找过程”完全一样，只是“命中之后做什么”不同。

所以不必写两个几乎一样的函数。

这就是系统题里常说的：

> **把公共逻辑提出来**

## 3. 统计函数：最难的主体

### 思路拆开

先假设：

- `cidInput == 0` 表示统计所有班
- 否则只统计某个班

统计时，你要维护：

### 每门课的量

- `sum[4]`
- `pass[4]`

### 每个学生的整体情况

- 这个学生过了几门 `passCnt`

### 总体人数

- `students`
- `passedAll`
- `passed3`
- `passed2`
- `passed1`
- `failedAll`

### 代码

```cpp
void stat() {
    int cidInput;
    printf("Please enter class ID, 0 for the whole statistics.\n");
    scanf("%d", &cidInput);

    int sum[4] = {0};
    int pass[4] = {0};
    int students = 0;

    int passedAll = 0;
    int passed3 = 0;
    int passed2 = 0;
    int passed1 = 0;
    int failedAll = 0;

    for (int i = 0; i < n; i++) {
        if (removed[i]) continue;
        if (cidInput != 0 && cid[i] != cidInput) continue;

        students++;

        int passCnt = 0;
        for (int k = 0; k < 4; k++) {
            sum[k] += score[i][k];
            if (score[i][k] >= 60) {
                pass[k]++;
                passCnt++;
            }
        }

        if (passCnt == 4) passedAll++;
        if (passCnt >= 3) passed3++;
        if (passCnt >= 2) passed2++;
        if (passCnt >= 1) passed1++;
        if (passCnt == 0) failedAll++;
    }

    const char* subject[4] = {
        "Chinese",
        "Mathematics",
        "English",
        "Programming"
    };

    for (int k = 0; k < 4; k++) {
        printf("%s\n", subject[k]);
        printf("Average Score: %.2f\n", students ? sum[k] * 1.0 / students + EPS : 0.0);
        printf("Number of passed students: %d\n", pass[k]);
        printf("Number of failed students: %d\n\n", students - pass[k]);
    }

    printf("Overall:\n");
    printf("Number of students who passed all subjects: %d\n", passedAll);
    printf("Number of students who passed 3 or more subjects: %d\n", passed3);
    printf("Number of students who passed 2 or more subjects: %d\n", passed2);
    printf("Number of students who passed 1 or more subjects: %d\n", passed1);
    printf("Number of students who failed all subjects: %d\n\n", failedAll);
}
```

# 五、你写这题时最推荐的整体结构

我建议你按下面这个框架写：

```cpp
main
 ├── print_menu()
 ├── add()
 ├── DQ(0)   // remove
 ├── DQ(1)   // query
 ├── show_ranking()
 ├── stat()
 └── get_rank(i)
```

## 主程序流程

```cpp
for (;;) {
    print_menu();
    scanf("%d", &choice);

    if (choice == 0) break;
    if (choice == 1) add();
    if (choice == 2) DQ(0);
    if (choice == 3) DQ(1);
    if (choice == 4) show_ranking();
    if (choice == 5) stat();
}
```

# 六、这题最重要的几句总结

如果你想真正学会这题，记住下面 4 句就够了。

### 1

**不要排序，排名现算。**

### 2

**不要真删，逻辑删除。**

### 3

**删除和查询共用一套查找逻辑。**

### 4

**统计函数里要区分“按科目统计”和“按学生整体统计”。**