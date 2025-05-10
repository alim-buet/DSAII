#include <bits/stdc++.h>
using namespace std;
#define yes cout << "YES" << endl;
#define no cout << "NO" << endl;
#define vi vector<int>
#define ll long long
#define vl vector<long long>
#define fori                    \
    for (int i = 0; i < n; i++) \
    {                           \
        cin >> arr[i];          \
    }

class Solution
{
public:
    vector<int> bellmanFord(int V, vector<vector<int>> &edges, int src)
    {
        vector<int> minDist(V, 1e8); // Use 1e8 instead of 10e8 (10e8 = 1e9)
        vector<int> err(1, -1);
        minDist[src] = 0;

        // Relax all edges V - 1 times
        for (int i = 0; i < V - 1; i++)
        {
            for (auto &e : edges)
            {
                int u = e[0], v = e[1], wt = e[2];
                if (minDist[u] != 1e8 && minDist[u] + wt < minDist[v])
                {
                    minDist[v] = minDist[u] + wt;
                }
            }
        }

        // Check for negative-weight cycles
        for (auto &e : edges)
        {
            int u = e[0], v = e[1], wt = e[2];
            if (minDist[u] != 1e8 && minDist[u] + wt < minDist[v])
            {
                return err;
            }
        }

        return minDist;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--)
    {
        solve();
    }
    return 0;
}