#include <cstdio>
#include <cstring>
int code[8][1 << 8]; // 对于code[len]，都需要存储 2^len - 1个字符

// 用于解决编码文本可以由多行组成的问题，返回读取的下一个字符
int readchar()
{
    for (;;)
    {
        int ch = getchar();
        if (ch == EOF)
            return EOF;
        if (ch != '\n' && ch != '\r')
        { // 由于不同操作系统下的换行代码可能不同
            return ch;
        }
    }
}

// 将读取的一段len长度编码转换为十进制并返回value
int readint(int len)
{
    int v = 0;
    while (len--)
    {
        v = v * 2 + readchar() - '0';
    }
    return v;
}

// 读取编码头
int readcodes()
{
    memset(code, 0, sizeof(code)); // 避免残留上一次的字符
    int ch = readchar();
    if (ch == EOF)
        return 0;
    code[1][0] = ch;
    for (int len = 2; len <= 7; len++)
    {
        for (int i = 0; i < (1 << len) - 1; i++)
        {
            int ch = getchar();
            if (ch == EOF)
            {
                return 0;
            }
            if (ch == '\n' || ch == '\r')
            {
                return 1;
            }
            code[len][i] = ch;
        }
    }
    return 1;
}

int main()
{
    while (readcodes())
    {
        for (;;)
        {
            int len = readint(3);
            if (len == 0)
            {
                break;
            }
            for (;;)
            {
                int v = readint(len);
                if (v == (1 << len) - 1)
                {
                    break;
                }
                else
                {
                    putchar(code[len][v]);
                }
            }
        }
        putchar('\n');
    }
    return 0;
}