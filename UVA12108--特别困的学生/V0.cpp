#include <iostream>
#include <string> // string、to_string 都在这个头文件里
#include <set>    // set 在这个头文件里
using namespace std;

int a[11], b[11], c[11], nxt[11];

int main()
{
    int n, Case = 0;

    while (cin >> n && n)
    {
        for (int i = 1; i <= n; i++)
        {
            cin >> a[i] >> b[i] >> c[i];
        }

        /*
            set<string> 的语法：

                set<元素类型> 集合名;

            这里写成：
                set<string> vis;

            意思是：
                定义一个集合 vis，里面存放 string 类型的元素。

            set 的特点：
            1. 元素自动按字典序存储
            2. 不会存重复元素
            3. 常用来做“判重”

            这题里用它来记录“某种全体学生状态是否已经出现过”。

            常用操作：
            vis.count(x)
                判断 x 是否在集合中
                - 返回 1：存在
                - 返回 0：不存在

            vis.insert(x)
                把 x 加入集合
        */
        set<string> vis;

        int minute = 0;

        while (1)
        {
            minute++;

            int cnt = 0; // 当前清醒人数
            for (int i = 1; i <= n; i++)
            {
                if (c[i] <= a[i])
                    cnt++;
            }

            // 如果当前这一分钟所有人都清醒
            if (cnt == n)
            {
                cout << "Case " << ++Case << ": " << minute << endl;
                break;
            }

            // 先算出下一分钟的状态（同时更新）
            for (int i = 1; i <= n; i++)
            {
                if (c[i] == a[i])
                {
                    // 当前在清醒阶段最后一分钟，下一分钟本来该睡觉
                    // 但题目要求：只有睡觉人数严格大于清醒人数时，才会去睡
                    if (2 * cnt < n)
                        nxt[i] = c[i] + 1;
                    else
                        nxt[i] = 1; // 否则重新开始一个清醒周期
                }
                else if (c[i] == a[i] + b[i])
                {
                    // 一个完整周期结束，回到第 1 分钟
                    nxt[i] = 1;
                }
                else
                {
                    // 正常往后走一分钟
                    nxt[i] = c[i] + 1;
                }
            }

            /*
                下面把“下一分钟所有学生的状态”拼成一个字符串 state，
                用它来做判重。

                例如：
                如果下一分钟 3 个学生的状态分别是 1, 3, 2
                那么 state 可以记成：
                    "1,3,2,"
            */
            string state = "";

            for (int i = 1; i <= n; i++)
            {
                c[i] = nxt[i];

                /*
                    to_string(c[i]) 的语法和用法：

                        to_string(数值)

                    作用：
                        把一个数字转成 string 字符串。

                    例如：
                        int x = 12;
                        string s = to_string(x);

                    结果：
                        s 变成 "12"

                    在这题里：
                        to_string(c[i])

                    表示把学生当前状态 c[i]（整数）
                    转成字符串，再拼接到 state 后面。

                    为什么不用 char(c[i] + '0')？
                    因为 c[i] 可能不止 1 位数，例如可能等于 10。
                    如果写 char(c[i] + '0')，只能正确处理 0~9，
                    对 10 这种两位数就会出错。

                    所以这里必须用：
                        to_string(c[i])

                    再加一个逗号：
                        + ","

                    这是为了防止状态混淆。
                    例如：
                        1,11 和 11,1
                    如果不加分隔符，可能都拼成类似 "111"，
                    会分不清。
                */
                state += to_string(c[i]) + ",";
            }

            /*
                set<string> 的用法之一：count()

                    vis.count(state)

                含义：
                    判断 state 这个字符串是否已经在集合 vis 中出现过。

                返回值：
                    1 -> 出现过
                    0 -> 没出现过
            */
            if (vis.count(state))
            {
                // 状态重复，说明进入循环，不会再出现全员清醒
                cout << "Case " << ++Case << ": -1" << endl;
                break;
            }

            /*
                set<string> 的用法之二：insert()

                    vis.insert(state)

                含义：
                    把当前状态 state 加入集合，
                    以后如果再遇到同样状态，就能查出来。
            */
            vis.insert(state);
        }
    }

    return 0;
}