#include <cstdio>
#include <cstring>

#define MAXN 2005

char s[MAXN];

static int is_period(const char *s, int n, int p)
{
    for (int i = 0; i < n; i++)
    {
        if (s[i] != s[i % p])
            return 0;
    }
    return 1;
}

int main()
{
    int T;
    if (scanf("%d", &T) != 1)
        return 0;

    while (T--)
    {
        scanf("%s", s);
        int n = (int)strlen(s);

        for (int p = 1; p <= n; p++)
        {
            if (n % p != 0)
                continue;
            if (is_period(s, n, p))
            {
                printf("%d\n", p);
                break;
            }
        }

        if (T)
            printf("\n");
    }
    return 0;
}

/*
V0 的做法是从小到大枚举周期长度：
1.对 p = 1..n 依次尝试。
2.如果 n % p != 0，说明长度为 p 的块无法重复整数次铺满整个串，直接跳过。
3.若能整除，则检查是否满足：对所有位置 i，都有 s[i] == s[i % p]
4.第一个满足条件的 p 就是最小周期，输出并结束。
*/

/*
缺点：时间复杂度接近O(n^2)
*/