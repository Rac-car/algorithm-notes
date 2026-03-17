/*
prog[i]：第 i 个程序的所有语句
pc[i]：第 i 个程序当前执行到哪一行
val[26]：共享变量 a~z
deque<int> ready：就绪队列
deque<int> blocked：阻塞队列
bool locked：当前锁是否被占用
*/

#include <iostream>
#include <vector>
#include <string>
#include <deque>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t1, t2, t3, t4, t5, q;
    cin >> n >> t1 >> t2 >> t3 >> t4 >> t5 >> q;

    string line;
    getline(cin, line); // 吃掉第一行末尾换行

    vector<vector<string>> prog(n);
    for (int i = 0; i < n; i++)
    {
        while (getline(cin, line))
        {
            prog[i].push_back(line);
            if (line == "end")
                break;
        }
    }

    int val[26] = {0};         // 共享变量 a~z
    vector<int> pc(n, 0);      // 每个程序当前执行到哪一行
    deque<int> ready, blocked; // 就绪队列、阻塞队列

    for (int i = 0; i < n; i++)
        ready.push_back(i);

    bool locked = false;

    while (!ready.empty())
    {
        int id = ready.front();
        ready.pop_front();

        int left = q;
        bool finish = false;
        bool block = false;

        while (left > 0)
        {
            string s = prog[id][pc[id]];

            if (s.find('=') != string::npos) // s.sfind()返回找到的位置，如果没有找到则返回string::npos
            {                                // 赋值
                char var = s[0];
                int num = 0;
                int pos = s.find('=');
                for (int i = pos + 1; i < (int)s.size(); i++)
                {
                    if (s[i] >= '0' && s[i] <= '9')
                    {
                        num = num * 10 + (s[i] - '0');
                    }
                }
                val[var - 'a'] = num;
                left -= t1;
                pc[id]++;
            }
            else if (s[0] == 'p')
            { // print
                char var = s[s.size() - 1];
                cout << id + 1 << ": " << val[var - 'a'] << '\n';
                left -= t2;
                pc[id]++;
            }
            else if (s == "lock")
            { // lock
                left -= t3;
                if (locked)
                {
                    blocked.push_back(id);
                    block = true;
                    break;
                }
                else
                {
                    locked = true;
                    pc[id]++;
                }
            }
            else if (s == "unlock")
            { // unlock
                locked = false;
                left -= t4;
                pc[id]++;

                if (!blocked.empty())
                {
                    ready.push_front(blocked.front());
                    blocked.pop_front();
                }
            }
            else if (s == "end")
            { // end
                left -= t5;
                pc[id]++;
                finish = true;
                break;
            }
        }

        if (!finish && !block)
        {
            ready.push_back(id);
        }
    }

    return 0;
}