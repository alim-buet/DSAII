#include <bits/stdc++.h>
using namespace std;

#define pii pair<int, int>
const int INF = 1e9;

void solve(int tc)
{
    cin.ignore(); 
    int n, m;
    cin >> n >> m;

    vector<vector<pii>> graph(n);
    for (int i = 0; i < m; ++i)
    {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // undirected
    }

    int t;
    cin >> t;

    // Modified Dijkstra (minimize max edge on path)
    vector<int> cost(n, INF);
    cost[t] = 0;

    priority_queue<pii, vector<pii>, greater<>> pq;
    pq.push({0, t}); // {cost, node}

    while (!pq.empty())
    {
        auto p = pq.top();
        int curCost = p.first;
        int u = p.second;

        pq.pop();

        for (auto nbr : graph[u])
        {
            int v = nbr.first;
            int w = nbr.second;
            int pathCost = max(curCost, w);
            if (pathCost < cost[v])
            {
                cost[v] = pathCost;
                pq.push({pathCost, v});
            }
        }
    }

    cout << "Case " << tc << ":\n";
    for (int i = 0; i < n; ++i)
    {
        if (cost[i] == INF)
            cout << "Impossible\n";
        else
            cout << cost[i] << '\n';
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (int tc = 1; tc <= T; ++tc)
    {
        solve(tc);
    }
    return 0;
}
