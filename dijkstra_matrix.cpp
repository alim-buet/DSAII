#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define pii pair<int, int>
const int INF = INT_MAX;

void dijkstra(vector<vi> &graph, vi &minDistances, int source)
{
    int n = graph.size();
    vector<bool> visited(n, false);
    minDistances[source] = 0;
    priority_queue<pii, vector<pii>, greater<pii>> minPq;
    minPq.push({0, source});

    while (!minPq.empty())
    {
        int u = minPq.top().second;
        minPq.pop();

        if (visited[u])
            continue;
        visited[u] = true;

        for (int v = 0; v < n; ++v)
        {
            if (graph[u][v] != INF && minDistances[u] + graph[u][v] < minDistances[v])
            {
                minDistances[v] = minDistances[u] + graph[u][v];
                minPq.push({minDistances[v], v});
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

    vector<vi> graph(n, vi(n, INF));
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u][v] = min(graph[u][v], w); // for multiedges
    }

    int source;
    cin >> source;

    vi minDistance(n, INF);
    dijkstra(graph, minDistance, source);

    for (int i = 0; i < n; i++)
    {
        cout << i << " : " << (minDistance[i] == INF ? -1 : minDistance[i]) << "\n";
    }

    return 0;
}
