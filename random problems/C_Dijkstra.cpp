#include <bits/stdc++.h>
using namespace std;
#define yes cout << "YES" << endl;
#define no cout << "NO" << endl;
#define vi vector<ll>
#define ll long long
#define vl vector<long long>
#define fori                    \
    for (ll i = 0; i < n; i++) \
    {                           \
        cin >> arr[i];          \
    }

void solve()
{
    ll n, m;
    cin >> n >> m;
    const ll inf = 1e18;
    vector<vector<pair<ll, ll>>> graph(n + 1);
    // u -> v,w
    // cout << "hello" << endl;
    for (ll i = 0; i < m; i++)
    {
        ll u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }
    vector<ll> minDist(n + 1, inf);
    vector<ll> parent(n + 1, -1);
    vector<ll> visited(n + 1, 0);
    minDist[1] = 0;
    // dijkstra apply
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;
    // wt, and kar weight(v)
    pq.push({0, 1});
    while (!pq.empty())
    {
        pair<ll, ll> p = pq.top();
        ll u = p.second;
        ll totalCost = p.first;
        pq.pop();
        if (visited[u])
            continue;
        visited[u] = 1;
        for (pair<ll, ll> adj : graph[u])
        {
            ll v = adj.first;

            ll wt = adj.second;
            if (totalCost + wt < minDist[v])
            {
                minDist[v] = totalCost + wt;
                parent[v] = u;
                pq.push({minDist[v], v});
            }
        }
    }
    stack<ll> path;
    path.push(n);
    ll i = n;
    while (parent[i] != -1)
    {
        i = parent[i];
        path.push(i);
    }
    if (i == 1)
    {
        while (!path.empty())
        {
            cout << path.top() << " ";
            path.pop();
        }
        cout << endl;
    }
    else
        cout << -1 << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}