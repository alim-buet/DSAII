#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

#define vi vector<int>
#define pii pair<int, int>
const int INF = 1e9;

int bfs(vector<vector<int>> &capacity, vector<vector<int>> &graph, int s, int t, vector<int> &parent)
{
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pii> q;
    q.push({s, INF});

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
                q.push({nbr, new_flow});
            }
        }
    }

    return 0;
}

void solve()
{
    int n, m, k;
    cin >> n >> m >> k;

    int total_nodes = n + m + 2;
    int s = 0, t = n + m + 1;

    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> flow(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> graph(total_nodes);

    for (int i = 0; i < k; i++)
    {
        int b, g;
        cin >> b >> g;
        g += n; // map girl node to n+1...n+m

        capacity[b][g] = 1;
        graph[b].push_back(g);
        graph[g].push_back(b); // reverse edge
    }

    for (int i = 1; i <= n; i++)
    {
        graph[s].push_back(i);
        graph[i].push_back(s); // reverse edge
        capacity[s][i] = 1;
    }

    for (int i = n + 1; i <= n + m; i++)
    {
        graph[i].push_back(t);
        graph[t].push_back(i); // reverse edge
        capacity[i][t] = 1;
    }

    int total_flow = 0;
    vector<int> parent(total_nodes);

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
            flow[curr][prev] -= new_flow;
            curr = prev;
        }
    }

    cout << total_flow << endl;
    for (int b = 1; b <= n; ++b)
    {
        int found = 0;
        for (int g : graph[b])
        {
            if (found)
                continue;
            if (g >= n + 1 && g <= n + m && flow[b][g] == 1)
            {
                cout << b << " " << (g - n) << endl;
                found = 1;
            }
        }
        if (found)
            continue;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}
