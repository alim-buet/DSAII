#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj, capacity;
vector<int> parent;

bool bfs(int s, int t, int n)
{
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty())
    {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur])
        {
            if (parent[next] == -1 && capacity[cur][next] > 0)
            {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                {
                    return true;
                }
                q.push({next, new_flow});
            }
        }
    }
    return false;
}

int maxflow(int s, int t, int n)
{
    int flow = 0;
    while (bfs(s, t, n))
    {
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            path_flow = min(path_flow, capacity[u][v]);
        }

        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            capacity[u][v] -= path_flow;
            capacity[v][u] += path_flow;
        }
        flow += path_flow;
    }
    return flow;
}

void findPath(int s, int t, int n, vector<int> &path)
{
    vector<bool> visited(n + 1, false);
    path.clear();

    function<bool(int)> dfs = [&](int u) -> bool
    {
        visited[u] = true;
        path.push_back(u);

        if (u == t)
        {
            return true;
        }

        for (int v : adj[u])
        {
            if (!visited[v] && capacity[v][u] > 0)
            { // reverse edge has flow
                if (dfs(v))
                {
                    return true;
                }
            }
        }

        path.pop_back();
        return false;
    };

    dfs(s);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    adj.resize(n + 1);
    capacity.resize(n + 1, vector<int>(n + 1, 0));
    parent.resize(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a); // for reverse edge
        capacity[a][b] = 1;
    }

    int max_days = maxflow(1, n, n);
    cout << max_days << "\n";

    // Reconstruct paths
    vector<int> path;
    for (int day = 0; day < max_days; day++)
    {
        findPath(1, n, n, path);
        cout << path.size() << "\n";
        for (int i = 0; i < path.size(); i++)
        {
            if (i > 0)
                cout << " ";
            cout << path[i];
        }
        cout << "\n";

        // Remove this path from the residual graph
        for (int i = 0; i < path.size() - 1; i++)
        {
            int u = path[i], v = path[i + 1];
            capacity[v][u]--; // reduce reverse flow
            capacity[u][v]++; // restore forward capacity
        }
    }

    return 0;
}