#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int bfs(vector<vector<int>> &capacity, vector<vector<int>> &graph, int s, int t, vector<int> &parent)
{
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push(make_pair(s, INF));

    while (!q.empty())
    {
        int node = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int i = 0; i < graph[node].size(); ++i)
        {
            int nbr = graph[node][i];
            if (parent[nbr] == -1 && capacity[node][nbr] > 0)
            {
                parent[nbr] = node;
                int new_flow = min(flow, capacity[node][nbr]);
                if (nbr == t)
                    return new_flow;
                q.push(make_pair(nbr, new_flow));
            }
        }
    }

    return 0;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> capacity(n + 1, vector<int>(n + 1, 0));
    vector<vector<int>> flow(n + 1, vector<int>(n + 1, 0));
    vector<vector<int>> graph(n + 1);
    vector<pair<int, int>> edgeList;

    for (int i = 0; i < m; ++i)
    {
        int u, v, c;
        cin >> u >> v >> c;
        capacity[u][v] += c; // handle multiple edges
        graph[u].push_back(v);
        graph[v].push_back(u); // for residual graph
        edgeList.push_back(make_pair(u, v));
    }

    int s = 1, t = n;
    int total_flow = 0;
    vector<int> parent(n + 1);

    int new_flow;
    while ((new_flow = bfs(capacity, graph, s, t, parent)))
    {
        total_flow += new_flow;
        int curr = t;
        while (curr != s)
        {
            int prev = parent[curr];
            capacity[prev][curr] -= new_flow;
            capacity[curr][prev] += new_flow;
            flow[prev][curr] += new_flow;
            flow[curr][prev] -= new_flow; // for reverse flow
            curr = prev;
        }
    }

    cout << "Total flow is: " << total_flow << "\n";
    cout << "Flow through each edge:\n";
    for (int i = 0; i < edgeList.size(); ++i)
    {
        int u = edgeList[i].first;
        int v = edgeList[i].second;
        cout << "Edge " << u << " -> " << v << " : " << flow[u][v] << "\n";
    }

    return 0;
}
