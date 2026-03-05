/*
V1（错误示范版）：尝试自己写 KMP 的 next/pi 数组

目标本来是：next[j] 表示 s[0..j] 的最长“真前后缀”长度（也叫 prefix function / pi）

但下面代码有多处逻辑/边界/定义错误，会导致：
- next 数组算错
- 可能越界（next[-1]）
- 失配回退机制不正确（回退不会沿着“已匹配长度”的链走）
*/

#include <cstdio>
#include <cstring>
#define maxn 100
char s[maxn];

int main()
{
    scanf("%s", s);
    int len = (int)strlen(s);

    // ❌ 错误点 1：int next[len] 是“变长数组 VLA”
    // 在 C++ 标准里不支持（有些编译器当扩展支持，但不可靠）。
    // 正确做法：用 vector<int> 或者 next[maxn]（前提 maxn 足够大）。
    int next[len] = {0};

    int count = 0;

    // ❌ 错误点 2：i、j 的初始化不对
    // 写 i=0, j=0 会先比较 s[0] 和 s[0]，导致 next[0] 被写成 1（见下面）
    // 但标准前缀函数应当满足：next[0] = 0
    int i = 0;
    int j = 0;

    // ❌ 错误点 3：while 条件 + j=0 会导致越界风险
    // 后面有 i = next[j-1]; 当 j==0 时会访问 next[-1]（越界，未定义行为）
    while (i < len - 1 && j < len)
    {

        if (s[i] == s[j])
        {

            // ❌ 错误点 4：next[j] 的含义被写乱了
            // 当 i=0, j=0 时进入这里：
            // count = i+1 = 1, next[0] = 1
            // 但标准定义 next[0] 必须是 0（长度为1的串没有“真前后缀”）
            count = i + 1;
            next[j] = count;

            i++;
            j++;
        }
        else
        {

            // ❌ 错误点 5：失配回退写错了
            // 写：i = next[j-1]
            // 但 KMP/前缀函数的回退应该根据“当前已匹配长度 i”来回退：
            // 正确应该是：i = next[i-1]（沿着 border 链回退）
            //
            // 用 next[j-1] 会把回退绑死在位置 j，不会正确地逐级缩短匹配长度。
            // 严重时会反复回到同一个 i，导致结果错误。
            i = next[j - 1]; // ⚠️ 这里 j=0 还会 next[-1] 越界

            // 下面这一大坨 if-else，本质是在“手动硬凑回退”，但仍然不符合 KMP 机制
            // KMP 正确做法是：while(i>0 && s[i]!=s[j]) i = next[i-1];

            // ❌ 错误点 6：回退后立刻再比较一次 + 多层分支
            // 容易漏掉多次回退（应该用 while 连续回退直到能匹配或 i=0）
            if (s[i] == s[j])
            {
                count = i + 1;
                next[j] = count;
                i++;
                j++;
            }
            else
            {
                count = 0;
                i = 0;

                // ❌ 错误点 7：把逻辑写成“再试一次”，但并没有处理一般情况
                // 仍然缺少“多级回退”的 while 过程
                if (s[i] == s[j])
                {
                    count = i + 1;
                    next[j] = count;
                    i++;
                    j++;
                }
                else
                {
                    next[j] = count;
                    j++;
                }
            }
        }
    }

    return 0;
}