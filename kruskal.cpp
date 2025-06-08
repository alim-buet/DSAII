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

class DisjointSet
{
    vector<int> parent, rank, size;

public:
    DisjointSet(int n)
    {
        parent.resize(n + 1);
        rank.resize(n + 1, 0); 
        size.resize(n + 1, 1); 

        for (int i = 1; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    int findParent(int node)
    {
        if (parent[node] == node)
            return node;
        return parent[node] = findParent(parent[node]); 
    }

    void unionByRank(int u, int v)
    {
        int uSet = findParent(u);
        int vSet = findParent(v);

        if (uSet != vSet)
        {
            if (rank[uSet] < rank[vSet])
            {
                parent[uSet] = vSet;
            }
            else if (rank[uSet] > rank[vSet])
            {
                parent[vSet] = uSet;
            }
            else
            {
                parent[uSet] = vSet;
                rank[vSet]++; 
            }
        }
    }

    void unionBySize(int u, int v)
    {
        int uSet = findParent(u);
        int vSet = findParent(v);

        if (uSet != vSet)
        {
            if (size[uSet] < size[vSet])
            {
                parent[uSet] = vSet;
                size[vSet] += size[uSet]; 
            }
            else
            {
                parent[vSet] = uSet;
                size[uSet] += size[vSet]; 
            }
        }
    }

    int getSize(int u)
    {
        int uSet = findParent(u);
        return size[uSet];
    }

    int getRank(int u)
    {
        int uSet = findParent(u);
        return rank[uSet];
    }
};
void solve()
{
    int n, m;
    cin >> n >> m;
    vector<vector<pii>> graph(n + 1);
    vector<pair<int, pii>> edges;

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, {u, v}});
        edges.push_back({w, {v, u}});
    }
    DisjointSet ds(n);
    sort(edges.begin(), edges.end());
    vector<pii> ans;
    int totalWt = 0;
    for (auto e : edges)
    {
        int wt = e.first;
        int u = e.second.first;
        int v = e.second.second;
        if (ds.findParent(u) == ds.findParent(v))
            continue;
        else
        {
            totalWt += wt;
            ans.push_back({u, v});
            ds.unionByRank(u, v);
        }
    }
    cout << "Total weight : " << totalWt << endl;
    cout << "Edges in the tree: ";
    for (auto p : ans)
    {
        cout << p.first << " " << p.second << endl;
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