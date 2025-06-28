#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int INF = 1e9;
int n;

// Directions: right, down, left, up
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

// Mapping (i,j) → unique id
int cellID(int i, int j)
{
    return i * n + j;
}

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

    int m;
    cin >> n >> m; // grid size and number of start points

    vector<pair<int, int>> starts(m);
    for (int i = 0; i < m; ++i)
    {
        int x, y;
        cin >> x >> y;
        starts[i] = {x - 1, y - 1}; // 0-based indexing
    }

    int total_nodes = 2 * n * n + 2;
    int S = total_nodes - 2;
    int T = total_nodes - 1;

    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> graph(total_nodes);
    vector<pair<int, int>> edgeList;

    auto add_edge = [&](int u, int v, int cap)
    {
        if (capacity[u][v] == 0)
        { // avoid duplicate in graph list
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        capacity[u][v] += cap;
        edgeList.push_back({u, v});
    };

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int id = cellID(i, j);
            int inNode = 2 * id;
            int outNode = 2 * id + 1;

            // Capacity 1 through the cell
            add_edge(inNode, outNode, 1);

            // Neighbor connections
            for (int d = 0; d < 4; ++d)
            {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if (ni >= 0 && ni < n && nj >= 0 && nj < n)
                {
                    int nid = cellID(ni, nj);
                    int neighborIn = 2 * nid;
                    add_edge(outNode, neighborIn, 1);
                }
            }

            // Boundary to sink
            if (i == 0 || j == 0 || i == n - 1 || j == n - 1)
            {
                add_edge(outNode, T, 1);
            }
        }
    }

    // Source to starts
    for (auto [x, y] : starts)
    {
        int sid = cellID(x, y);
        int startIn = 2 * sid;
        add_edge(S, startIn, 1);
    }

    // Maxflow
    vector<int> parent(total_nodes);
    int total_flow = 0, new_flow;

    while ((new_flow = bfs(capacity, graph, S, T, parent)))
    {
        total_flow += new_flow;
        int curr = T;
        while (curr != S)
        {
            int prev = parent[curr];
            capacity[prev][curr] -= new_flow;
            capacity[curr][prev] += new_flow;
            curr = prev;
        }
    }

    cout << (total_flow == m ? "YES\n" : "NO\n");
    return 0;
}
