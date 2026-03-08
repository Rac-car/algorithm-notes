#include <cstdio>
#include <cstring>

int H[10][10], V[10][10];
int rightLen[10][10], downLen[10][10];
int ans[10];

int main()
{
    int n, m;
    int kase = 1;

    while (scanf("%d", &n) == 1)
    {
        scanf("%d", &m);

        memset(H, 0, sizeof(H));
        memset(V, 0, sizeof(V));
        memset(rightLen, 0, sizeof(rightLen));
        memset(downLen, 0, sizeof(downLen));
        memset(ans, 0, sizeof(ans));

        for (int k = 0; k < m; k++)
        {
            char ch;
            int i, j;
            scanf(" %c %d %d", &ch, &i, &j);

            if (ch == 'H')
            {
                H[i][j] = 1;
            }
            else
            {
                // 输入里的 V i j 是“第 i 列，从第 j 行往下连”
                // 统一转换成 V[行][列]，便于后面和 H 的坐标体系保持一致
                V[j][i] = 1;
            }
        }

        // 计算 rightLen：
        // rightLen[i][j] = 从 (i,j) 开始向右连续的横边数量
        //
        // 为什么从右往左推？
        // 因为 rightLen[i][j] 依赖 rightLen[i][j+1]
        // 所以必须保证右边的位置已经算好
        for (int i = 1; i <= n; i++)
        {
            for (int j = n - 1; j >= 1; j--)
            {
                if (H[i][j])
                {
                    rightLen[i][j] = rightLen[i][j + 1] + 1;
                }
                else
                {
                    rightLen[i][j] = 0;
                }
            }
        }

        // 计算 downLen：
        // downLen[i][j] = 从 (i,j) 开始向下连续的竖边数量
        //
        // 为什么从下往上推？
        // 因为 downLen[i][j] 依赖 downLen[i+1][j]
        for (int i = n - 1; i >= 1; i--)
        {
            for (int j = 1; j <= n; j++)
            {
                if (V[i][j])
                {
                    downLen[i][j] = downLen[i + 1][j] + 1;
                }
                else
                {
                    downLen[i][j] = 0;
                }
            }
        }

        // 枚举边长和左上角
        for (int len = 1; len < n; len++)
        {
            int cnt = 0;

            for (int x = 1; x + len <= n; x++)
            {
                for (int y = 1; y + len <= n; y++)
                {
                    // 一个边长为 len 的正方形成立，
                    // 等价于四条边起点处的连续边数都不少于 len
                    if (rightLen[x][y] >= len &&
                        rightLen[x + len][y] >= len &&
                        downLen[x][y] >= len &&
                        downLen[x][y + len] >= len)
                    {
                        cnt++;
                    }
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