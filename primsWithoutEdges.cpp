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
    return totalWt;
}
void solve()
{
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