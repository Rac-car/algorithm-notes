#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>

using namespace std;

const int MAXN = 10005;
const int MAXM = 15;

int a[MAXN][MAXM];
map<string, int> id;
int tot;

int getID(const string &s)
{
    if (id.count(s))
        return id[s];
    id[s] = ++tot;
    return tot;
}

int main()
{
    int n, m;

    while (cin >> n >> m)
    {
        cin.get(); // 读掉这一行最后的换行符，避免后续的getline()读取

        id.clear();
        tot = 0;

        // 读入整个表，并把每个字符串转成整数编号
        for (int i = 1; i <= n; i++)
        {
            string line;
            getline(cin, line);

            stringstream ss(line);
            string cell;
            int j = 1;

            while (getline(ss, cell, ','))
            {
                a[i][j] = getID(cell);
                j++;
            }
        }

        bool ok = true;

        // 枚举两列 c1, c2
        for (int c1 = 1; c1 <= m && ok; c1++)
        {
            for (int c2 = c1 + 1; c2 <= m && ok; c2++)
            {
                map<pair<int, int>, int> mp; // 二元组，存在于<utility>，可以通过.first和.second访问元素

                // 扫描每一行
                for (int r = 1; r <= n; r++)
                {
                    pair<int, int> key = make_pair(a[r][c1], a[r][c2]); // 二元组赋值

                    if (mp.count(key))
                    {
                        cout << "NO\n";
                        cout << mp[key] << " " << r << "\n";
                        cout << c1 << " " << c2 << "\n";
                        ok = false;
                        break;
                    }
                    else
                    {
                        mp[key] = r;
                    }
                }
            }
        }

        if (ok)
        {
            cout << "YES\n";
        }
    }

    return 0;
}