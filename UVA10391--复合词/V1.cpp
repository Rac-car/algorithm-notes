#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> words;
    set<string> st;
    string s;

    // 读到 EOF 为止
    while (cin >> s)
    {
        words.push_back(s);
        st.insert(s);
    }

    for (int i = 0; i < words.size(); i++)
    {
        string word = words[i];
        int len = word.length();

        for (int j = 1; j < len; j++)
        {
            string left = word.substr(0, j);
            string right = word.substr(j);

            if (st.count(left) && st.count(right))
            {
                cout << word << '\n';
                break;
            }
        }
    }

    return 0;
}