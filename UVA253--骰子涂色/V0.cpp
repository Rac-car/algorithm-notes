#include <cstdio>
#include <cstring>

int same(char a[], char b[])
{
    for (int i = 0; i < 6; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

// 面的顺序：
// s[0] = 上
// s[1] = 前
// s[2] = 左
// s[3] = 右
// s[4] = 后
// s[5] = 下

void spin(char s[])
{ // 绕竖直轴转 90 度
    char t = s[1];
    s[1] = s[2];
    s[2] = s[4];
    s[4] = s[3];
    s[3] = t;
}

void rollRight(char s[])
{ // 向右翻
    char t = s[0];
    s[0] = s[2];
    s[2] = s[5];
    s[5] = s[3];
    s[3] = t;
}

void rollForward(char s[])
{ // 向前翻
    char t = s[0];
    s[0] = s[4];
    s[4] = s[5];
    s[5] = s[1];
    s[1] = t;
}

int equalCube(char a[], char b[])
{
    char t[6];
    memcpy(t, a, 6);

    // 枚举 24 种朝向
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (same(t, b))
                return 1;
            spin(t);
        }

        if (i % 2 == 0)
            rollRight(t);
        else
            rollForward(t);
    }

    return 0;
}

int main()
{
    char s[20];
    char a[6], b[6];

    while (scanf("%s", s) == 1)
    {
        for (int i = 0; i < 6; i++)
            a[i] = s[i];
        for (int i = 0; i < 6; i++)
            b[i] = s[i + 6];

        if (equalCube(a, b))
            printf("TRUE\n");
        else
            printf("FALSE\n");
    }

    return 0;
}