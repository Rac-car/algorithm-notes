#include <cstdio>
#include <cstring>

int H[10][10], V[10][10];
int ans[10];

int main()
{
    int n, m;
    int kase = 1;

    while (scanf("%d", &n) == 1)
    {
        scanf("%d", &m);

        // 每组数据都要清空。
        // H[i][j] 表示 (i,j) -> (i,j+1) 是否存在横边
        // V[i][j] 表示 (i,j) -> (i+1,j) 是否存在竖边
        memset(H, 0, sizeof(H));
        memset(V, 0, sizeof(V));
        memset(ans, 0, sizeof(ans));

        for (int k = 0; k < m; k++)
        {
            char ch;
            int i, j;
            scanf(" %c %d %d", &ch, &i, &j);

            if (ch == 'H')
            {
                // H i j : 第 i 行，第 j 列的点连到右边那个点
                H[i][j] = 1;
            }
            else
            {
                // V i j : 第 i 列，第 j 行的点连到下边那个点
                // 为了后续判断方便，统一存成 V[行][列]
                V[j][i] = 1;
            }
        }

        // len 表示正方形边长（由多少条小边构成）
        for (int len = 1; len < n; len++)
        {
            int cnt = 0;

            // 枚举正方形左上角 (x, y)
            // 因为边长是 len，所以右下角不能越界
            for (int x = 1; x + len <= n; x++)
            {
                for (int y = 1; y + len <= n; y++)
                {
                    bool ok = true;

                    // 逐段检查四条边：
                    // 上边和下边检查 H
                    // 左边和右边检查 V
                    for (int k = 0; k < len; k++)
                    {
                        if (!H[x][y + k])
                            ok = false; // 上边
                        if (!H[x + len][y + k])
                            ok = false; // 下边
                        if (!V[x + k][y])
                            ok = false; // 左边
                        if (!V[x + k][y + len])
                            ok = false; // 右边
                    }

                    if (ok)
                        cnt++;
                }
            }

            ans[len] = cnt;
        }

        if (kase > 1)
        {
            printf("\n**********************************\n\n");
        }

        printf("Problem #%d\n\n", kase++);

        bool found = false;
        for (int len = 1; len < n; len++)
        {
            if (ans[len] > 0)
            {
                found = true;
                printf("%d square (s) of size %d\n", ans[len], len);
            }
        }

        if (!found)
        {
            printf("No completed squares can be found.\n");
        }
    }

    return 0;
}