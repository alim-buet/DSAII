#include <bits/stdc++.h>
using namespace std;

void prims(vector<vector<pair<int, int>>> &graph, vector<pair<int, int>> &ans, vector<int> &visited, int &totalweight)
{
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> minpq;
    // priority_queue stores: {weight, {parent, node}}

    minpq.push({0, {-1, 0}}); // starting from node 0

    while (!minpq.empty())
    {
        auto p = minpq.top();
        minpq.pop();

        int wt = p.first;
        int par = p.second.first;
        int node = p.second.second;

        if (visited[node])
            continue;
        visited[node] = 1;

        if (par != -1)
        {
            ans.push_back({par, node});
            totalweight += wt;
        }

        for (auto &[nbr, edgeWt] : graph[node])
        {
            if (!visited[nbr])
            {
                minpq.push({edgeWt, {node, nbr}});
            }
        }
    }
}

int main()
{
    int v, e;
    cin >> v >> e;

    vector<vector<pair<int, int>>> graph(v); // graph[u] = { {v, weight}, ... }

    for (int i = 0; i < e; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // undirected graph
    }

    vector<pair<int, int>> ans;
    int totalWeight = 0;
    vector<int> visited(v, 0);

    prims(graph, ans, visited, totalWeight);

    cout << "Edges in MST:\n";
    for (auto &[u, v] : ans)
    {
        cout << u << " - " << v << '\n';
    }
    cout << "Total weight of MST: " << totalWeight << '\n';

    return 0;
}
