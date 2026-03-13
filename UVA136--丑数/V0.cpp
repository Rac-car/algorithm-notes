#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <functional>
using namespace std;

typedef long long LL;
const int coeff[3] = {2, 3, 5};

int main()
{
    // 小根堆：top() 取当前最小丑数
    priority_queue<LL, vector<LL>, greater<LL>> pq;
    set<LL> s; // 判重，避免同一个丑数重复进入堆

    pq.push(1);
    s.insert(1);

    for (int i = 1;; i++)
    {
        LL x = pq.top();
        pq.pop();

        if (i == 1500)
        {
            cout << "The 1500'th ugly number is " << x << ".\n";
            break;
        }

        for (int j = 0; j < 3; j++)
        {
            LL x2 = x * coeff[j];

            // 这里可能会提前生成一些比第1500个更大的数，
            // 但它们并不是立刻成为“第几个丑数”，因为前面还有很多更小的数没弹出来。
            if (!s.count(x2))
            {
                s.insert(x2);
                pq.push(x2);
            }
        }
    }

    return 0;
}