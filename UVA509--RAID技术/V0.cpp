#include <cstdio>
#include <cstring>

#define maxs 64
#define maxd 6
#define maxb 100
#define maxbits (maxd * maxb * maxs + 10)

char Disk[maxd][maxb][maxs];
char bits[maxbits];

int main()
{
    int d, s, b;
    char check_;
    int kase = 0;

    while (scanf("%d", &d) == 1 && d != 0)
    {
        scanf("%d%d", &s, &b);

        int check = 0; // 0 表示偶校验 E，1 表示奇校验 O
        scanf(" %c", &check_);
        if (check_ == 'O')
        {
            check = 1;
        }

        memset(Disk, 0, sizeof(Disk));

        // 读入每个磁盘的内容
        // Disk[i][j][k] 表示：
        // 第 i 个磁盘，第 j 个块，第 k 个 bit
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < b; j++)
            {
                for (int k = 0; k < s; k++)
                {
                    scanf(" %c", &Disk[i][j][k]);
                }
            }
        }

        int valid = 1;

        // 检查每一列 bit（固定 j 和 k，看所有磁盘 i）
        // 并在可能的情况下修复唯一的 x
        for (int j = 0; j < b && valid; j++)
        {
            for (int k = 0; k < s && valid; k++)
            {
                int cnt1 = 0;  // 这一列中 '1' 的个数
                int cntx = 0;  // 这一列中 'x' 的个数
                int posx = -1; // 记录 x 在哪一个磁盘上

                for (int i = 0; i < d; i++)
                {
                    if (Disk[i][j][k] == '1')
                    {
                        cnt1++;
                    }
                    else if (Disk[i][j][k] == 'x')
                    {
                        cntx++;
                        posx = i;
                    }
                }

                // 如果同一列有两个或更多 x，这一列无法恢复
                if (cntx > 1)
                {
                    valid = 0;
                    break;
                }

                // 如果恰好有一个 x，可以根据奇偶校验补出来
                if (cntx == 1)
                {
                    if (check == 0)
                    {
                        // 偶校验 E：
                        // 总共 1 的个数应为偶数
                        // 当前 cnt1 为偶数 -> x 应补 0
                        // 当前 cnt1 为奇数 -> x 应补 1
                        if (cnt1 % 2 == 0)
                        {
                            Disk[posx][j][k] = '0';
                        }
                        else
                        {
                            Disk[posx][j][k] = '1';
                        }
                    }
                    else
                    {
                        // 奇校验 O：
                        // 总共 1 的个数应为奇数
                        // 当前 cnt1 为偶数 -> x 应补 1
                        // 当前 cnt1 为奇数 -> x 应补 0
                        if (cnt1 % 2 == 0)
                        {
                            Disk[posx][j][k] = '1';
                        }
                        else
                        {
                            Disk[posx][j][k] = '0';
                        }
                    }
                }
                else
                {
                    // 没有 x，就直接检查这一列是否满足校验
                    if (check == 0)
                    {
                        // 偶校验：1 的个数必须是偶数
                        if (cnt1 % 2 != 0)
                        {
                            valid = 0;
                            break;
                        }
                    }
                    else
                    {
                        // 奇校验：1 的个数必须是奇数
                        if (cnt1 % 2 == 0)
                        {
                            valid = 0;
                            break;
                        }
                    }
                }
            }
        }

        kase++;
        if (!valid)
        {
            printf("Disk set %d is invalid.\n", kase);
            continue;
        }

        // 走到这里说明整个 disk set 合法
        // 下面要按题目要求提取真正的数据内容
        //
        // 第 j 个块的校验盘是 p = j % d
        // 这一点非常关键，不是 j%d-1
        //
        // 每个块中，除了校验盘，其余磁盘的 bit 都是数据
        int len = 0;
        for (int j = 0; j < b; j++)
        {
            int p = j % d; // 第 j 个块对应的校验盘

            for (int i = 0; i < d; i++)
            {
                if (i == p)
                    continue; // 跳过校验盘

                for (int k = 0; k < s; k++)
                {
                    bits[len++] = Disk[i][j][k];
                }
            }
        }

        // 按题意，最后如果不足 4 位，要在右边补 0
        while (len % 4 != 0)
        {
            bits[len++] = '0';
        }

        bits[len] = '\0';

        // 输出十六进制结果
        printf("Disk set %d is valid, contents are: ", kase);

        for (int i = 0; i < len; i += 4)
        {
            int val = 0;
            for (int j = 0; j < 4; j++)
            {
                val = val * 2 + (bits[i + j] - '0');
            }

            if (val < 10)
            {
                printf("%c", '0' + val);
            }
            else
            {
                printf("%c", 'A' + (val - 10));
            }
        }
        printf("\n");
    }

    return 0;
}