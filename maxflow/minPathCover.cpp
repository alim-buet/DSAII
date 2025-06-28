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
    q.push({s, INF});

    while (!q.empty())
    {
        int node = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int nbr : graph[node])
        {
            if (parent[nbr] == -1 && capacity[node][nbr] > 0)
            {
                parent[nbr] = node;
                int new_flow = min(flow, capacity[node][nbr]);
                if (nbr == t)
                    return new_flow;
                q.push({nbr, new_flow});
            }
        }
    }
    return 0;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int V, E;
    cin >> V >> E;

    // Nodes:
    // For each original vertex v: create v_L and v_R
    // Total nodes = 2*V + 2 (S, T)
    int S = 2 * V;
    int T = 2 * V + 1;
    int total_nodes = 2 * V + 2;

    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> graph(total_nodes);
    vector<int> parent(total_nodes);

    auto add_edge = [&](int u, int v, int cap)
    {
        if (capacity[u][v] == 0)
        {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        capacity[u][v] += cap;
    };

    // Read edges of DAG and build bipartite flow graph
    for (int i = 0; i < E; ++i)
    {
        int u, v;
        cin >> u >> v;
        add_edge(u, v + V, 1); // from u_L to v_R
    }

    // Connect source to all left vertices
    for (int u = 0; u < V; ++u)
        add_edge(S, u, 1);

    // Connect all right vertices to sink
    for (int v = 0; v < V; ++v)
        add_edge(v + V, T, 1);

    // Max Flow
    int flow = 0, new_flow;
    while ((new_flow = bfs(capacity, graph, S, T, parent)))
    {
        flow += new_flow;
        int curr = T;
        while (curr != S)
        {
            int prev = parent[curr];
            capacity[prev][curr] -= new_flow;
            capacity[curr][prev] += new_flow;
            curr = prev;
        }
    }

    cout << "Minimum Path Cover = " << (V - flow) << "\n";
    return 0;
}
