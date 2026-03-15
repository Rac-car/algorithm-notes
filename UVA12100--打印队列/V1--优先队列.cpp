// 比较器写法：
// 1.自定义比较器结构体的仿函数
/*
struct Node {
    int x;
    int y;
};

struct cmp {
    bool operator()(const Node& a, const Node& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

priority_queue<Node, vector<Node>, cmp> pq;
*/

// 2.重载 operator<
/*
struct Node {
    int x;
    int y;

    bool operator<(const Node& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

priority_queue<Node> pq;
*/

#include <iostream>
#include <queue>
using namespace std;

int main()
{
    int T;
    cin >> T;

    while (T--)
    {
        int n, m;
        cin >> n >> m;

        queue<pair<int, int>> q; // first: 优先级, second: 原下标
        priority_queue<int> pq;  // 维护当前最高优先级，使用大根堆
        // 完整写法：priority_queue<元素类型, 底层容器, 比较器>
        // priority_queue<int, vector<int>, greater<int> >表示小根堆

        for (int i = 0; i < n; i++)
        {
            int x;
            cin >> x;
            q.push(make_pair(x, i));
            pq.push(x);
        }

        int ans = 0;

        while (!q.empty())
        {
            pair<int, int> cur = q.front();
            q.pop();

            if (cur.first == pq.top())
            {
                ans++;
                pq.pop();

                if (cur.second == m)
                {
                    cout << ans << endl;
                    break;
                }
            }
            else
            {
                q.push(cur);
            }
        }
    }

    return 0;
}