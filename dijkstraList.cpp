#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define pii pair<int, int>
const int INF = INT_MAX;

void dijkstra(vector<vector<pii>> &graph, vi &minDistances, int source)
{
    int n = graph.size();
    vector<bool> visited(n, false);
    priority_queue<pii, vector<pii>, greater<pii>> minPQ;
    minDistances[source] = 0;
    minPQ.push({0, source});

    while (!minPQ.empty())
    {
        int u = minPQ.top().second;
        minPQ.pop();

        if (visited[u])
            continue;
        visited[u] = true;

        for (auto &[v, w] : graph[u])
        {
            if (minDistances[u] + w < minDistances[v])
            {
                minDistances[v] = minDistances[u] + w;
                minPQ.push({minDistances[v], v});
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;

    vector<vector<pii>> graph(n); // {neighbor, weight}
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    vi minDistances(n, INF);
    int source;
    cin >> source;
    dijkstra(graph, minDistances, source);

    for (int i = 0; i < n; i++)
    {
        cout << i << " : ";
        if (minDistances[i] == INF)
            cout << "INF";
        else
            cout << minDistances[i];
        cout << "\n";
    }

    return 0;
}
