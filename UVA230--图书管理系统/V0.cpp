#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

struct Book
{
    string title;
    string author;

    bool operator<(const Book &other) const
    {
        if (author != other.author)
            return author < other.author;
        return title < other.title;
    }
};

int main()
{
    vector<Book> books;
    map<string, int> mp;
    string line;

    // 读入所有书
    while (getline(cin, line))
    {
        if (line == "END")
            break;

        // 在字符串 line 里查找子串 " by 第一次出现的位置，并把这个位置下标存到 pos 里。
        // 这个位置指 " 的位置
        int pos = line.find("\" by ");
        string title = line.substr(1, pos - 1); // 去掉前面的 "，pos-1指的是长度不是坐标
        string author = line.substr(pos + 5);   // 跳过 " by "

        Book b;
        b.title = title;
        b.author = author;
        books.push_back(b);
    }

    // 按作者、书名排序
    sort(books.begin(), books.end());

    // 建立 书名 -> 排序后编号 的映射
    for (int i = 0; i < (int)books.size(); i++)
    {
        mp[books[i].title] = i;
    }

    // 状态：
    // 0 = 在书架上
    // 1 = 被借走
    // 2 = 已归还但还没上架
    vector<int> state(books.size(), 0);

    // 保存待上架的书
    vector<int> returned;

    // 处理操作
    while (getline(cin, line))
    {
        if (line == "END")
            break;

        if (line.substr(0, 6) == "BORROW")
        {
            int l = line.find("\"");
            int r = line.rfind("\"");
            string title = line.substr(l + 1, r - l - 1);

            int id = mp[title];
            state[id] = 1;
        }
        else if (line.substr(0, 6) == "RETURN")
        {
            int l = line.find("\"");
            int r = line.rfind("\"");
            string title = line.substr(l + 1, r - l - 1);

            int id = mp[title];
            state[id] = 2;
            returned.push_back(id);
        }
        else if (line == "SHELVE")
        {
            sort(returned.begin(), returned.end());

            for (int i = 0; i < (int)returned.size(); i++)
            {
                int id = returned[i];

                int j = id - 1;
                while (j >= 0 && state[j] != 0)
                {
                    j--;
                }

                if (j < 0)
                {
                    cout << "Put \"" << books[id].title << "\" first\n";
                }
                else
                {
                    cout << "Put \"" << books[id].title
                         << "\" after \"" << books[j].title << "\"\n";
                }

                state[id] = 0;
            }

            returned.clear();
            cout << "END\n";
        }
    }

    return 0;
}