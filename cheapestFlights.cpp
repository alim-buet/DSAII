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
    int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k)
    {
        const int inf = 1e8;
        vector<int> minDist(n, inf);
        minDist[src] = 0;

        for (int i = 0; i <= k; i++)
        {
            vector<int> temp = minDist;

            for (auto &f : flights)
            {
                int from = f[0];
                int to = f[1];
                int cost = f[2];

                if (minDist[from] != inf && minDist[from] + cost < temp[to])
                {
                    temp[to] = minDist[from] + cost;
                }
            }

            minDist = temp;
        }

        return (minDist[dst] == inf) ? -1 : minDist[dst];
    }
};

class Solution
{
public:
    int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k)
    {
        // using dijksra
        // forming the graph
        vector<vector<pair<int, int>>> graph(n);
        for (auto e : flights)
        {
            graph[e[0]].push_back(make_pair(e[1], e[2]));
        }
        const int inf = 1e8;
        vector<int> minDist(n, inf);
        minDist[src] = 0;
        //stops, node, dist
        queue<pair<int,pair<int,int>>> q;
        q.push({0, {src, 0}});
        while(!q.empty()){
            pair<int,pair<int,int>> p = q.front();
            q.pop();
            int stops = p.first;
            int u = p.second.first;
            int d = p.second.second;
            if(stops>k) continue;
            for(auto e:graph[u]){
                int v = e.first;
                int cost = e.second;
                if(cost+d < minDist[v] && stops<=K){
                    minDist[v] = cost+d;
                    q.push({stops + 1, {v, minDist[v]}});
                }
            }

        }
        return (minDist[dst] == inf) ? -1 : minDist[dst];
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