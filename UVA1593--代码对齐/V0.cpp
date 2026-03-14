#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

vector<vector<string>> codes; // 相邻的<、>一定要用空格隔开！
vector<int> maxlen;

void print(const string &s, int len)
{
    cout << s;
    for (int i = 0; i < len - (int)s.size(); ++i)
        cout << ' ';
}

int main()
{
    ios::sync_with_stdio(0); // 为了加速输入输出，无需加入头文件
    // 作用是：取消 C++ 的 cin/cout 和 C 的 scanf/printf 之间的同步；因此适用于只适用C++时

    string s, word;
    int row = 0;
    while (getline(cin, s))
    {
        stringstream ss(s);
        int j = 0;
        codes.push_back(vector<string>());
        while (ss >> word)
        {
            if (j < maxlen.size())
            {
                if (word.length() > maxlen[j])
                {
                    maxlen[j] = word.length();
                }
            }
            else
            {
                maxlen.push_back(word.length());
            }
            codes[row].push_back(word);
            j++;
        }
        row++;
    }
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < (int)codes[i].size(); ++j)
        {
            if (j == (int)codes[i].size() - 1)
            {
                print(codes[i][j], (int)codes[i][j].size());
            }
            else
            {
                print(codes[i][j], maxlen[j] + 1);
            }
        }
        cout << '\n';
    }
    return 0;
}