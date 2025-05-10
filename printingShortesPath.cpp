#include <bits/stdc++.h>
using namespace std;
#define yes     cout << "YES" << endl;
#define no      cout << "NO" << endl;
#define vi      vector<int>
#define ll      long long
#define vl      vector<long long>
#define fori    for (int i = 0; i < n; i++) { cin >> arr[i]; }
class Solution
{
public:
    vector<int> shortestPath(int n, int m, vector<vector<int>> &edges)
    {
        //use dijkstra, as the problem statement said that the weight of all the edges will be +ve so no need to use bellman ford
        const int inf = 1e8;
        vector<int> minDist(n+1,inf);
        vector<int> pred(n+1,-1); 
        //source is 1
        minDist[1]=0;
        vector<vector<pair<int, int>>> graph(n + 1);
        for(auto e:edges){
            int u = e[0];
            int v = e[1];
            int wt = e[2];
            graph[u].push_back({v,wt});
        }
        //setting up pq
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minPq;
        minPq.push({0, 1});
        while (!minPq.empty())
        {
            pair<int, int> topel = minPq.top();
            minPq.pop();
            int d = topel.first;
            int u = topel.second;
            if(minDist[u]<d) continue; 
            for(auto& e:graph[u]){
                int dist = e.second;
                int v = e.first;
                if(d+dist < minDist[v]){
                    pred[v] = u;
                    minDist[v] = d + dist;
                    minPq.push({minDist[v], v});
                }
            }
        }
        if(minDist[n]==inf){
            return {-1};

        }
        int i = n;
        vector<int> ans;
        ans.push_back(n);
        while(pred[i]!=-1){
            ans.insert(ans.begin(), pred[i]);
            i = pred[i];
        }
        ans.insert(ans.begin(), minDist[n]);
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}