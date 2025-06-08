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
int manhattanDist(vector<int> &p1, vector<int> &p2)
{
    return abs(p1[0] - p2[0]) + abs(p2[1] - p1[1]);
}

class Solution
{
public:
    int minCostConnectPoints(vector<vector<int>> &points)
    {
        int n = points.size();
        vector<bool> visited(n, false);
        vector<int> minConnection(n, INT32_MAX);
        minConnection[0] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minpq;
        // minpq will store (wt,u) -> the min wt needed to add the node u to the existing tree
        minpq.push({0, 0});
        int totalWt = 0;
        while (!minpq.empty())
        {
            pii p = minpq.top();
            minpq.pop();
            int wt_need = p.first;
            int node = p.second;
            if (visited[node] || minConnection[node] < wt_need) // if it is visited that means it is already in the mst, if the min connection is less than current connection then baba tumar add korar dorkar nei there is another guy who can connect that node in the mst with a better cost
                continue;
            visited[node] = true;
            totalWt += wt_need;
            for (int nbr = 0; nbr < n; nbr++)
            {
                if (!visited[nbr])
                {
                    // so neighbour is not in the mst yet
                    int mdist = manhattanDist(points[node], points[nbr]);
                    if (mdist < minConnection[nbr])
                    {
                        minConnection[nbr] = mdist;
                        minpq.push({mdist, nbr});
                    }
                }
            }
        }
        return totalWt;
    }
};

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