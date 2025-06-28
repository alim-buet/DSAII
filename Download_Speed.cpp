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

const ll inf = 1e18;

ll bfs(vector<vl> &capacity, vector<vl> &graph, vl &parents, ll src, ll sink)
{
    ll n = graph.size();
    for (ll i = 0; i < n; i++)
    {
        parents[i] = -1;
    }
    parents[src] = -2;
    queue<pair<ll, ll>> q;
    q.push(make_pair(src, inf));
    while (!q.empty())
    {
        pair<ll, ll> p = q.front();
        q.pop();
        ll prevNode = p.first;
        ll flow_till = p.second;
        ll curr_flow;
        for (ll i = 0; i < graph[prevNode].size(); ++i)
        {
            ll next = graph[prevNode][i];
            if (parents[next] == -1 && capacity[prevNode][next] > 0)
            {
                parents[next] = prevNode;
                curr_flow = min(flow_till, capacity[prevNode][next]);
                if (next == sink)
                {
                    return curr_flow;
                }
                q.push(make_pair(next, curr_flow));
            }
        }
    }
    return 0;
}

void solve()
{
    ll n, m;
    cin >> n >> m;
    vector<vl> capacity(n + 1, vl(n + 1, 0));
    vector<vl> graph(n + 1);
    ll totalFlow = 0;
    for (ll i = 0; i < m; i++)
    {
        ll u, v, c;
        cin >> u >> v >> c;
        capacity[u][v] += c;
        graph[u].push_back(v);
        graph[v].push_back(u); // needed for residual edges
    }
    vl parents(n + 1, -1);
    ll currFlow;
    while ((currFlow = bfs(capacity, graph, parents, 1, n)))
    {
        totalFlow += currFlow;
        ll currNode = n;
        while (currNode != 1)
        {
            ll prevNode = parents[currNode];
            capacity[prevNode][currNode] -= currFlow;
            capacity[currNode][prevNode] += currFlow;
            currNode = prevNode;
        }
    }
    cout << totalFlow << endl;
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
