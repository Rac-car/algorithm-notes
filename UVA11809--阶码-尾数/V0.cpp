#include <cstdio>
#include <cstring>
#include <cmath>

/*
1. 规格化形式写成：0.1xxxx...
2. 连续的小数 1 的个数是 M+1 位
   所以最大尾数是：
      0.111...111   (共 M+1 个 1)
   = 1/2 + 1/4 + ... + 1/2^(M+1)
   = 1 - 2^(-(M+1))
3. 指数部分有 E 位，最大正指数是：
      2^E - 1
所以最大可表示值：
   Fmax = (1 - 2^(-(M+1))) * 2^(2^E - 1)
两边取 log10：
   log10(Fmax)
 = log10(1 - 2^(-(M+1))) + (2^E - 1) * log10(2)
题目给出的是 A e B，也就是：
   A * 10^B
所以目标值的对数是：
   log10(A) + B
枚举所有：
   0 <= M <= 9
   1 <= E <= 30
一共 10 * 30 = 300 种可能，找最接近的一组。
*/

int main()
{
    char s[128];

    /*
    log10l 是 long double 版本的 log10 函数
    2.0L 里的 L 表示这是一个 long double 常量
    类似地：
      log10f  -> float 版
      log10   -> double 版
      log10l  -> long double 版
    这里用 long double 是为了提高精度，
    因为后面要比较很多候选值和目标值的对数差。
    */
    const long double LOG10_2 = log10l(2.0L);

    /*
    cand[M][E] 存每个 (M,E) 对应的 log10(Fmax)
    M 取 0~9
    E 取 1~30
    */
    long double cand[10][31];

    for (int M = 0; M <= 9; M++)
    {
        for (int E = 1; E <= 30; E++)
        {

            /*
            1LL 是 long long 类型的整数常量 1
            例如：
              1    -> int
              1L   -> long
              1LL  -> long long
            这里写：
              1LL << E
            表示把 long long 类型的 1 左移 E 位，
            等价于 2^E

            为什么用 long long 而不是 long？
            因为 long 在不同平台位数不稳定，
            long long 至少 64 位，更稳妥。

            注意：
            位移 << 只能用于整数类型，不能用于 long double。
            所以这里必须先用整数算出 2^E - 1。
            */
            long long emax = (1LL << E) - 1; // 最大正指数 = 2^E - 1

            /*
            最大尾数：
              1 - 2^(-(M+1))
            powl 是 long double 版本的幂函数：
              powl(a, b) = a^b
            */
            long double frac = 1.0L - powl(2.0L, -(long double)(M + 1));

            /*
            log10(Fmax)
            = log10(frac) + emax * log10(2)
            log10l 也是 long double 版本
            */
            cand[M][E] = log10l(frac) + (long double)emax * LOG10_2;
        }
    }

    while (scanf("%s", s) == 1)
    {

        /*
        strcmp 是字符串比较函数（定义在 string.h）
        strcmp(a, b) 的返回值：
          == 0  表示两个字符串完全相同
          < 0   表示 a 字典序小于 b
          > 0   表示 a 字典序大于 b
        所以：
          strcmp(s, "0e0") == 0
        的意思是：
          如果字符串 s 恰好等于 "0e0"，就结束输入
        */
        if (strcmp(s, "0e0") == 0)
            break;

        long double A;
        int B;

        /*
        sscanf = 从字符串里按格式读数据
        scanf  = 从标准输入读数据
        sscanf = 从字符数组/字符串读数据

        这里的格式串 "%Lfe%d" 表示：
          %Lf  -> 读一个 long double
          e    -> 中间必须有一个字符 'e'
          %d   -> 再读一个 int

        所以如果 s = "9.205357638345294e18"
        那么会得到：
          A = 9.205357638345294
          B = 18

        %Lf 中：
          %   表示格式说明开始
          L   表示 long double
          f   表示浮点数

        常见对应：
          scanf("%d", &x);    // int
          scanf("%lf", &x);   // double
          scanf("%Lf", &x);   // long double
        */
        sscanf(s, "%Lfe%d", &A, &B);

        /*
        输入值是 A * 10^B
        所以它的 log10 是：
          log10(A) + B
        */
        long double target = log10l(A) + (long double)B;

        int bestM = 0, bestE = 1;
        long double bestDiff = 1e100L;

        for (int M = 0; M <= 9; M++)
        {
            for (int E = 1; E <= 30; E++)
            {

                /*
                fabsl 是 long double 版本的绝对值函数

                对应关系：
                  abs    -> 整数绝对值
                  fabs   -> double 绝对值
                  fabsl  -> long double 绝对值

                这里表示：
                  求 cand[M][E] 和 target 的差的绝对值
                也就是比较“谁更接近目标值”
                */
                long double diff = fabsl(cand[M][E] - target);

                if (diff < bestDiff)
                {
                    bestDiff = diff;
                    bestM = M;
                    bestE = E;
                }
            }
        }

        printf("%d %d\n", bestM, bestE);
    }

    return 0;
}