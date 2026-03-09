#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
using namespace std;

int main()
{
    map<char, string> dic1; // dic1: 字符 -> 摩尔斯码

    /*
        vector 是 C++ 的动态数组。
        和普通数组不同，vector 的长度可以自动增长。

        这里：
        words 用来存词典中的单词
        codes 用来存每个单词翻译后的摩尔斯码

        例如：
        words[0] = "EAT"
        codes[0] = "..--"

        表示第 0 个单词是 EAT，它对应的摩尔斯码是 ..--
    */
    vector<string> words;
    vector<string> codes;

    char a;
    string b, c;

    // 第一部分：读入“字符 -> 摩尔斯码”的映射
    cin >> a;
    while (a != '*')
    {
        cin >> b;
        dic1[a] = b;
        cin >> a;
    }

    // 第二部分：读入词典单词，并把每个单词翻译成摩尔斯码
    cin >> b;
    while (b != "*")
    {
        c.clear(); // 清空 c，准备重新拼当前单词的摩尔斯码

        for (char ch : b)
        {
            c += dic1[ch];
        }

        // push_back 表示在 vector 的末尾加入一个元素
        words.push_back(b);
        codes.push_back(c);

        cin >> b;
    }

    // 第三部分：处理查询
    string s;
    cin >> s;
    while (s != "*")
    {
        int exactCnt = 0;     // 完全匹配的个数
        string exactAns = ""; // 保存“完全匹配中应输出的答案”

        int bestDiff = 1000000; // 保存最小长度差
        string bestAns = "";    // 保存“最接近匹配”的答案

        for (int i = 0; i < (int)words.size(); i++)
        {
            // 情况1：完全匹配
            if (codes[i] == s)
            {
                exactCnt++;

                /*
                    为什么要先判断 exactAns == "" ？

                    因为一开始 exactAns 是空串，表示“还没有选中过答案”。

                    如果不先判断 exactAns == ""，
                    那么第一次匹配时去比较：

                        words[i].size() < exactAns.size()

                    由于 exactAns 是空串，exactAns.size() == 0，
                    而正常单词长度都大于 0，
                    条件就永远不成立，导致第一个匹配单词存不进去。

                    所以必须先判断：
                    如果 exactAns 还是空串，就先把当前单词存进去；
                    否则再比较长度，保留更短的那个。
                */
                if (exactAns == "" || words[i].size() < exactAns.size())
                {
                    exactAns = words[i];
                }
            }
            else
            {
                /*
                    情况2：不是完全匹配
                    题目要求：错误只可能发生在“末尾”
                    也就是：
                    - 查询串比正确串短了一截
                    或
                    - 查询串比正确串长了一截

                    这说明：两个串在前面较短的那一部分必须完全相同，
                    也就是一个串必须是另一个串的前缀。
                */
                int len1 = s.size();
                int len2 = codes[i].size();
                int minlen = min(len1, len2);

                /*
                    substr 的作用：截取子串

                    用法：
                        字符串.substr(起始位置, 长度)

                    例如：
                        string x = "abcdef";
                        x.substr(0, 3) 就是 "abc"

                    这里：
                        s.substr(0, minlen)
                    表示取 s 的前 minlen 个字符

                        codes[i].substr(0, minlen)
                    表示取 codes[i] 的前 minlen 个字符

                    如果这两段相同，
                    就说明两个串在较短长度范围内完全一致，
                    也就是其中一个是另一个的前缀，
                    符合“只在结尾多/少一些符号”的条件。
                */
                if (s.substr(0, minlen) == codes[i].substr(0, minlen))
                {
                    int diff = abs(len1 - len2); // 长度差越小，说明越接近

                    if (diff < bestDiff)
                    {
                        bestDiff = diff;
                        bestAns = words[i];
                    }
                }
            }
        }

        // 按题目要求输出
        if (exactCnt == 1)
        {
            cout << exactAns << endl;
        }
        else if (exactCnt > 1)
        {
            cout << exactAns << "!" << endl;
        }
        else
        {
            cout << bestAns << "?" << endl;
        }

        cin >> s;
    }

    return 0;
}