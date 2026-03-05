/*
V1（修正版）：用 KMP 前缀函数 next/pi 计算最小周期

关键修正点：
1) next[0] 必须是 0，所以 j 从 1 开始（不能从 0 开始，否则 next[0] 会被算成 1）
2) 失配回退必须按“当前已匹配长度 i”回退：i = next[i-1]
   不能写 i = next[j-1]（那会把回退绑死在位置 j，而且 j=0 时会 next[-1] 越界）
3) 回退要用 while 连续回退，直到能匹配或 i=0（否则会漏掉多级回退）
4) 不用变长数组 int next[len]（C++不标准），用固定数组更稳
*/

#include <cstdio>
#include <cstring>

#define MAXN 2005
char s[MAXN];
int nextArr[MAXN];

int main()
{
    if (scanf("%s", s) != 1)
        return 0;
    int len = (int)strlen(s);

    // nextArr[j] = s[0..j] 的最长真前后缀长度
    nextArr[0] = 0;

    int i = 0; // i 表示：当前已经匹配的长度（下一次要比较 s[i]）
    int j = 1; // j 从 1 开始计算 next（保证 next[0]=0）

    while (j < len)
    {
        if (s[i] == s[j])
        {
            // 匹配成功：已匹配长度 +1
            i++;
            nextArr[j] = i;
            j++;
        }
        else
        {
            if (i > 0)
            {
                // 失配：沿着 border 链回退（关键！）
                i = nextArr[i - 1];
            }
            else
            {
                // i==0 仍失配：next 为 0，j 继续前进
                nextArr[j] = 0;
                j++;
            }
        }
    }

    // ---- 用 nextArr 求最小周期 ----
    // L = nextArr[len-1] 是最长真前后缀长度
    // p = len - L 是候选周期长度
    int L = nextArr[len - 1];
    int p = len - L;

    int ans = (len % p == 0) ? p : len;
    printf("%d\n", ans);

    return 0;
}

/*
若前L与后L之间存在重叠--则len-L一定是最小周期,此时len%(len-L)==0
若前L与后L之间不存在重叠--则len一定是最小周期,此时len%(len-L)!=0
*/