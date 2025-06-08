#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include<queue>
using namespace std;

#define ll long long
#define pii pair<int, int>
#define pll pair<ll, ll>
#define vi vector<int>
#define vl vector<ll>
int prims(vector<vector<pii>> &graph)
{
    int n = graph.size();
    int totalWt = 0;
    vector<bool> inMst(n, false);
    vi minWt(n, INT32_MAX);
    priority_queue<pii, vector<pii>, greater<pii>> minpq;
    // minpq contains the minwt needed to connect the node to the current mst
    minpq.push({0, 0});
    while (!minpq.empty())
    {
        pii p = minpq.top();
        minpq.pop();
        int node = p.second;
        int wt = p.first;
        if (inMst[node])
            continue;
        totalWt += wt;
        inMst[node] = true;
        for (auto nbr : graph[node])
        {
            int v = nbr.first;
            int needed_wt = nbr.second;
            if (needed_wt < minWt[v])
            {
                minWt[v] = needed_wt;
                minpq.push({needed_wt, v});
            }
        }
    }
    return totalWt;
}
int prims2(vector<vector<pii>> &graph)
{
    int n = graph.size();
    int totalWt = 0;
    vector<bool> inMst(n, false);
    vi minWt(n, -3);
    priority_queue<pii> minpq;
    // minpq contains the minwt needed to connect the node to the current mst
    minpq.push({0, 0});
    while (!minpq.empty())
    {
        pii p = minpq.top();
        minpq.pop();
        int node = p.second;
        int wt = p.first;
        if (inMst[node])
            continue;
        totalWt += wt;
        inMst[node] = true;
        for (auto nbr : graph[node])
        {
            int v = nbr.first;
            int needed_wt = nbr.second;
            if (needed_wt > minWt[v])
            {
                minWt[v] = needed_wt;
                minpq.push({needed_wt, v});
            }
        }
    }
    return totalWt;
}
void solve(int tc)
{
    int n;
    cin >> n;

    vector<vector<pii>> graph(n+1);
    while (true)
    {
        int u, v, w;
        cin >> u >> v >> w;
        if (u == 0 && v == 0 && w == 0)
            break;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
        
    }
    int maxWt = prims2(graph);
    int minWt = prims(graph);
    cout << "Case " << tc << ": ";
    if((maxWt+minWt)%2){
        cout << (maxWt + minWt) << "/" << 2 << endl;
    }
    else
        cout << (maxWt + minWt) / 2 << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int T;
    cin >> T;
    cin.ignore(); // ignore newline after T

    for (int tc = 1; tc <= T; ++tc)
    {
        string blank;
        getline(cin, blank); // read blank line
        solve(tc);
    }


    return 0;
}
