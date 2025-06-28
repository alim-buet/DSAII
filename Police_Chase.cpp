#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <deque>
#include <bitset>
#include <numeric>
#include <limits>
#include <cstring>
#include <cassert>
#include <list>
#include <tuple>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
#define yes cout << "YES" << endl
#define no cout << "NO" << endl
#define vi vector<int>
#define vl vector<long long>
#define pii pair<int, int>
#define pll pair<long long, long long>
#define ll long long
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
void solve()
{
    int n, m;
    cin >> n >> m;
    vector<vi> capacity(n + 1, vi(n + 1, 0));
    vector<vector<int>> flow(n + 1, vector<int>(n + 1, 0));

    vector<vi> graph(n + 1);
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        capacity[u][v] = 1;
        capacity[v][u] = 1;
        graph[u].push_back(v);
        graph[v].push_back(u);
        edges.push_back({u, v});
    }
    int totalFlow = 0;
    vector<int> parents(n + 1, -1);
    while (bfs(capacity, graph, 1, n, parents))
    {
        totalFlow++;
        int curr = n;
        while (curr != 1)
        {
            int prev = parents[curr];
            capacity[prev][curr] -= 1;
            capacity[curr][prev] += 1;
            flow[prev][curr] += 1;
            flow[curr][prev] -= 1; // for reverse flow
            curr = prev;
        }
    }
    cout << totalFlow << endl;
    vi visited(n + 1, 0);
    vector<pair<int, int>> minCutEdges;
    visited[1] = 1;
    queue<int> q;
    q.push(1);
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        for (int nbr : graph[node])
        {
            if (capacity[node][nbr] > 0 && !visited[nbr])
            {
                visited[nbr] = 1;
                q.push(nbr);
            }
        }
    }
    for (int i = 0; i < m; i++)
    {
        int u = edges[i].first;
        int v = edges[i].second;
        if (visited[u] && visited[v] == 0)
        {
            minCutEdges.push_back({u, v});
        }
        else if (visited[v] && visited[u] == 0)
        {
            minCutEdges.push_back({v, u});
        }
    }
    for (auto p : minCutEdges)
    {
        cout << p.first << " " << p.second << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    while (t--)
    {
        solve();
    }
    return 0;
}