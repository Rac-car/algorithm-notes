#include <cstdio>
#include <stack>
using namespace std;

const int MAXN = 1000 + 10;

int n, target[MAXN];

int main()
{
    while (scanf("%d", &n) == 1)
    {
        stack<int> s;
        int A = 1, B = 1; // A表示将入栈的数字，B表示出栈队列下一个需要匹配的ID
        for (int i = 1; i <= n; i++)
            scanf("%d", &target[i]);

        int ok = 1;
        while (B <= n)
        {
            if (A == target[B])
            {
                A++;
                B++;
            }
            else if (!s.empty() && s.top() == target[B])
            {
                s.pop();
                B++;
            }
            else if (A <= n)
            {
                s.push(A++);
            }
            else
            {
                ok = 0;
                break;
            }
        }

        printf("%s\n", ok ? "Yes" : "No");
    }

    return 0;
}