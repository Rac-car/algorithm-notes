#include <cstdio>
#include <cstring>

const int maxn = 60;

// findPos[i][j] 的含义：
// 原来在 (i, j) 的那份数据，现在移动到了哪里。
// 如果变成 {0, 0}，表示这份数据已经被删除了。
struct Point
{
    int x, y;
};

Point findPos[maxn][maxn];

// R, C 是原始表格大小。
// 后面所有查询中的 (r, c)，都是指“原始表格里的那个单元格”。
int R, C;

// 当前表格的行数和列数。
// 这两个变量不是必须参与每次查询，但保留下来能帮助理解表格当前大小。
int curR, curC;

// 初始化映射关系：
// 一开始原来在 (i, j) 的数据，当然就在 (i, j)。
void init(int r, int c)
{
    memset(findPos, 0, sizeof(findPos));

    R = r;
    C = c;
    curR = r;
    curC = c;

    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            findPos[i][j] = {i, j};
        }
    }
}

// 删除列的思路：
// 对每一份“原始数据”来看：
// 1. 如果它当前就在被删掉的列上，那么这份数据直接消失。
// 2. 否则，它左边删掉了几列，它就向左移动几列。
void deleteCol(int A, int a[])
{
    bool del[maxn];
    memset(del, 0, sizeof(del));

    for (int i = 0; i < A; i++)
    {
        del[a[i]] = true;
    }

    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            // 已经消失的数据，不再参与后续操作
            if (findPos[i][j].x == 0 && findPos[i][j].y == 0)
                continue;

            int x = findPos[i][j].x;
            int y = findPos[i][j].y;

            // 注意这里看的是“当前列 y”，不是原始列 j
            if (del[y])
            {
                findPos[i][j] = {0, 0};
            }
            else
            {
                int cnt = 0;

                // 统计它左边被删掉了多少列
                for (int k = 0; k < A; k++)
                {
                    if (a[k] < y)
                        cnt++;
                }

                findPos[i][j].y = y - cnt;
            }
        }
    }

    curC -= A;
}

// 删除行和删除列完全同理：
// 1. 如果当前就在被删掉的行上，这份数据消失。
// 2. 否则，它上面删掉了几行，它就向上移动几行。
void deleteRow(int A, int a[])
{
    bool del[maxn];
    memset(del, 0, sizeof(del));

    for (int i = 0; i < A; i++)
    {
        del[a[i]] = true;
    }

    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            if (findPos[i][j].x == 0 && findPos[i][j].y == 0)
                continue;

            int x = findPos[i][j].x;
            int y = findPos[i][j].y;

            if (del[x])
            {
                findPos[i][j] = {0, 0};
            }
            else
            {
                int cnt = 0;

                for (int k = 0; k < A; k++)
                {
                    if (a[k] < x)
                        cnt++;
                }

                findPos[i][j].x = x - cnt;
            }
        }
    }

    curR -= A;
}

// 插入列的思路和删除列相反：
// 在第 p 列前插入一列，会让原来第 p 列及其右边的列都向右移动。
// 所以统计的是：有多少插入位置 <= 当前列。
void insertCol(int A, int a[])
{
    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            if (findPos[i][j].x == 0 && findPos[i][j].y == 0)
                continue;

            int y = findPos[i][j].y;
            int cnt = 0;

            for (int k = 0; k < A; k++)
            {
                if (a[k] <= y)
                    cnt++;
            }

            findPos[i][j].y = y + cnt;
        }
    }

    curC += A;
}

// 插入行同理：
// 在第 p 行前插入一行，会让原来第 p 行及其下面的行向下移动。
void insertRow(int A, int a[])
{
    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            if (findPos[i][j].x == 0 && findPos[i][j].y == 0)
                continue;

            int x = findPos[i][j].x;
            int cnt = 0;

            for (int k = 0; k < A; k++)
            {
                if (a[k] <= x)
                    cnt++;
            }

            findPos[i][j].x = x + cnt;
        }
    }

    curR += A;
}

// 交换单元格时，不是交换“原始位置”，
// 而是交换“当前表格里这两个位置上的内容”。
//
// 因为我们维护的是：原始数据现在在哪。
// 所以要做的是：
// 找到当前在 (r1, c1) 的那份数据，和当前在 (r2, c2) 的那份数据，交换它们的位置记录。
void exchangeCell(int r1, int c1, int r2, int c2)
{
    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            if (findPos[i][j].x == 0 && findPos[i][j].y == 0)
                continue;

            if (findPos[i][j].x == r1 && findPos[i][j].y == c1)
            {
                findPos[i][j] = {r2, c2};
            }
            else if (findPos[i][j].x == r2 && findPos[i][j].y == c2)
            {
                findPos[i][j] = {r1, c1};
            }
        }
    }
}

int main()
{
    int kase = 1;

    while (true)
    {
        int r, c;
        scanf("%d%d", &r, &c);

        // 读到 0 0，表示所有数据结束
        if (r == 0 && c == 0)
            break;

        init(r, c);

        int n;
        scanf("%d", &n);

        while (n--)
        {
            char op[5];
            scanf("%s", op);

            // EX 的参数固定是 4 个数
            if (op[0] == 'E' && op[1] == 'X')
            {
                int r1, c1, r2, c2;
                scanf("%d%d%d%d", &r1, &c1, &r2, &c2);
                exchangeCell(r1, c1, r2, c2);
            }
            else
            {
                // DC / DR / IC / IR 都是：
                // 先给一个个数 A，再给 A 个行号或列号
                int A, a[15];
                scanf("%d", &A);

                for (int i = 0; i < A; i++)
                {
                    scanf("%d", &a[i]);
                }

                if (op[0] == 'D' && op[1] == 'C')
                {
                    deleteCol(A, a);
                }
                else if (op[0] == 'D' && op[1] == 'R')
                {
                    deleteRow(A, a);
                }
                else if (op[0] == 'I' && op[1] == 'C')
                {
                    insertCol(A, a);
                }
                else if (op[0] == 'I' && op[1] == 'R')
                {
                    insertRow(A, a);
                }
            }
        }

        int q;
        scanf("%d", &q);

        if (kase > 1)
            printf("\n");
        printf("Spreadsheet #%d\n", kase);

        while (q--)
        {
            int x, y;
            scanf("%d%d", &x, &y);

            printf("Cell data in (%d,%d) ", x, y);

            // 查询的是“原始 (x, y) 这份数据现在在哪”
            if (findPos[x][y].x == 0 && findPos[x][y].y == 0)
            {
                printf("GONE\n");
            }
            else
            {
                printf("moved to (%d,%d)\n", findPos[x][y].x, findPos[x][y].y);
            }
        }

        kase++;
    }

    return 0;
}