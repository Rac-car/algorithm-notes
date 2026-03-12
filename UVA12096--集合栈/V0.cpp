#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <string>
#include <algorithm> // 使用了set_union()与set_intersection()
#include <iterator>  // 使用了inserter()
#define ALL(x) x.begin(), x.end()
#define INS(x) inserter(x, x.begin())
using namespace std;

// 由于集合都是空集，集合保存包含集合的ID（类似指针） ，整个集合又通过下述的Setcache有一个自己的ID
// 查询任意的集合，最终都会指向ID==0，也就是Setcache[0]一定是{}
typedef set<int> Set;

stack<int> s;          // 通过栈操作获得栈中存储的ID
vector<Set> Setcache;  // 通过输入ID获取数组中存储的集合
map<Set, int> IDcache; // 通过输入集合获取对应的ID

// 查询ID，若未查询到则新建对应ID
int ID(Set x)
{
    if (IDcache.count(x))
        return IDcache[x];
    Setcache.push_back(x);
    return IDcache[x] = Setcache.size() - 1;
}

int main()
{
    int n;
    cin >> n;
    while (n--)
    {
        string op;
        cin >> op;
        if (op[0] == 'P')
        {
            s.push(ID(Set()));
        }
        else if (op[0] == 'D')
        {
            s.push(s.top());
        }
        else
        {
            Set x1 = Setcache[s.top()];
            s.pop();
            Set x2 = Setcache[s.top()];
            s.pop();
            Set x;
            if (op[0] == 'U')
            {
                set_union(ALL(x1), ALL(x2), INS(x));
            }
            else if (op[0] == 'I')
            {
                set_intersection(ALL(x1), ALL(x2), INS(x));
            }
            else if (op[0] == 'A')
            {
                x = x2;
                x.insert(ID(x1));
            }
            s.push(ID(x));
        }
        cout << Setcache[s.top()].size() << endl;
    }
}