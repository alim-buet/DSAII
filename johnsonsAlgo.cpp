#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Edge
{
    int from, to, weight;
};

const int INF = INT_MAX;

bool bellmanFord(int V, int src, const vector<Edge> &edges, vector<int> &h)
{
    h.assign(V + 1, INF);
    h[src] = 0;

    for (int i = 0; i < V; ++i)
    {
        for (const auto &edge : edges)
        {
            if (h[edge.from] != INF && h[edge.from] + edge.weight < h[edge.to])
            {
                h[edge.to] = h[edge.from] + edge.weight;
            }
        }
    }

    // Check for negative cycles
    for (const auto &edge : edges)
    {
        if (h[edge.from] != INF && h[edge.from] + edge.weight < h[edge.to])
        {
            return false; // Negative weight cycle found
        }
    }

    return true;
}

void dijkstra(int src, int V, const vector<vector<pair<int, int>>> &adj, vector<int> &dist)
{
    dist.assign(V, INF);
    dist[src] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, src});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u])
            continue;

        for (auto [v, weight] : adj[u])
        {
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
}

void johnsonsAlgorithm(int V, const vector<Edge> &edges)
{
    vector<Edge> newEdges = edges;

    // Step 1: Add vertex q (index V) with 0-weight edges to all others
    for (int i = 0; i < V; ++i)
        newEdges.push_back({V, i, 0});

    vector<int> h;
    if (!bellmanFord(V, V, newEdges, h))
    {
        cout << "Graph contains a negative weight cycle.\n";
        return;
    }

    // Step 2: Reweight edges
    vector<vector<pair<int, int>>> adj(V);
    for (const auto &e : edges)
    {
        int newWeight = e.weight + h[e.from] - h[e.to];
        adj[e.from].emplace_back(e.to, newWeight);
    }

    // Step 3: Run Dijkstra from each vertex
    for (int u = 0; u < V; ++u)
    {
        vector<int> dist;
        dijkstra(u, V, adj, dist);

        cout << "Shortest paths from vertex " << u << ":\n";
        for (int v = 0; v < V; ++v)
        {
            if (dist[v] == INF)
            {
                cout << "  to " << v << " = INF\n";
            }
            else
            {
                int actualDist = dist[v] - h[u] + h[v];
                cout << "  to " << v << " = " << actualDist << "\n";
            }
        }
    }
}

int main()
{
    int V = 5;
    vector<Edge> edges = {
        {0, 1, 3}, {0, 2, 8}, {0, 4, -4}, {1, 3, 1}, {1, 4, 7}, {2, 1, 4}, {3, 0, 2}, {3, 2, -5}, {4, 3, 6}};

    johnsonsAlgorithm(V, edges);
    return 0;
}
