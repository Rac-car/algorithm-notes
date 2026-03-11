// V0见课本原题

#include <cstdio>

int n;

struct Block
{
    int up, down; // up: 上面的木块编号；down: 下面的木块编号；-1 表示没有
} block[25];

// 找到 x 所在这一堆的最顶端
int find_up(int x)
{
    if (block[x].up == -1)
        return x;
    return find_up(block[x].up);
}

// 找到 x 所在这一堆的最底端
int find_down(int x)
{
    if (block[x].down == -1)
        return x;
    return find_down(block[x].down);
}

// 把从 x 开始往上的所有木块都“送回原位”
// 这里的“回原位”在你的链式模型里就是：把它们一个个拆成单独的一堆
void move_up(int x)
{
    if (x == -1)
        return;

    int next = block[x].up; // 先记住 x 上面的木块，避免后面断链后丢信息

    // 把 x 从下面那个木块上断开
    if (block[x].down != -1)
        block[block[x].down].up = -1;

    block[x].down = -1; // x 成为新堆底
    block[x].up = -1;   // x 上面暂时没人，它自己成为单独一堆

    move_up(next); // 继续处理原来在 x 上面的那些木块
}

// 从底到顶输出一整堆
void solve(int x)
{
    printf(" %d", x);
    if (block[x].up == -1)
        return;
    solve(block[x].up);
}

int main()
{
    scanf("%d", &n);

    // 初始化：每个木块单独一堆
    for (int i = 0; i < n; i++)
    {
        block[i].up = -1;
        block[i].down = -1;
    }

    while (1)
    {
        int a, b;
        int bb; // bb 表示目标位置：要接到哪个木块上面
        char ope1[10], ope2[10];

        scanf("%s", ope1);
        if (ope1[0] == 'q')
            break; // quit

        scanf("%d%s%d", &a, ope2, &b);

        // 如果 a 和 b 在同一堆，命令无效
        if (find_down(a) == find_down(b))
            continue;

        // move：先把 a 上面的木块都送回原位
        if (ope1[0] == 'm')
            move_up(block[a].up);

        // onto：先把 b 上面的木块都送回原位，然后目标就是 b 本身
        if (ope2[1] == 'n') // onto 的第二个字母是 'n'
        {
            move_up(block[b].up);
            bb = b;
        }
        else // over：目标是 b 所在堆的最顶端
        {
            bb = find_up(b);
        }

        // 把 a 从原来下面的木块上断开
        if (block[a].down != -1)
            block[block[a].down].up = -1;

        // 把 a（以及它上面的整段，如果有的话）接到 bb 上面
        block[a].down = bb;
        block[bb].up = a;
    }

    // 输出所有位置的状态
    for (int i = 0; i < n; i++)
    {
        printf("%d:", i);
        if (block[i].down == -1)
            solve(i);
        printf("\n");
    }

    return 0;
}