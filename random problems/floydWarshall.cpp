#include <iostream>
#include <vector>
#include <stack>
using namespace std;

#define ll long long
const ll INF = 1e15;

void solve()
{
    int n, m;
    cin >> n >> m;

    vector<vector<ll>> minWeight(n + 1, vector<ll>(n + 1, INF));
    vector<vector<int>> parent(n + 1, vector<int>(n + 1, -1));

    for (int i = 1; i <= n; ++i)
        minWeight[i][i] = 0;

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        if (w < minWeight[u][v])
        { // multiple edge considered
            minWeight[u][v] = w;
            parent[u][v] = u;
        }
    }

    // run floyd
    for (int k = 1; k <= n; ++k)
    {
        for (int i = 1; i <= n; ++i)
        {
            for (int j = 1; j <= n; ++j)
            {
                if (minWeight[i][k] < INF && minWeight[k][j] < INF)
                {
                    if (minWeight[i][j] > minWeight[i][k] + minWeight[k][j])
                    {
                        minWeight[i][j] = minWeight[i][k] + minWeight[k][j];
                        parent[i][j] = parent[k][j];
                    }
                }
            }
        }
    }

    // neg cycle-> no need to proceed
    for (int i = 1; i <= n; ++i)
    {
        if (minWeight[i][i] < 0)
        {
            cout << "NO PATH. There is a negative cycle" << endl;
            return;
        }
    }

    int q;
    cin >> q;
    while (q--)
    {
        int u, v;
        cin >> u >> v;

        if (minWeight[u][v] == INF)
        {
            cout << "NO PATH" << endl;
        }
        else
        {
            cout << "Min path cost: " << minWeight[u][v] << endl;
            cout << "Shortest path: ";
            stack<int> path;
            int curr = v;
            while (curr != u)
            {
                if (curr == -1)
                    break;
                path.push(curr);
                curr = parent[u][curr];
            }
            if (curr == -1)
            {
                cout << "NO PATH" << endl;
            }
            else
            {
                path.push(u);
                while (!path.empty())
                {
                    cout << path.top() << " ";
                    path.pop();
                }
                cout << endl;
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}
