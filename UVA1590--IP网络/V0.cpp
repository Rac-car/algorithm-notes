#include <cstdio>
#include <cstdint> // 这里定义了 uint32_t，表示“32位无符号整数”

/*
    把四段十进制 IP：a.b.c.d
    转成一个 32 位无符号整数。

    为什么要用 uint32_t？
    因为 IPv4 地址本质上就是 32 位二进制数。
    用 uint32_t 可以明确保证是“32位无符号整数”，
    比 unsigned int 更严谨，因为 unsigned int 在不同平台上不一定一定是 32 位。

    (uint32_t) 是强制类型转换：
    例如 (uint32_t)a 表示把 a 临时转成 uint32_t 再参与运算。

    这里要先转成 uint32_t 再左移，是为了确保位运算在 32 位无符号范围内进行。
*/
uint32_t ip_to_int(int a, int b, int c, int d)
{
    return ((uint32_t)a << 24) |
           ((uint32_t)b << 16) |
           ((uint32_t)c << 8) |
           (uint32_t)d;
}

/*
    把一个 32 位整数重新按 IP 的形式输出。
    比如：
    x = 11000010 01010101 10100000 10110000
    输出成：
    194.85.160.176
*/
void print_ip(uint32_t x)
{
    printf("%u.%u.%u.%u\n",
           (x >> 24) & 255,
           (x >> 16) & 255,
           (x >> 8) & 255,
           x & 255);
}

int main()
{
    int m;

    /*
        UVA 这题通常读到 EOF 为止，所以用 while(scanf(...) == 1)
    */
    while (scanf("%d", &m) == 1)
    {
        int a, b, c, d;
        uint32_t ip;
        uint32_t minIP, maxIP;

        /* 先读第一个 IP，用它初始化最小值和最大值 */
        scanf("%d.%d.%d.%d", &a, &b, &c, &d);
        minIP = maxIP = ip_to_int(a, b, c, d);

        /* 继续读剩下的 IP，维护最小值和最大值 */
        for (int i = 1; i < m; i++)
        {
            scanf("%d.%d.%d.%d", &a, &b, &c, &d);
            ip = ip_to_int(a, b, c, d);

            if (ip < minIP)
                minIP = ip;
            if (ip > maxIP)
                maxIP = ip;
        }

        /*
            关键思想：
            所有 IP 的最长公共前缀，等价于 minIP 和 maxIP 的最长公共前缀。

            diff = minIP ^ maxIP
            按位异或 XOR 的性质：
            - 相同位异或结果为 0
            - 不同位异或结果为 1

            所以 diff 中为 1 的位置，就是 minIP 和 maxIP 不同的位置。
        */
        uint32_t diff = minIP ^ maxIP;

        /*
            0xFFFFFFFFu 的含义：
            - 0x 表示十六进制
            - FFFFFFFF 表示 8 个十六进制 F
            - 每个 F 是二进制 1111
            所以它整体就是 32 个 1：

            11111111 11111111 11111111 11111111

            最后的 u 表示这是 unsigned（无符号）常量。
        */
        uint32_t mask;

        /*
            如果 diff == 0，说明 minIP 和 maxIP 完全相同，
            也就说明所有 IP 全都相同。
            这时掩码应为全 1，即 255.255.255.255
        */
        if (diff == 0)
        {
            mask = 0xFFFFFFFFu;
        }
        else
        {
            int pos;

            /*
                从高位到低位找“最高的不同位”。

                if ((diff >> i) & 1u) 的意思：
                1. diff >> i ：把第 i 位移到最右边
                2. & 1u      ：只取最右边这一位
                3. 如果结果是 1，说明 diff 的第 i 位为 1

                也就是说，这句是在判断：
                “diff 的第 i 位是不是 1”
            */
            for (pos = 31; pos >= 0; pos--)
            {
                if ((diff >> pos) & 1u)
                {
                    break;
                }
            }

            /*
                如果最高不同位在 pos，
                那么公共前缀只能保留 pos 左边的那些位，
                从 pos 到 0 都必须清零。

                理想情况下，mask 可以写成：
                0xFFFFFFFFu << (pos + 1)

                例如：
                pos = 2
                说明最高不同位在第 2 位
                那么掩码应是：
                11111111 11111111 11111111 11111000

                也就是把最后 3 位清零。

                但是这里有一个必须特别注意的边界：
                如果 pos == 31，那么 (pos + 1) == 32
                也就是要写：
                    0xFFFFFFFFu << 32
                在 C 语言里，“左移位数等于类型位数”是未定义行为。

                所以这里必须单独处理，避免未定义行为。
            */
            if (pos == 31)
            {
                /*
                    最高位都不同，说明根本没有公共前缀，
                    掩码就应该是 0.0.0.0
                */
                mask = 0;
            }
            else
            {
                /*
                    这里只有 pos <= 30，所以下面的左移是安全的，
                    不会出现 << 32 的未定义行为。
                */
                mask = 0xFFFFFFFFu << (pos + 1);
            }
        }

        /*
            网络地址 = 任意一个 IP & mask
            用 minIP 就可以，因为公共前缀部分对所有 IP 都相同。

            & 是按位与：
            mask 为 1 的位置保留原值
            mask 为 0 的位置清零
        */
        uint32_t network = minIP & mask;

        print_ip(network);
        print_ip(mask);
    }

    return 0;
}