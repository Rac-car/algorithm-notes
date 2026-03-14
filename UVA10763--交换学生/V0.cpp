#include <iostream>
#include <map>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n != 0)
    {
        map<pair<int, int>, int> mp;

        for (int i = 0; i < n; i++)
        {
            int a, b;
            cin >> a >> b;

            pair<int, int> now = {a, b};
            pair<int, int> rev = {b, a};

            if (mp.count(rev))
            {
                mp[rev]--;
                if (mp[rev] == 0)
                {
                    mp.erase(rev);
                }
            }
            else
            {
                mp[now]++;
            }
        }

        if (mp.empty())
        {
            cout << "YES\n";
        }
        else
        {
            cout << "NO\n";
        }
    }

    return 0;
}