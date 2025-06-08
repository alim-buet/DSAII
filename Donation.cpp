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
#define yes cout << "YES" << endl;
#define no cout << "NO" << endl;
#define vi vector<int>
#define vl vector<long long>
#define pii pair<int, int>
#define pll pair<long long, long long>
#define ll long long
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
    for (int i = 0; i < n; i++)
    {
        if (inMst[i] == 0)
            return -1;
    }
    return totalWt;
}
void solve(int caseNum)
{
    int n;
    cin >> n;
    vector<vector<pii>> graph(n);
    int totalCable = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int wt;
            cin >> wt;
            if (wt == 0)
                continue;
            graph[i].push_back({j, wt});
            graph[j].push_back({i, wt});
            totalCable += wt;
        }
    }
    // apply prims
    int needed_min_cable = prims(graph);
    int ans = (needed_min_cable == -1) ? needed_min_cable : totalCable - needed_min_cable;
    cout << "Case " << caseNum << ": " << ans << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    string blank;
    getline(cin, blank); // consume newline after T

    for (int caseNum = 1; caseNum <= T; ++caseNum)
    {
        getline(cin, blank); // consume blank line before each case
        solve(caseNum);
    }

    return 0;
}